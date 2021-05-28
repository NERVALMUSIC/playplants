#include "src/RF24/RF24.h"
#include "src/RF24Network/RF24Network.h"
#include "src/RF24Mesh/RF24Mesh.h"
#include "src/MPR121/MPR121.h"
#include "thisrock.h"
#include <SPI.h>


//Radio instance and variables
RF24 radio(PINCE, PINCS);
RF24Network network(radio);
RF24Mesh mesh(radio,network);
struct MIDI_payload {
  uint8_t channel;
  uint8_t note;
  uint8_t message;
  uint8_t velocity;
};

MIDI_payload payload;

//Sensor instance and variables
int16_t raw[SENSORS];
int16_t maxdiff[SENSORS];
int16_t mindiff[SENSORS];
int16_t filteredCC[SENSORS];
uint8_t windowCount[SENSORS];
bool touching[SENSORS];

//Midi instance

void setup(void){     //call to all setup functions and start serial port
  randomSeed(analogRead(0));
  Serial.begin(115200);
  mesh.setNodeID(HEAD);
  MPR121.begin(I2CADDR);
  mesh.begin();
  MPRconfig();
  #ifdef DEBUG
    Serial.print("Sender: ");  Serial.println(mesh.getNodeID());
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
    MPR121.setProxMode(prox_mode);  //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11
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
    //MPR121.setGlobalCDC(1);
}


void debug(){
  #ifdef DEBUG
  Serial.print("channel");Serial.print("\t|");Serial.print("note");Serial.print("\t|");Serial.print("message");Serial.print("\t|");Serial.println("Velocity");
  Serial.print(payload.channel);Serial.print("\t|");Serial.print(payload.note);Serial.print("\t|");Serial.print(payload.message,HEX);Serial.print("\t|");Serial.println(payload.velocity);
  #endif
}


void loop(void){
MPR121.updateAll();
    //delay(10);
    for(uint8_t n=0; n<SENSORS; n++){
      
      int16_t diff = MPR121.getFilteredData(n)-MPR121.getBaselineData(n);
      raw[n] = diff;
      if( diff > maxdiff[n]){maxdiff[n] = diff;}
      if(diff < SATVAL){ diff = SATVAL;}
      else if( diff < mindiff[n]){mindiff[n] = diff;}
      payload.channel = HEAD;
      payload.note = n;
      payload.message = CC;
      payload.velocity = uint8_t(map(diff,maxdiff[n],mindiff[n],0,127));
      
      if (n == 12){payload.velocity = uint8_t(map(diff,MAXPROX,MINPROX,0,127));}

      if (MPR121.isNewTouch(n) && n != 12 ) {     //check if there is a new touch on the electrode
        payload.message = NOTE_ON;
        sendRadio();
        debug();
        touching[n] = true;
      }
      
      else if (MPR121.isNewRelease(n) && n != 12 ) {   //check if the touch has just been lifted
        payload.message = NOTE_OFF;
        sendRadio();
        debug();
        touching[n] = false;
      }
      
      if (n == 12 ){
        if(payload.velocity >=25){
          if(windowCount[n] == WINDOW){
            payload.velocity = filteredCC[n] / WINDOW;
            windowCount[n] = 0;
            filteredCC[n] = 0; 
            sendRadio();
            debug();
          }
          else{
            filteredCC[n] = filteredCC[n] + uint8_t(payload.velocity);
            windowCount[n] += 1;
          }
        }
        touching[n] = false;
      }
      
      if(touching[n] == true){
        if(windowCount[n] == WINDOW){
          payload.velocity = filteredCC[n] / WINDOW;
          windowCount[n] = 0;
          filteredCC[n] = 0; 
          sendRadio();
          debug();
        }
        else{
          filteredCC[n] = filteredCC[n] + uint8_t(payload.velocity);
          windowCount[n] += 1;
        }
      }
    } // end of electrode for loop
}

void sendRadio(){
  if (!mesh.write(&payload, 'M', sizeof(payload))) {
    // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
      //refresh the network address
      Serial.println("Renewing Address");
      mesh.renewAddress();
    } else {
      Serial.println("Send fail, Test OK");
    }
  } else {
    Serial.print("Send OK: ");
  }
}
