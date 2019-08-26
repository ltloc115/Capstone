#ifndef _GPS_AND_GSM_LOCATION_H
#define _GPS_AND_GSM_LOCATION_H

#include "Adafruit_FONA.h"
#include <string.h>
#include <stdio.h>
#define FONA_RX 15
#define FONA_TX 16
#define FONA_RST 4

char *token;
char *lat;
char *lon;
int8_t gpsstat;
char gpsdata[120];


// this is a large buffer for replies
char replybuffer[255];

// Hardware serial is also possible!
HardwareSerial *fonaSerial = &Serial2;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t type;

bool gsmnetwork = false;

/*****************Set volume**************************/
void volume_fona()
{
  uint8_t vol = 95;
  Serial.println();
  if (! fona.setVolume(vol)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
}


/***************************Connect to Fona******************************/
void Fona808_intial() {
  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));

  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    default:
      Serial.println(F("???")); break;
  }

  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  fona.setGPRSNetworkSettings(F("chatrweb.apn"));


}





/***************************Initial Fona Connection for GPS******************************/
// Below function will connect to GPS and GPRS
void Fona808_connect () {
  Serial.print(F("FONA> "));

  /*********************************** INITIAL SET UP FOR PROJECT */
  /*********************************** GPRS */

  // turn GPRS OFF then ON
  if (!fona.enableGPRS(false)) {
    Serial.println(F("Failed to turn off"));
    Serial.println("Turn off");
    delay(3000);
  }

  if (!fona.enableGPRS(true)) {
    Serial.println(F("Failed to turn on"));
    Serial.println("Turn on");
    delay(3000);
    gsmnetwork = true;
  }

  else
  {
    gsmnetwork = false;
  }


  /*********************************** GPS (SIM808 only) */

  // turn GPS on
  if (!fona.enableGPS(true))
    Serial.println(F("Failed to turn on"));
  delay (3000);

 /*********************************** Set volume */
  volume_fona();
  
}

void gps_off ()
{
  // turn GPS off
  if (!fona.enableGPS(false))
    Serial.println(F("Failed to turn off"));
}



/***************************Get GPS Location******************************/
void gps_location ()
{
  // check GPS fix
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("Fix Status:"));
  Serial.println(F(""));
  gpsstat = fona.GPSstatus();
  if (gpsstat < 0)
    Serial.println(F("Failed to query"));
  if (gpsstat == 0) Serial.println(F("GPS off"));
  if (gpsstat == 1) Serial.println(F("No fix"));
  if (gpsstat == 2) Serial.println(F("2D fix"));
  if (gpsstat == 3) Serial.println(F("3D fix"));

  delay (3000);

  // check for GPS location
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("GPS Location:"));
  Serial.println(F(""));

  fona.getGPS(0, gpsdata, 120);
  if (type == FONA808_V1)
    Serial.println(F("Format: mode,longitude,latitude,altitude,utctime(yyyymmddHHMMSS),ttff,satellites,speed,course"));
  else
    Serial.println(F("Format: mode,fixstatus,utctime(yyyymmddHHMMSS),latitude,longitude,altitude,speed,course,fixmode,reserved1,HDOP,PDOP,VDOP,reserved2,view_satellites,used_satellites,reserved3,C/N0max,HPA,VPA"));
  Serial.println(gpsdata);
  Serial.println(F("----------------------------------------------------"));

  delay (2000);

}


/*****************Retrieve Lat or Lon variable (GPS or GSM)**************************/
void retrieve_lat_lon ()
{
  // --------------------------------------------------------
  // Extract Long and Lat from GPS
  if (gpsstat >= 2) {
    // if the fix is 2D or 3D
    int i = 0;
    /* get the first token */
    token = strtok(gpsdata, ",");

    /* walk through other tokens */
    while ( token != NULL )
    {
      // Third token represents the latitude
      if (i == 3)
      {
        lat = token;
      }
      // Fourth token represents the longitude
      if (i == 4)
      {
        lon = token;
      }
      i += 1;
      token = strtok(NULL, ",");
    }
  }// if statement for gpsstat


  // --------------------------------------------------------
  // Extract Long and Lat from GSM if GPS is outputting nothing
  if (gpsstat == 1)
    // if no fix
  {
    Serial.println(F("No fix"));
    // check for GSMLOC (requires GPRS)
    uint16_t returncode;


    // Getting location via GSM -> using l in FONATEST
    if (!fona.getGSMLoc(&returncode, replybuffer, 250))
      Serial.println(F("Failed!"));
    if (returncode == 0) {
      Serial.println(replybuffer);
    } else {
      Serial.print(F("Fail code #")); Serial.println(returncode);
    }

    int i = 0;
    token = strtok(replybuffer, ",");
    /* walk through other tokens */
    while ( token != NULL )
    {
      // Third token represents the latitude
      if (i == 1)
      {
        lat = token;
      }
      // Fourth token represents the longitude
      if (i == 0)
      {
        lon = token;
      }
      i += 1;
      token = strtok(NULL, ",");
    }

  }// if statement for stat


}


/*****************Send text message**************************/
void text_message (char* sendto, char* message)
{
  Serial.print(F("Send to #"));
  Serial.println(sendto);
  Serial.print(F("Type out one-line message (140 char): "));
  Serial.println(message);

  if (!fona.sendSMS(sendto, message)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("Sent!"));
  }

}


/*****************Set as headphone jack**************************/
void headphone_jack ()
{
  // Set Headphone output
  if (! fona.setAudio(FONA_HEADSETAUDIO)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  fona.setMicVolume(FONA_HEADSETAUDIO, 15);

}



/*****************Phone call**************************/
void phonecall (char* number)
{
  // call a phone!
  Serial.print(F("Call #"));
  Serial.println(number);
  Serial.print(F("Calling "));
  Serial.println(number);
  if (!fona.callPhone(number)) {
    Serial.println(F("Failed"));
  }
  else {
    Serial.println(F("Sent!"));
    //
    // Where the phone call gets made
  }
}

void hangup ()
{
  // hang up!
  if (! fona.hangUp()) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

}


#endif //_SETUP_BLE_H
