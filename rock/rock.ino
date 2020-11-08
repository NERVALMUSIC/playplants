#include "src/RF24/RF24.h"
#include "src/RF24/nRF24L01.h"
#include "src/RF24/RF24_config.h"
#include "src/MPR121/MPR121.h"
#include "thisrock.h"
#include <SPI.h>


//Radio instance and variables
RF24 radio(PINCE, PINCS);
uint8_t data[BUFF]; //data [0] is the channel - data[1] is hte note - data[2] es the message type - data[3] is the velocity

//Sensor instance and variables
int16_t maxdiff[SENSORS];
int16_t mindiff[SENSORS];
int16_t filteredCC[SENSORS];
uint8_t windowCount[SENSORS];
bool touching[SENSORS];

//Midi instance
#ifdef CONTROL
  uint8_t mode[sizeof(ROCKS)];      //variable to set operation mode for each connected rock
#endif

void setup(void){     //call to all setup functions and start serial port
  MPR121.begin(I2CADDR);
  MPRconfig();
  radio.begin();
  NRFconfig();
  Serial.begin(115200);
#ifdef CONTROL
  Serial.println("Control");
#else
  Serial.println("Sender");
#endif
}

void MPRconfig(){
  /*
   * Para un mejor control de la sensibilidad y la deteccion de toques consultar:
   * Datasheet:
   * AN3889: https://www.nxp.com/docs/en/application-note/AN3889.pdf
   * AN3890: https://www.nxp.com/docs/en/application-note/AN3890.pdf
   */
   
    MPR121.goFast();      //I2C speed
    MPR121.setTouchThreshold(TOUCH);      //Set Thresholds
    MPR121.setReleaseThreshold(RELEASE);  //Set Thresholds
    MPR121.setProxMode(PROX_0_11);  //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11
    //MPR121.setCalibrationLock(CAL_LOCK_DISABLED); //CAL_LOCK_ENABLED CAL_LOCK_DISABLED CAL_LOCK_ENABLED_5_BIT_COPY CAL_LOCK_ENABLED_10_BIT_COPY
    
  /*
  * Optional settings, test best configuration for each plant,
  * see datasheet and application notes to better understand how
  * to change these settings.
  */
  
    delay(200);
    MPR121.autoSetElectrodes(false);  // autoset all electrode settings
    //MPR121.autoSetElectrodeCDC();
    delay(200);    

}

void NRFconfig(void){   //Radio stuff don't look
  radio.setChannel(120);
  radio.setPALevel(RF24_PA_HIGH); //RF24_PA_MIN RF24_PA_LOW RF24_PA_HIGH RF24_PA_MAX
  radio.setDataRate(RF24_1MBPS);  //RF24_250KBPS RF24_1MBPS RF24_2MBPS
  #ifdef CONTROL
    for(int i = 0; i < 4; i += 1){
      radio.openReadingPipe(i+1,PIPES[i]);
    }
    radio.startListening();
  #else
    radio.openWritingPipe(PIPES[HEAD-1]);
  #endif
}

void debug(){
  #ifdef DEBUG
  Serial.print("note");Serial.print("\t|");Serial.print("message");Serial.print("\t|");Serial.println("Velocity");
  Serial.print(data[1]);Serial.print("\t|");Serial.print(data[2],HEX);Serial.print("\t|");Serial.println(data[3]);
  #endif
}


