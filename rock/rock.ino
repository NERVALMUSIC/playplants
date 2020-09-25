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
  uint8_t control = 0;
#endif
void setup(void){
  cap.begin(I2CADDR);
  MPRconfig();
  radio.begin();
  NRFconfig();
      Serial.begin(115200);
  #ifdef CONTROL
  #endif  
}

void MPRconfig(){
  cap.writeRegister(MPR121_NHDF, 0x1); //noise half delta (falling)
  cap.writeRegister(MPR121_FDLF, 0x3f); //filter delay limit (falling)
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


void self_check(int n) {
  #ifndef CONTROL
    data[0] = HEAD;
    data[1] = NOTES[n];
          Serial.println(data[3] );

             data[3] = constrain(map(cap.filteredData(n),720,400,0,127),0,127);                
    if ((currtouched & _BV(n)) && !(lasttouched & _BV(n)) ) {
      data[2] = NOTE_ON;
      radio.write(data, sizeof data);
    }
    if (!(currtouched & _BV(n)) && (lasttouched & _BV(n))) {
      data[2] = NOTE_OFF;
      //  Serial.println(data[3] );
        
      radio.write(data, sizeof data);
    }
  #else
      if ((currtouched & _BV(n)) && !(lasttouched & _BV(n)) ) {
        control = NOTES[n];
      }  
    switch (control) {
      case 0:
       for(int n=0; n<5; n+=1){
          if (radio.available()) {
            radio.read(data, sizeof data);
            sendMIDI(data[2], data[0], data[1], data[3]);
          }
        }
        break;
      case 1:
      control=0;
        break;
      default:
      for(int n=0; n<5; n+=1){
          if (radio.available()) {
            radio.read(data, sizeof data);
            sendMIDI(data[2], data[0], data[1], data[3]);
          }
        }
        break;
    }
  #endif
}

void loop(void){
  currtouched = cap.touched();
   for (int i = 0; i < sizeof(NOTES); i += 1){
    self_check(1);
     //  Serial.println(cap.filteredData(1));

   }
   lasttouched = currtouched;
}
