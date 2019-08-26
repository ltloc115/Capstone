#ifndef _HARDWARE_FUNCTIONS_H
#define _HARDWARE_FUNCTIONS_H

#include "DHT.h"
#define DHT1PIN 13
#define DHTPIN 32     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#include "driver/adc.h"

int a1;
int b1;
int c1;
int d1;

int a2;
int b2;
int c2;
int d2;


DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHTPIN, DHTTYPE);


void PostureAlert(int a, int b, int c, int d) {

  int alert_level = 0;
  int requirement;
  requirement = 2;

  if ((a > 2000) && (b > 2000) && ( c >= 0) && (d >= 0))
  {
    Serial.println("Normal Posture");
    a1 = 0;
    b1 = 0;
    c1 = 0;
    d1 = 0;
  }

  if (((a > 1.8 * b) || (a > 1.8 * b) && (c >= 0) && (d >= 0)) && (a > 1400))
  {
    Serial.println ("Lean Right Posture");

    a1 += 1;
    b1 = 0;
    c1 = 0;
    d1 = 0;

    // Set alert level for leaning right
    // Frog
    if (a1 == requirement)
    {
      filename_value (10);
      alert_level = 1;
      // Counting the number of times the sensors occurs
      a1 = 0;
      b1 = 0;
      c1 = 0;
      d1 = 0;

      // Counting the number of times the alert occurs
      a2 += 1;
      b2 = 0;
      c2 = 0;
      d2 = 0;
    }

  }



  if (((b > 1.8 * a) || (b > 1.8 * a) && (c >= 0) && (d >= 0)) && (b > 1400))
  {
    Serial.println ("Lean Left Posture");
    // Set alert level for leaning  left
    // Frog
    a1 = 0;
    b1 += 1;
    c1 = 0;
    d1 = 0;

    // Set alert level for leaning right
    // Frog
    if (b1 == requirement)
    {
      filename_value (8);
      alert_level = 1;
      a1 = 0;
      b1 = 0;
      c1 = 0;
      d1 = 0;

      // Counting the number of times the alert occurs
      a2 = 0;
      b2 += 1;
      c2 = 0;
      d2 = 0;
    }

  }



  if ((c >= 2000) && (a < 800) && (b < 800))
  {
    Serial.println("Chin to Chest Posture");
    a1 = 0;
    b1 = 0;
    c1 += 1;
    d1 = 0;

    // Set alert level for leaning right
    // Frog
    if (c1 == requirement)
    {
      filename_value (12);
      alert_level = 1;
      a1 = 0;
      b1 = 0;
      c1 = 0;
      d1 = 0;


      // Counting the number of times the alert occurs
      a2 = 0;
      b2 = 0;
      c2 += 1;
      d2 = 0;
    }


  }


  if ((d > 0) && (a < 800) && (b < 800) && (c < 800))
  {
    Serial.println ("Leaning Forward Posture");

    a1 = 0;
    b1 = 0;
    c1 = 0;
    d1 += 1;

    // Set alert level for leaning right
    // Frog
    if (d1 == requirement)
    {
      filename_value (6);
      alert_level = 1;
      a1 = 0;
      b1 = 0;
      c1 = 0;
      d1 = 0;


      // Counting the number of times the alert occurs
      a2 = 0;
      b2 = 0;
      c2 = 0;
      d2 += 1;
    }
  }

  // check statement to see if the same alert has gone 2 times in a row
  // if so, go to medium alert
  if ((a2 > requirement) || (b2 > requirement) || (c2 > requirement) || (d2 > requirement))
  {
    a2 = 0;
    b2 = 0;
    c2 = 0;
    d2 = 0;
    alert_level = 2;
  }


  // check alert level for either soft or medium
  alert_level = alert (alert_level);

}






void GasAlert (int e) {

  int alert_level = 0;

  if (e > 1000)
  {
    Serial.println ("CO levels above safety limit");
    // Set alert level for CO above limit
    // Frog
    filename_value (4);
    alert_level = 1;
  }

  if (e < 500)
    Serial.println ("CO levels are Normal");


  if ((e > 500) && (e < 1000))
  {
    Serial.println ("CO levels are slightly elevated");
  }

  // Calls the alert level and enters soft alert
  alert_level = alert (alert_level);

}





