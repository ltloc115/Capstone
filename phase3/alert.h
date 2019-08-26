#ifndef _ALERT_H
#define _ALERT_H

#include "driver/adc.h"
int trig = 0; // global variable for trigger
byte filename;
int m_sample = 0;
int baby_alert_repeat;

void filename_value (byte filename)
{
  switch (filename)
  {
    case 2:
      m_sample = open("/sdcard/untitled.wav", O_RDONLY);
      break;

    case 4:
      m_sample = open("/sdcard/imperial.wav", O_RDONLY);
      break;

    case 6:
      m_sample = open("/sdcard/leanFord.wav", O_RDONLY);
      break;

    case 8:
      m_sample = open("/sdcard/leanLeft.wav", O_RDONLY);
      break;

    case 10:
      m_sample = open("/sdcard/leanRight.wav", O_RDONLY);
      break;

    case 12:
      m_sample = open("/sdcard/chinAlert.wav", O_RDONLY);
      break;

    case 14:
      m_sample = open("/sdcard/wetAlert.wav", O_RDONLY);
      break;


    default:
      break;
  }

  filename = 0;
  Serial.println(m_sample);
  if (m_sample < 0) {
    Serial.println("Failed to load file");
  }


}



void play_audio (char* filename_pl, char* filename_pl2)
{

  ESP_LOGI(TAG, "Opening file 1");
  FILE *f;
  f = fopen(filename_pl, "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file:%s", filename_pl);
    return;
  }
  ESP_LOGI(TAG, "Opening file 2");
  FILE *f2;
  f2 = fopen(filename_pl2, "r");
  if (f2 == NULL) {
    ESP_LOGE(TAG, "Failed to open file:%s", filename_pl2);
    return;
  }

  WAV_HEADER wav_head;
  //fprintf(f, "Hello %s!\n", card->cid.name);
  int rlen = fread(&wav_head, 1, sizeof(wav_head), f);
  if (rlen != sizeof(wav_head)) {
    ESP_LOGE(TAG, "read faliled");
    return;
  }
  int channels = wav_head.wChannels;
  int frequency = wav_head.nSamplesPersec;
  int bit = wav_head.wBitsPerSample;
  int datalen = wav_head.wSampleLength;
  (void)datalen;
  ESP_LOGI(TAG, "channels:%d,frequency:%d,bit:%d\n", channels, frequency, bit);

  WAV_HEADER wav_head2;
  //fprintf(f, "Hello %s!\n", card->cid.name);
  int rlen2 = fread(&wav_head2, 1, sizeof(wav_head2), f2);
  if (rlen2 != sizeof(wav_head2)) {
    ESP_LOGE(TAG, "read faliled");
    return;
  }
  int channels2 = wav_head2.wChannels;
  int frequency2 = wav_head2.nSamplesPersec;
  bit = wav_head2.wBitsPerSample;
  int datalen2 = wav_head2.wSampleLength;
  (void)datalen2;
  ESP_LOGI(TAG, "channels:%d,frequency:%d,bit:%d\n", channels2, frequency2, bit);
  do {
    if (feof(f) || feof(f2)) {
      //          dacWrite(25, 190);
      break;
    }

    delayMicroseconds(1);
    dac_output_voltage(DAC_CHANNEL_1, fgetc(f));
    //    delayMicroseconds(6);
    dac_output_voltage(DAC_CHANNEL_2, fgetc(f2));
    delayMicroseconds(1);

  } while (1);
  delay(2);
  dac_output_voltage(DAC_CHANNEL_1, 0);
  dac_output_voltage(DAC_CHANNEL_2, 0);
  delay(100);
  fclose(f);
  fclose(f2);
  //  f = NULL;
  //  f2 = NULL;
}




void dacwrite_0 ()
{
  dacWrite(25, 0);
  dacWrite(26, 0);
}



void case_statement_audio_files (char val)
{
  char filename_phonecall[25];
  char filename_phonecall1[26];

  int delay_val;
  delay_val = 20;

  switch (val) {

    case '0':
      {
        Serial.println ("0\n");
        strcpy(filename_phonecall, "/sdcard/another/0.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i0.wav");

        delay (delay_val);
        break;
      }

    case '1':
      {
        Serial.println ("1\n");
        strcpy(filename_phonecall, "/sdcard/another/1.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i1.wav");

        delay(delay_val);
        break;
      }

    case '2':
      {
        Serial.println ("2\n");
        strcpy(filename_phonecall, "/sdcard/another/2.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i2.wav");

        delay(delay_val);
        break;
      }

    case '3':
      {
        Serial.println ("3\n");
        strcpy(filename_phonecall, "/sdcard/another/3.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i3.wav");

        delay(delay_val);
        break;
      }

    case '4':
      {
        Serial.println ("4\n");
        strcpy(filename_phonecall, "/sdcard/another/4.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i4.wav");

        delay(delay_val);
        break;
      }

    case '5':
      {
        Serial.println ("5\n");
        strcpy(filename_phonecall, "/sdcard/another/5.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i5.wav");

        delay(delay_val);
        break;
      }

    case '6':
      {
        Serial.println ("6\n");
        strcpy(filename_phonecall, "/sdcard/another/6.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i6.wav");

        delay(delay_val);
        break;
      }

    case '7':
      {
        Serial.println ("7\n");
        strcpy(filename_phonecall, "/sdcard/another/7.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i7.wav");

        delay(delay_val);
        break;
      }

    case '8':
      {
        Serial.println ("8\n");
        strcpy(filename_phonecall, "/sdcard/another/8.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i8.wav");

        delay(delay_val);
        break;
      }

    case '9':
      {
        Serial.println ("9\n");
        strcpy(filename_phonecall, "/sdcard/another/9.wav");
        strcpy(filename_phonecall1, "/sdcard/another/i9.wav");

        delay(delay_val);
        break;
      }
    case '-':
      {
        Serial.println ("-\n");
        strcpy(filename_phonecall, "/sdcard/another/neg.wav");
        strcpy(filename_phonecall1, "/sdcard/another/ineg.wav");

        delay(delay_val);
        break;
      }

    case '.':
      {
        Serial.println (".\n");
        strcpy(filename_phonecall, "/sdcard/point.wav");
        strcpy(filename_phonecall1, "/sdcard/ipoint.wav");

        delay(delay_val);
        break;
      }

    default: /* Optional */
      Serial.println ("Can't read this shit");

  }// switch statement


  delay (200);
  play_audio (filename_phonecall, filename_phonecall1);
  //  dacwrite_0 ();
  delay(350);
}




