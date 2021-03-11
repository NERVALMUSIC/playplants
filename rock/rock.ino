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
int16_t raw[SENSORS];
int16_t maxdiff[SENSORS];
int16_t mindiff[SENSORS];
int16_t filteredCC[SENSORS];
uint8_t windowCount[SENSORS];
bool touching[SENSORS];

//Midi instance
#ifdef CONTROL
  uint8_t showcount = 0;      //variable to control show modes
  uint8_t modes[4];
  uint8_t notes[ 4 ][ 12 ];
  uint8_t counter = 0;
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
  randomSeed(analogRead(0));
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

}

void NRFconfig(void){   //Radio stuff don't look
  radio.setChannel(120);
  radio.setPALevel(RF24_PA_MAX); //RF24_PA_MIN RF24_PA_LOW RF24_PA_HIGH RF24_PA_MAX
  radio.setDataRate(RF24_250KBPS);  //RF24_250KBPS RF24_1MBPS RF24_2MBPS
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
  Serial.print("channel");Serial.print("\t|");Serial.print("note");Serial.print("\t|");Serial.print("message");Serial.print("\t|");Serial.println("Velocity");
  Serial.print(data[0]);Serial.print("\t|");Serial.print(data[1]);Serial.print("\t|");Serial.print(data[2],HEX);Serial.print("\t|");Serial.println(data[3]);
  #endif
}


void loop(void){
  #ifndef CONTROL   // Transmiter code
    sendRadio();
  #else   //Receiver code
    MPR121.updateTouchData();
    if ( MPR121.isNewTouch(MODENOMIDI) || MPR121.isNewTouch(MODEMIDI)){
      if (MPR121.isNewTouch(MODEMIDI)){sendMIDI(CC, HEAD, 60, 127);}
      showcount += 1;
      counter = 0;
    }
/************************************************/                                                    
/*   SHOW Must go on !                          */                               
/*   Aqui se meten todas las funciones que hagan*/ 
/*   falta para que el show avance siguiendo    */
/*   el formato de la presente, cuando el SHOW  */
/*   acabe las plantas se callan                */
/************************************************/
     switch (showcount)
    {
      case 0:
        memcpy(modes, MODES1, sizeof(MODES1));
        memcpy(notes, NOTES1, sizeof(NOTES1));
      break;
      case 1:
        memcpy(modes, MODES2, sizeof(MODES2));
        memcpy(notes, NOTES2, sizeof(NOTES2));
      break;
      case 2:
        memcpy(modes, MODES3, sizeof(MODES3));
        memcpy(notes, NOTES3, sizeof(NOTES3));
      break;
      case 3:
        memcpy(modes, MODES4, sizeof(MODES4));
        memcpy(notes, NOTES4, sizeof(NOTES4));
      break;
      case 4:
        memcpy(modes, MODES5, sizeof(MODES5));
        memcpy(notes, NOTES5, sizeof(NOTES5));
      break;
/************************************************/   
//      case #:
//        memcpy(modes, MODES#, sizeof(MODES#));
//        memcpy(notes, NOTES#, sizeof(NOTES#));
//      break;
/************************************************/         
      default:
        while(1);
      break;
    }
/************************************************/    
/*                    THE END                   */
/************************************************/
    if (radio.available()){
      radio.read(data, sizeof data);    //load buffer
      playModes();
    }
  #endif     //Store last value to compare and get new touches and releases
}
