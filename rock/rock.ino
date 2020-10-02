#include "src/RF24/RF24.h"
#include "src/RF24/nRF24L01.h"
#include "src/RF24/RF24_config.h"
#include "src/MPR121/MPR121.h"
#include "thisrock.h"
#include <SPI.h>

//Radio instance and variables
RF24 radio(PINCE, PINCS);
uint8_t data[BUFF];

//Sensor instance and variables
MPR121 cap = MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

//Midi instance
#ifdef CONTROL
  uint8_t control[sizeof(ROCKS)];      //variable to set operation mode for each connected rock
#endif

void setup(void){     //call to all setup functions and start serial port
  cap.begin(I2CADDR);
  MPRconfig();
  radio.begin();
  NRFconfig();
  Serial.begin(115200);
}

void MPRconfig(){
  /*
   * Para un mejor control de la sensibilidad y la deteccion de toques consultar:
   * Datasheet:
   * AN3889: https://www.nxp.com/docs/en/application-note/AN3889.pdf
   * AN3890: https://www.nxp.com/docs/en/application-note/AN3890.pdf
   */
  cap.writeRegister(MPR121_NHDF, 0x1); //noise half delta (falling)
  cap.writeRegister(MPR121_FDLF, 0x3f); //filter delay limit (falling)
  cap.setThresholds(TOUCH, RELEASE);    // Limits for Note ON and Note OFF signal send
}

void NRFconfig(void){   //Radio stuff don't look
  #ifdef CONTROL
    for(int i = 0; i < 5; i += 1){
      radio.openReadingPipe(i+1,PIPES[i]);
    }
    radio.startListening();
  #else
    radio.openWritingPipe(PIPES[HEAD]);
  #endif
}

void sendMIDI(uint8_t messageType, uint8_t channel, uint8_t data1, uint8_t data2) {
                                               // corresponds to binary channel 0
  messageType &= 0b11110000;                   // Make sure that only the high nibble 
                                               // of the message type is set
  channel     &= 0b00001111;                   // Make sure that only the low nibble
                                               // of the channel is set
  uint8_t statusByte = messageType | channel;  // Combine the messageType (high nibble) 
                                               // with the channel (low nibble)
                                               // Both the message type and the channel
                                               // should be 4 bits wide
  statusByte  |= 0b10000000;                   // Set the most significant bit of the status byte
  data1       &= 0b01111111;                   // Clear the most significant bit of the data bytes
  data2       &= 0b01111111;
  Serial.write(statusByte);                    // Send over Serial
  Serial.write(data1);
  Serial.write(data2);
}

#ifndef CONTROL   // Transmiter code
  void sendRadio(int n) {
    data[0] = HEAD;     //envia el canal (Editar thisrock)
    data[1] = NOTES[n]; //envia la nota (Editar thisrock)
    data[3] = constrain(map(cap.filteredData(n),720,400,0,127),0,127);      //get filtered capacitance (puede mejorarse)
    if ((currtouched & _BV(n)) && !(lasttouched & _BV(n)) ) {     //check if there is a new touch on the electrode
      data[2] = NOTE_ON;
    }
    else if (!(currtouched & _BV(n)) && (lasttouched & _BV(n))) {   //check if the touch has just been lifted
      data[2] = NOTE_OFF;
    }
    else{
      data[2] = KEY_PRESSURE;     //use the aftertouch message in this case as it seems appropiate (can be used as a modulator)
    }
    if(currtouched & _BV(n)){      // Only send data if someone is touching the electrode
      radio.write(data, sizeof data);
    }
  }

#else   //Receiver code
  uint8_t checkModes() {
    uint8_t b = 0;
      for (int n = 0; n < sizeof(MODES); n += 1)    //Loop through Mode selection electrodes
      {
        if (currtouched & _BV(MODES[n]))    //only check if the electrode is currently being touched, not if it is a new touch
        {
          bitWrite(b, n, 1);  //Set bit for each electorde that has a new touch, the table is as follows right now, more modes can be added:
          /*                  mode 2  | mode 1  | mode 0  | Value
           *                 ---------|---------|---------|--------
           *                     0    |    0    |    0    |    0
           *                     0    |    0    |    1    |    1
           *                     0    |    1    |    0    |    2
           *                     1    |    0    |    0    |    3
           *                     0    |    1    |    1    |    4
           *                     1    |    1    |    1    |    5
           */
        }
      }
    return b;
  }
#endif
void loop(void){
  currtouched = cap.touched();    //get current Touch value for all electrodes
  #ifndef CONTROL   // Transmiter code
    for (int i = 0; i < sizeof(NOTES); i += 1){sendRadio(i);} // loop through electrodes
  #else   //Receiver code
    for (int i = 0; i < sizeof(ROCKS); i += 1) //loop through electrodes defined as rock selectors
    {
      if ((currtouched & _BV(ROCKS[i])) && !(lasttouched & _BV(ROCKS[i])))
      {
        control[i]=checkModes(); //get mode cobination value for recently touched rock selection electrode
      }
    }
    if (radio.available()) {    //when new data is available read it and send it through midi
      radio.read(data, sizeof data);    //load buffer
      switch (control[data[0]])   //choose mode for the channel received based on last time it was configured
      {
        case 0:
          sendMIDI(data[2], data[0], data[1], data[3]);     //Send data as received (default mode)
        break;
        case 1:
          randomSeed(data[1]);
          sendMIDI(data[2], data[0], random(30,90), data[3]);   //Send data with random notes between 30 and 90
        break;
        case 2:
          sendMIDI(NOTE_ON, data[0], data[1], data[3]);   //Send data continuously
        break;
        case 3:
          sendMIDI(data[2], data[0], data[1], 127);   //Send data at fixed velocity
        break;
        case 4:
          randomSeed(data[1]);
          sendMIDI(data[2], data[0], NOTES[random(0,sizeof(NOTES))], data[3]);   //Send data with random notes from NOTES
        break;
        case 5:
          sendMIDI(data[2], data[0], 30+map(data[3],57,127,0,24), 127);   //Send notes that vary according to velocity
        break;
        default:
        break;
      }
    }
  #endif
   lasttouched = currtouched;     //Store last value to compare and get new touches and releases
}
