#include "src/RF24/RF24.h"
#include "src/RF24/nRF24L01.h"
#include "src/RF24/RF24_config.h"
#include "src/MPR121/MPR121.h"
#include "src/MIDIUSB/MIDIUSB.h"
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
  MIDI_CREATE_DEFAULT_INSTANCE();
  uint8_t control = 0;
#endif

void setup(void){
  cap.begin(I2CADDR);
  MPRconfig();
  radio.begin();
  NRFconfig();
  #ifdef CONTROL
    Serial.begin(115200);
    MIDI.begin();
  #endif  
}

void MPRconfig(){
  cap.writeRegister(MPR121_NHDF, 0x01); //noise half delta (falling)
  cap.writeRegister(MPR121_FDLF, 0x3F); //filter delay limit (falling)
  cap.setThresholds(TOUCH, RELEASE);
}

void NRFconfig(void){
  #ifdef CONTROL
    for(int i = 0; i < 5; i += 1){
      radio.openReadingPipe(i+1,PIPES[i]);
    }
    radio.startListening();
  #else
    radio.openWritingPipe(PIPES[HEAD]);
  #endif
}

void play(uint8_t note, uint8_t velocity, uint8_t channel, bool pressed) {
  #ifdef CONTROL
    if (pressed) {
      MIDI.sendNoteOn(note, velocity, channel);
    } 
    else {
      MIDI.sendNoteOff(note, velocity, channel);
    }
  #endif
}


void self_check(int n) {
  #ifndef CONTROL
    data[0] = HEAD;
    data[1] = NOTES[n];
    if ((currtouched & _BV(n)) && !(lasttouched & _BV(n)) ) {
      data[2] = 1;
      data[3] = constrain(cap.filteredData(n)-cap.baselineData(n),0,127);
      radio.write(data, sizeof data);
    }
    if (!(currtouched & _BV(n)) && (lasttouched & _BV(n))) {
      data[2] = 0;
      data[3] = cap.filteredData(n)-cap.baselineData(n);
      radio.write(data, sizeof data);
    }
  #else
      if ((currtouched & _BV(n)) && !(lasttouched & _BV(n)) ) {
        control = NOTES[n];
        //Serial.println(control);
      }  
    switch (control) {
      case 0:  
        for(int n=0; n<5; n+=1){
          if (radio.available()) {
            radio.read(data, sizeof data);
            //Serial.print(data[0]); Serial.print("\t");
            //Serial.print(data[1]); Serial.print("\t");
            //Serial.print(data[2]); Serial.print("\t");
            //Serial.println(data[3]);
            play(data[1],data[3],data[0]+1,data[2]);
          }
        }
        break;
      case 1:
        play(10,127,6,1);
        break;
      default:
      //nothing
        break;
    }
  #endif
}

void loop(void){
  currtouched = cap.touched();
   for (int i = 0; i < sizeof(NOTES); i += 1){
    self_check(i);
   }
   lasttouched = currtouched;
}