void easy_alert () {
  Serial.println(F("Entering Easy Alert"));
  delay (1000);

  Serial.println("----------------------------------------------------");
  trig = 1;
  delay (10000);
  trig = 0;
  delay (10000);

  // Close file
  close(m_sample);
}


void hard_alert (int a, int b) {

  int i;
  Serial.println(F("Entering Hard Alert"));
  delay (1000);
  Serial.println("----------------------------------------------------");
  //  sendto = "4167169452";
  //  sendto = "6477162554";
  if (a == 1)
  {
    sendto = "6477063072";
  }
  else if (b == 1)
  {
    sendto = "4167169452";
  }
  headphone_jack ();
  phonecall (sendto);


  Serial.println("---------------------Test 1-------------------------------");
  delay (500); // Wait 10 seconds for phone call pick up


  Serial.println("---------------------Playing Tien's File ------------------------------");
  play_audio ("/sdcard/another/initCall.wav", "/sdcard/another/iinitCall.wav");
  delay (1500); // Wait 5 seconds before outputting the coordinates
  Serial.println("---------------------Test 2-------------------------------");


  play_audio ("/sdcard/another/latSay.wav", "/sdcard/another/ilatSay.wav");
  delay (800);


  Serial.println ("Reading out Latitude");
  Serial.println (lat);
  for (i = 0; i < 6; i++)
  {
    case_statement_audio_files (lat[i]);
  }

  Serial.println("---------------------Test 3-------------------------------");
  play_audio ("/sdcard/another/longSay.wav", "/sdcard/another/ilongSay.wav");
  delay (800);


  Serial.println ("Reading out Longitude");
  Serial.println (lon);
  for (i = 0; i < 6; i++)
  {
    case_statement_audio_files (lon[i]);
  }

  delay (1000);

}







int pin_reset () {

  int input;
  int output = 0;
  Serial.println ("Entering pin reset");
  //A8 channel 4
  //  pinMode(A8, INPUT);  // sets the digital pin 15 as input
  //  input = digitalRead(A8);
  adc2_get_raw(ADC2_CHANNEL_3, ADC_WIDTH_12Bit, &input); // A8
  Serial.println ("Input reads: ");
  Serial.print (input);
  // reads 4095 when not pressed
  // reads 0 when pressed

  if (input == 0)
  {
    Serial.println (input);
    Serial.println ("Entering loop for getting A8 value");

    // Set alert 1 to announce reset of product
    filename_value (2);
    easy_alert ();

    // Set value as 1 to return
    output = 1;
  }



  return output;

}








void medium_alert () {
  int pin_answer;
  int hard_count;
  hard_count = 0;
  while (1) {
    Serial.println(F("Entering Medium Alert"));
    delay (4000);
    Serial.println("----------------------------------------------------");
    sendto = "4167169452"; //rad
    //    sendto = "6477162554"; //tien
    //sendto = "6477063072";
    strcpy(message, "You have left your baby behind, please return to Latitude: ");
    strcat(message, lat);
    strcat(message, " Longitude: ");
    strcat(message, lon);

    //    message = "You have left your baby behind, please return to Latitude: ", lat, " Longitude: ", lon;


    text_message (sendto, message);
    count += 1;
    Serial.println(count);
    delay (10000);


    pin_answer = pin_reset ();
    if (pin_answer == 1)
    {
      count = 0;
      break;
    }

    Serial.println("Post Pin reset");


    if (hard_count == 1)
    {
      hard_alert(1, 0);
      hard_count = 0;



      hangup ();

      pin_answer = pin_reset ();
      if (pin_answer == 1)
      {
        count = 0;
        break;
      }


      delay (20);
    }


    else
    {
      hard_alert(0, 1);
      count = 0;
      hard_count += 1; // increase the hard count to go to other emergency personnel


      hangup ();



      pin_answer = pin_reset ();
      if (pin_answer == 1)
      {
        count = 0;
        break;
      }
      delay (20);
    }

  }// while loop

}



int alert (int alert_level) {
  if (alert_level == 1) // easy alert -> Baby positioned weird, something that needs to be adjusted
  {
    delay  (1000);
    Serial.println("Entering Easy Alert");
    easy_alert();
    delay (1000);

  }

  else if (alert_level == 2) // medium alert -> BL disconnected, Baby is not there, etc.
  {
    delay  (1000);
    Serial.println("Entering Medium Alert");
    medium_alert();
    delay  (1000);
  }

  else if (alert_level == 3) // hard alert -> No response from parent
  {
    delay  (1000);
    Serial.println("Entering Hard Alert");
    hard_alert(1, 0);
    delay  (1000);
  }

  return 0;
}


#endif //_ALERT_H