//void WetnessAlert (int f, int g) { // flipped
//  int alert_level = 0;
//  if (f - g > 3)
//  {
//        Serial.println ("Baby is wet");
//        // Set alert 1 for baby being wet
//        filename_value (4);
//        alert_level = 1;
//  }
//
//  if (f - g <= 3)
//    Serial.println ("Baby is dry");
//
//
//  alert_level = alert (alert_level);
//}


void WetnessAlert (int f, int g) { // flipped
  int alert_level = 0;


  if (g > 5 + f)
  {
    Serial.println ("Baby is wet");
    // Set alert 1 for baby being wet
    filename_value (14);
    alert_level = 1;
  }

  if (g <= 5 + f)
    Serial.println ("Baby is dry");


//  alert_level = alert (alert_level);
}



void temperatureAlert (int t1,int t2) {
  int alert_level = 0;

  if ((t1 > 28) || (t1 < 15))
  {
    Serial.println ("Temperature is out of condition");
    // Set alert 1 for baby being wet
    filename_value (4);
    alert_level = 1;
  }


  if ((t2 > 28) || (t2 < 15))
  {
    Serial.println ("Temperature is out of condition");
    // Set alert 1 for baby being wet
    filename_value (4);
    alert_level = 1;
  }

//  alert_level = alert (alert_level);
}




void sensor_act_begin ()
{
  pinMode(LED_BUILTIN, OUTPUT);
  dht1.begin();
  dht2.begin();
}




//Task1code: blinks an LED every 1000 ms
void ReadSensCode() {



  int sens1Value;
  int sens2Value;
  int sens3Value;
  int sens4Value;
  int gasSenVal;

  int sensArr[5] = {sens1Value, sens2Value, sens3Value, sens4Value, gasSenVal};


  // Initilaizing ADC 1
  adc1_config_width (ADC_WIDTH_BIT_12);

  //sens1Value = analogRead(sens1);
  adc1_config_channel_atten (ADC1_CHANNEL_6, ADC_ATTEN_DB_0); // A2
  sens1Value = adc1_get_raw (ADC1_CHANNEL_6);


  //sens2Value = analogRead(sens2);
  adc1_config_channel_atten (ADC1_CHANNEL_3, ADC_ATTEN_DB_0); // A3
  sens2Value = adc1_get_raw (ADC1_CHANNEL_3);

  //sens3Value = analogRead(sens3);
  adc2_get_raw(ADC2_CHANNEL_7, ADC_WIDTH_12Bit, &sens3Value); // A10


  //sens4Value = analogRead(sens4);
  adc1_config_channel_atten (ADC1_CHANNEL_0, ADC_ATTEN_DB_0); // A4
  sens4Value = adc1_get_raw (ADC1_CHANNEL_0);

  //gasSenVal = analogRead(gasSen);
  adc2_get_raw(ADC2_CHANNEL_6, ADC_WIDTH_12Bit, &gasSenVal); // A6


  PostureAlert(sens2Value, sens1Value, sens3Value, sens3Value);
  GasAlert(gasSenVal);

  sensArr[0] = map(sens1Value, 0, 4095, 0, 4095);
  sensArr[1] = map(sens2Value, 0, 4095, 0, 4095);
  sensArr[2] = map(sens3Value, 0, 4095, 0, 4095);
  sensArr[3] = map(sens4Value, 0, 4095, 0, 4095);
  sensArr[4] = map(gasSenVal, 0, 4095, 0, 1000);

  Serial.println("FSR 1      FSR 2      FSR 3      FSR 4       Gas");

  for (int i = 0; i < 5; i++) {
    Serial.print(sensArr[i]);
    Serial.print("          ");
  }
  Serial.print("\n");
  delay(500);


}





void sensor_act_main_obtain ()

{

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();

  WetnessAlert (h1, h2);
  temperatureAlert (t1, t2);

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t1) || isnan(h1)) {
    Serial.println("Failed to read from DHT #1");
  }
  else {
    Serial.print("Humidity 1: ");
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temperature 1: ");
    Serial.print(t1);
    Serial.println(" *C");
  }
  if (isnan(t2) || isnan(h2)) {
    Serial.println("Failed to read from DHT #2");
  } else {
    Serial.print("Humidity 2: ");
    Serial.print(h2);
    Serial.print(" %\t");
    Serial.print("Temperature 2: ");
    Serial.print(t2);
    Serial.println(" *C");
  }
  Serial.println();
}







#endif //_HARDWARE_FUNCTIONS_H
