#include "src/RF24/RF24.h"
#include "src/RF24/nRF24L01.h"
#include "src/RF24/RF24_config.h"
#include "src/MPR121/MPR121.h"
#include "thisrock.h"
#include <SPI.h>

#define DEBUG //Uncomment for debug
//Radio instance and variables
RF24 radio(PINCE, PINCS);
uint8_t data[BUFF]; //data [0] is the channel - data[1] is hte note - data[2] es the message type - data[3] is the velocity
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
  radio.setChannel(111);
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
  #ifdef DEBUG
  Serial.print("Channel");Serial.print("\t|");Serial.print("Message");Serial.print("\t|");Serial.print("Byte 1");Serial.print("\t|");Serial.println("Byte 2");
  Serial.print(channel);Serial.print("\t|");Serial.print(messageType);Serial.print("\t\t|");Serial.print(data1);Serial.print("\t|");Serial.println(data2);
  #else
  Serial.write(statusByte);                    // Send over Serial
  Serial.write(data1);
  Serial.write(data2);
  #endif
}

#ifndef CONTROL   // Transmiter code
  void sendRadio(int n) {
    data[0] = HEAD;     //envia el canal (Editar thisrock)
    data[1] = n; //envia la nota (Editar thisrock)
    data[3] = map(constrain(cap.baselineData(n)-cap.filteredData(n),0,300),0,300,0,127);      //get filtered capacitance (puede mejorarse)
    if ((currtouched & _BV(n)) && !(lasttouched & _BV(n)) ) {     //check if there is a new touch on the electrode
      data[2] = NOTE_ON;
    }
    else if (!(currtouched & _BV(n)) && (lasttouched & _BV(n))) {   //check if the touch has just been lifted
      data[2] = NOTE_OFF;
      radio.write(data, sizeof data);
    }
    else{
      data[2] = CC;     //use the Control Change message in this case to be used for special purposes
    }
    //if(currtouched & _BV(n)){      // Only send data if someone is touching the electrode
    //  radio.write(data, sizeof data);
    //}
    radio.write(data, sizeof data);
  #ifdef DEBUG
    Serial.print("Canal | ");Serial.print("Mensaje | ");Serial.print("Nota | ");Serial.println("Velocity");
    Serial.print(data[0]);Serial.print("\t|\t");Serial.print(data[2]);Serial.print("\t|\t");Serial.print(data[1]);Serial.print("\t|\t");Serial.println(data[3]);
  #endif
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
           *                     0    |    1    |    1    |    3
           *                     1    |    0    |    0    |    4
           *                     1    |    0    |    1    |    5
           */
        }
      }
    return b;
  }
#endif
void loop(void){
  currtouched = cap.touched();    //get current Touch value for all electrodes
  #ifndef CONTROL   // Transmiter code
    for (int i = 0; i < 12; i += 1){sendRadio(i);} // loop through electrodes
  #else   //Receiver code
    for (int i = 0; i < sizeof(ROCKS); i += 1) //loop through electrodes defined as rock selectors
    {
      if ((currtouched & _BV(ROCKS[i])) && !(lasttouched & _BV(ROCKS[i])))
      {
        control[i]=checkModes(); //get mode cobination value for recently touched rock selection electrode
        #ifdef DEBUG
          Serial.print("channel: ");Serial.println(i);
          Serial.print("mode: ");Serial.println(control[i]);
        #endif
      }
    }
    if (radio.available()) {    //when new data is available read it and send it through midi
      radio.read(data, sizeof data);    //load buffer
      switch (control[data[0]])   //choose mode for the channel received based on last time it was configured
      {
        case 0:
          if( data[2] == NOTE_ON || data[2] == NOTE_OFF){
            sendMIDI(data[2], data[0], NOTES[data[0]][data[1]], data[3]);     //Send data as received (default mode)
          }
        break;
        case 1:
          if( data[2] == CC){
            sendMIDI(data[2], data[0], NOTES[data[0]][data[1]], data[3]);     //Send data as received (default mode)
          }
        break;
        case 2:
          sendMIDI(CC, data[0], data[1], data[3]);   //Send Control changes only
        break;
        case 3:
          sendMIDI(data[2], data[0], data[1], 127);   //Send data at maximum velocity when touched
        break;
        case 4:
          sendMIDI(data[2], data[0], NOTES[map(data[3],50,120,0,sizeof(NOTES))], data[3]);   //Send NOTES depending on intensity
        break;
        case 5:
            if( data[2] == NOTE_ON){
              sendMIDI(data[2], data[0], data[1], data[3]);     //Send notes with sostenuto
              sendMIDI(CC, data[0], 66, 127);   //Add sostenuto
            }
            else{sendMIDI(data[2], data[0], data[1], data[3]);}     //Send NoteOFF and CC data
        break;
        default:
        break;
      }
    }
  #endif
   lasttouched = currtouched;     //Store last value to compare and get new touches and releases
}
