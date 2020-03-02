//#include "src/plant/plant.h"
#include "src/RF24/RF24.h"
#include "src/RF24/nRF24L01.h"
#include "src/RF24/RF24_config.h"
#include "src/MPR121/MPR121.h"
#include "src/MIDIUSB/MIDIUSB.h"
#include "thisrock.h"
#include <SPI.h>


//Configure radio parameters
RF24 radio(pinCE, pinCS);
const uint8_t bufferSize = 5;
uint8_t data[bufferSize];
#define DO3 48

//configure the sensor parameters
MPR121 mpr121 = MPR121();

//configure the MIDI parameters
MIDI_CREATE_DEFAULT_INSTANCE();

void setup(void){
  radio.begin();
  MIDI.begin();
  Serial.begin(115200);
}

void loop(void){
  
}
