#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <Adafruit_MPR121.h>
#include <Wire.h>
 
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);
#define DO3 48
#define TOUCH 30
#define RELEASE 5
// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe[5] = {0xF0F0F0F0D2LL, 0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL };
const int head = 0;
uint8_t data[4] = {head,DO3,0,0};
Adafruit_MPR121 mpr121 = Adafruit_MPR121();
uint8_t mpr121Address = 0x5A;
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

void setup(void)
{
   Serial.begin(115200);
   // Iniciar el sensor
   if (!mpr121.begin(mpr121Address)) {
      Serial.println("MPR121 no encontrado");
      while (1);
   }
   Serial.println("MPR121 iniciado");

  // slow down some of the MPR121 baseline filtering to avoid 
  // filtering out slow hand movements
  mpr121.writeRegister(MPR121_NHDF, 0x01); //noise half delta (falling)
  mpr121.writeRegister(MPR121_FDLF, 0x3F); //filter delay limit (falling)
  mpr121.setThresholds(TOUCH, RELEASE);

  //start the radio
  radio.begin();
  radio.openWritingPipe(pipe[head]);
}
 
void loop(void)
{
   currtouched = mpr121.touched();
   for (int i = 0; i < 12; i += 1){
     data[1] = DO3+i;
     if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      data[2] = 1;
      data[3] = map(mpr121.filteredData(i)-mpr121.baselineData(i),RELEASE,2*TOUCH,0,127);
      show(data);
      radio.write(data, sizeof data); 
    }
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      data[2] = 0;
      data[3] = map(mpr121.filteredData(i)-mpr121.baselineData(i),RELEASE,2*TOUCH,0,127);
      show(data);
      radio.write(data, sizeof data); 
    }
   }
   lasttouched = currtouched;
}

void show(uint8_t data[]){
      Serial.print("header1: ");
      Serial.print(data[0]);
      Serial.print("  note: ");
      Serial.print(data[1]);
      Serial.print("  keypress: ");
      Serial.print(data[2]);
      Serial.print("  velocity: ");
      Serial.println(data[3]);
      
}
