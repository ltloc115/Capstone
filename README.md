# Capstone
Capstone project KU04 of Biomed 2019 at Ryerson University. <br><br>
The Project is a smart pad that can be attached to a toddler/infant's stroller or seat. The pad monitors the conditions that the toddler is in such as temperature, humidity, and location. Various sensors and modules were implemented to measure and record the conditions, and feed them to the microcontroller - an ESP32. <br><br>
 When alarming conditions are detected, the pad plays audio alerts to Bluetooth speakers in the car, or texts alerts to the parents' phones. Emergency service will be alerted if there is no appropriate response from the parents. <br><br>
A GPS module was also implemented to mark the location of the child, which can be sent off in alerts. <br><br>
Individual work includes setting up communication from the ESP32 to an SD card which was used to store pre-recorded audio alerts, and sending audio information from the microcontroller, physically and wirelessly. Interfacing and helping other members' parts were also one of the responsibilities. <br><br>
The newAudio.ino file contains the code for communicating with the SD card and playing audio files to DACs on the ESP32, which can be connected with a physical speaker. The audio files were formatted in .wav. Various sample rates were tested for sound clarity and speed. <br><br>
The bluTest.ino file contains the code for communication with the SD card and streaming audio in .wav format through Bluetooth A2DP on ESP32. The audio files must be configured in the same manner as those used for physical speakers. <br><br>
Phase 3 folder contains the combined code of all the members for the device. <br><br>
To replicate the the demonstration, all the necessary libraries are in the libraries folder. Some components can be seen in the newSchematic.png. The GPS-GSM module was the FONA 808 from Adafruit. <br><br>

![Schematic](https://raw.githubusercontent.com/ltloc115/Capstone/master/newSchematic.png)
