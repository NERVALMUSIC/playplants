 //#include "src/plant/plant.h"
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
const uint64_t pipes[5] = {0xF0F0F0F0D2LL, 0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL };
int myNotes[] = {41, 53, 45, 57, 44, 56, 65, 77, 69, 81, 38, 50 };


//Sensor instance and variables
MPR121 cap = MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

//Midi instance
MIDI_CREATE_DEFAULT_INSTANCE();

void setup(void){
  radio.begin();
  NRFconfig();
  MIDI.begin();
  cap.begin(I2CADDR);
  MPRconfig();
  Serial.begin(115200);
  data[0] = HEAD;
}

void MPRconfig(){
  cap.writeRegister(MPR121_NHDF, 0x01); //noise half delta (falling)
  cap.writeRegister(MPR121_FDLF, 0x3F); //filter delay limit (falling)
  cap.setThresholds(TOUCH, RELEASE);
}

void NRFconfig(void){
  //const uint64_t pipes[]=PIPES;
  if(ROLE == 0){
    for(int i = 0; i < 5; i += 1){
      radio.openReadingPipe(i+1,pipes[i]);
    }
    radio.startListening();
  }
  else{
    radio.openWritingPipe(pipes[HEAD]);
  }
}

void play() {
  const uint8_t channel = data[0] + 1;
  const uint8_t note = data[1];
  bool pressed = data[2];
  const uint8_t velocity = data[3];
  
  if (pressed) {
    MIDI.sendNoteOn(note, 14, channel);
  } else {
    MIDI.sendNoteOff(note, 14, channel);
  }
  if(ROLE==1){
    radio.write(data, sizeof data);
  }
}


void self_check(int n) {
  data[0] = HEAD;
  if ((currtouched & _BV(n)) && !(lasttouched & _BV(n)) ) {
      data[1] = myNotes[n];
      data[2] = 1;
      data[3] = cap.filteredData(n)-cap.baselineData(n);
      play();
  }
  if (!(currtouched & _BV(n)) && (lasttouched & _BV(n)) ) {
      data[1] = myNotes[n];
      data[2] = 0;
      data[3] = cap.filteredData(n)-cap.baselineData(n);
      play();
  }
}

void receive() {
  for(int n=0; n<5; n+=1){
    if (radio.available()) {
      radio.read(data, sizeof data);
      play();
    }
  }
}

void loop(void){
  currtouched = cap.touched();
   for (int i = 0; i < 12; i += 1){
    self_check(i);
    if(ROLE == 0){receive();}
   }
   lasttouched = currtouched;
}
