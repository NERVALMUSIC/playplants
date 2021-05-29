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
int16_t raw[SENSORS+1];
int16_t filteredCC[SENSORS+1];
uint8_t windowCount[SENSORS+1];
bool touching[SENSORS];
bool touched[SENSORS];
unsigned long lastDebounceTime[SENSORS];  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


//Midi instance

void setup(void){     //call to all setup functions and start serial port
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
    MPR121.setNumEnabledElectrodes(SENSORS);
    MPR121.setTouchThreshold(TOUCH);      //Set Thresholds
    MPR121.setReleaseThreshold(RELEASE);  //Set Thresholds
    MPR121.setProxMode(prox_mode);  //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11
  /*
  * Optional settings, test best configuration for each plant,
  * see datasheet and application notes to better understand how
  * to change these settings.
  */
    delay(10);
    MPR121.autoSetElectrodes(false);  // autoset all electrode settings with fixed charge time disabled
    delay(1000);
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
      bool touch = MPR121.getTouchData(n);
      int16_t diff = MPR121.getBaselineData(n) - MPR121.getFilteredData(n) ;
      payload.channel = HEAD;
      payload.note = n;
      if (twotouch()){raw[n] = constrain(diff * TOUCHMULT,MIN,MAX);}
      else{raw[n] = constrain(diff,MIN,MAX);}
      payload.velocity = uint8_t(map(raw[n],MIN,MAX,0,127));
      
      if (touch != touched[n]) {
        // reset the debouncing timer
        lastDebounceTime[n] = millis();
      }      
      if ((millis() - lastDebounceTime[n]) > debounceDelay) {
        if (touch != touching[n]) {
          touching[n] = touch;
          // only toggle the LED if the new button state is HIGH
          if (touching[n] == true) {
          payload.message = NOTE_ON;
          }
          else{
          payload.message = NOTE_OFF;
          }
          //sendRadio();
          debug();
        }
      }
      touched[n] = touch;    
      if(touching[n] == true){
        if(windowCount[n] == WINDOW){
          payload.message = CC;
          payload.velocity = filteredCC[n] / WINDOW;
          windowCount[n] = 0;
          filteredCC[n] = 0;
          //sendRadio();
        }
        else{
          filteredCC[n] = filteredCC[n] + uint8_t(payload.velocity);
          windowCount[n] += 1;
        }
      }
    } // end of electrode for loop
    if (prox_mode != PROX_DISABLED ){
      int16_t diff = MPR121.getBaselineData(12) - MPR121.getFilteredData(12) ;
      if (twotouch()){raw[SENSORS+1] = constrain(diff * TOUCHMULT,MIN,MAX);}
      else{raw[SENSORS+1] = constrain(diff,MIN,MAX);}
      payload.velocity = uint8_t(map(raw[SENSORS+1],MIN,MAX,0,127));
      if(diff >= PROXMIN){
        if(windowCount[SENSORS+1] == WINDOW){
          payload.channel = HEAD;
          payload.message = CC;
          payload.velocity = filteredCC[SENSORS+1] / WINDOW;
          windowCount[SENSORS+1] = 0;
          filteredCC[SENSORS+1] = 0; 
          sendRadio();
          
          }
          else{
            filteredCC[SENSORS+1] = filteredCC[SENSORS+1] + uint8_t(diff);
            windowCount[SENSORS+1] += 1;
          }
        }
      }
}

void sendRadio(){
  if (!mesh.write(&payload, 'M', sizeof(payload))) {
    // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
      //refresh the network address
      Serial.println("Renewing Address");
      mesh.renewAddress();
    } 
    else {
      Serial.println("Send fail, Test OK");
    }
  } 
  else {
    debug();
  }
}

bool twotouch(){
  uint8_t numtouch = 0;
  for (uint8_t i = 0; i < SENSORS; i++){
    if (true == touching[i]){
      numtouch += 1;
      if (numtouch > 1){
        return true;
      }
    }
  }
  return false;
}