#ifndef CONTROL   // Transmiter functions
  void sendRadio() {
    data[0] = HEAD;     //envia el canal (Editar thisrock)
    MPR121.updateAll();
    //delay(10);
    for(uint8_t n=0; n<SENSORS; n++){
      int16_t diff = MPR121.getFilteredData(n)-MPR121.getBaselineData(n);
      if( diff > maxdiff[n]){maxdiff[n] = diff;}
      else if( diff < mindiff[n]){mindiff[n] = diff;}
      data[1] = n; //envia la nota (Editar thisrock)
      data[2] = CC;
      data[3] = map(diff,maxdiff[n],mindiff[n],0,127);

      if (MPR121.isNewTouch(n) && n != 12 ) {     //check if there is a new touch on the electrode
        data[2] = NOTE_ON;
        radio.write(data, sizeof data);
        debug();
        touching[n] = true;
      }
      
      else if (MPR121.isNewRelease(n) && n != 12 ) {   //check if the touch has just been lifted
        data[2] = NOTE_OFF;
        radio.write(data, sizeof data);
        debug();
        touching[n] = false;
      }
      
      if (n == 12 ){
        if(data[3] >=20){
          if(windowCount[n] == WINDOW){
            data[3] = filteredCC[n] / WINDOW;
            windowCount[n] = 0;
            filteredCC[n] = 0; 
            radio.write(data, sizeof data);
            //debug();
          }
          else{
            filteredCC[n] = filteredCC[n] + int16_t(data[3]);
            windowCount[n] += 1;
          }
        }
        touching[n] = false;
      }
      
      if(touching[n] == true){
        if(windowCount[n] == WINDOW){
          data[3] = filteredCC[n] / WINDOW;
          windowCount[n] = 0;
          filteredCC[n] = 0; 
          radio.write(data, sizeof data);
          debug();
        }
        else{
          filteredCC[n] = filteredCC[n] + int16_t(data[3]);
          windowCount[n] += 1;
        }
      }
    }
  }

#else   //Receiver funcions

  void setMode() {
    for(uint8_t n=0; n < sizeof(ROCKS); n++){
       MPR121.updateTouchData();
       if(MPR121.getTouchData(ROCKS[n])){
         for(uint8_t m=0; m < sizeof(MODES); m++){
           if(MPR121.getTouchData(MODES[m])){
             mode[n] = m;
           }
         }
         if(MPR121.getTouchData(STOP)){
          for(uint8_t m=0; m<128; m++){
                  sendMIDI(NOTE_OFF, n+1, m, 0);     //Send data as received (default mode)
                }
         }
       while(!MPR121.isNewRelease(ROCKS[n])){MPR121.updateTouchData();}       
       sendMIDI(CC, HEAD, 20+n, mode[n]);     //Send data as received (default mode)   
       }
     }
  }
   
  void playMusic() {
    switch (mode[data[0]-1])   //choose mode for the channel received based on last time it was configured
    {
      case 0:
      if( data[1] != 12 && (data[2] == NOTE_ON || data[2] == NOTE_OFF)){
        sendMIDI(data[2], data[0], NOTES1[data[0]][data[1]], data[3]);     //Send notes when touched
      }
      break;
      case 1:
      if( data[1] != 12 && (data[2] == NOTE_ON || data[2] == NOTE_OFF)){
        sendMIDI(data[2], data[0], NOTES2[data[0]][data[1]], data[3]);     //Send notes when touched
      }
      if( data[1] != 12){
        sendMIDI(data[2], data[0], NOTESCC[data[0]][data[1]], data[3]);     //Send CC based on intensity
      }
      break;
      case 2:
      if( data[1] == 12){
        sendMIDI(data[2], data[0], NOTESPROX[data[0]], data[3]);     //Send CC based on proximity to rock
      }
      break;
      default:
      sendMIDI(data[2], data[0], data[1], data[3]);     //Send data as received (default mode)
      break;
    }
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
  Serial.print(channel);Serial.print("\t|");Serial.print(messageType,HEX);Serial.print("\t\t|");Serial.print(data1);Serial.print("\t|");Serial.println(data2);
  #else
  statusByte -= 1;        // to correct hairlessmidi offset
  Serial.write(statusByte);                    // Send over Serial
  Serial.write(data1);
  Serial.write(data2);
  #endif
}
  
#endif


void loop(void){
  #ifndef CONTROL   // Transmiter code
    sendRadio();
  #else   //Receiver code
  uint8_t c = 0;
  while(1){
    if (c == 100){
      setMode();
      c = 0;
    }
    if (radio.available()){
      radio.read(data, sizeof data);    //load buffer
      //debug();
      playMusic();
    }
    c += 1; 
  }

  #endif     //Store last value to compare and get new touches and releases
}
