/* SD card and FAT filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "sdmmc_cmd.h"
#include "driver/dac.h"

// This example can use SDMMC and SPI peripherals to communicate with SD card.
// By default, SDMMC peripheral is used.
// To enable SPI mode, uncomment the following line:

#define USE_SPI_MODE

// When testing SD and SPI modes, keep in mind that once the card has been
// initialized in SPI mode, it can not be reinitialized in SD mode without
// toggling power to the card.

#ifdef USE_SPI_MODE
// Pin mapping when using SPI mode.
// With this mapping, SD card can be used both in SPI and 1-line SD mode.
// Note that a pull-up on CS line is required in SD mode.
#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_MOSI GPIO_NUM_18
#define PIN_NUM_CLK  GPIO_NUM_5
#define PIN_NUM_CS   GPIO_NUM_21
#endif //USE_SPI_MODE

typedef struct 
{
    char rld[4];    //riff 
    int  rLen;      //
    char wld[4];    //（wave）
    char fld[4];    //"fmt"
 
    int fLen;   //sizeof(wave format matex)
 
    short wFormatTag;   
    short wChannels;   
    int   nSamplesPersec; 
    int   nAvgBitsPerSample;
    short wBlockAlign; //
    short wBitsPerSample; 
 
    char dld[4];        //”data“
    int  wSampleLength; 
 }WAV_HEADER;


void setup()
{

    Serial.begin(115200);
    ESP_LOGI(TAG, "Initializing SD card");

#ifndef USE_SPI_MODE
    ESP_LOGI(TAG, "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // To use 1-line SD mode, uncomment the following line:
    // slot_config.width = 1;

    // GPIOs 15, 2, 4, 12, 13 should have external 10k pull-ups.
    // Internal pull-ups are not sufficient. However, enabling internal pull-ups
    // does make a difference some boards, so we do that here.
    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
    gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);   // D2, needed in 4-line mode only
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes

#else
    ESP_LOGI(TAG, "Using SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = PIN_NUM_MISO;
    slot_config.gpio_mosi = PIN_NUM_MOSI;
    slot_config.gpio_sck  = PIN_NUM_CLK;
    slot_config.gpio_cs   = PIN_NUM_CS;
    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
#endif //USE_SPI_MODE

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
    
    ESP_LOGI(TAG, "Opening file 1");

    WAV_HEADER wav_head;
    FILE *f= fopen("/sdcard/original-8-bit.wav", "r");
    if (f == NULL) {
        ESP_LOGE(TAG,"Failed to open file:%s","/sdcard/original-8-bit.wav");
        return;
    }

    //fprintf(f, "Hello %s!\n", card->cid.name);
    int rlen=fread(&wav_head,1,sizeof(wav_head),f);
    if(rlen!=sizeof(wav_head)){
        ESP_LOGE(TAG,"read faliled");
        return;
    }
    int channels = wav_head.wChannels;
    int frequency = wav_head.nSamplesPersec;
    int bit = wav_head.wBitsPerSample;
    int datalen= wav_head.wSampleLength;
    (void)datalen;
    ESP_LOGI(TAG,"channels:%d,frequency:%d,bit:%d\n",channels,frequency,bit);

    ESP_LOGI(TAG, "Opening file 2");
    FILE *f2;
    f2= fopen("/sdcard/ioriginal-8-bit.wav", "r");
    if (f2 == NULL) {
        ESP_LOGE(TAG,"Failed to open file:%s","/sdcard/ioriginal-8-bit.wav");
        return;
    }

    WAV_HEADER wav_head2;
    //fprintf(f, "Hello %s!\n", card->cid.name);
    int rlen2=fread(&wav_head2,1,sizeof(wav_head2),f2);
    if(rlen2!=sizeof(wav_head2)){
        ESP_LOGE(TAG,"read faliled");
        return;
    }
    int channels2 = wav_head2.wChannels;
    int frequency2 = wav_head2.nSamplesPersec;
    bit = wav_head2.wBitsPerSample;
    int datalen2= wav_head2.wSampleLength;
    (void)datalen2;
    ESP_LOGI(TAG,"channels:%d,frequency:%d,bit:%d\n",channels2,frequency2,bit);
    
    do {
        if (feof(f) || feof(f2)) {  
          fclose(f);
          fclose(f2);
          break;
        }
        dacWrite(25, fgetc(f));
        dacWrite(26, fgetc(f2));
        delayMicroseconds(5);
//        delay(1);
    } while(1);
    
    f=NULL;
    f2 = NULL;
}

void loop() {

}
