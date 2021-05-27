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
#ifdef CONTROL
  uint8_t showcount = 0;      //variable to control show modes
  uint8_t modes[9];
  uint8_t notes[ 9 ][ 12 ];
  uint8_t counter = 0;
  bool asked = false;
#endif

void setup(void){     //call to all setup functions and start serial port
  randomSeed(analogRead(0));
  Serial.begin(115200);
#ifdef CONTROL
  #ifdef DEBUG
    Serial.print("Control: ");  Serial.println(mesh.getNodeID());
  #endif
  mesh.setNodeID(0);
#else
  mesh.setNodeID(HEAD);
  #ifdef DEBUG
    Serial.print("Sender: ");  Serial.println(mesh.getNodeID());
  #endif
  MPR121.begin(I2CADDR);
  MPRconfig();
#endif
  mesh.begin();
}
void MPRconfig(){
  #ifndef CONTROL
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
    #endif
  }


void debug(){
  #ifdef DEBUG
  Serial.print("channel");Serial.print("\t|");Serial.print("note");Serial.print("\t|");Serial.print("message");Serial.print("\t|");Serial.println("Velocity");
  Serial.print(payload.channel);Serial.print("\t|");Serial.print(payload.note);Serial.print("\t|");Serial.print(payload.message,HEX);Serial.print("\t|");Serial.println(payload.velocity);
  #endif
}


void loop(void){
  mesh.update();
  #ifndef CONTROL   // Transmiter code
    sender();
  #else   //Receiver code
    mesh.DHCP();   
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
      case 5:
        memcpy(modes, MODES6, sizeof(MODES6));
        memcpy(notes, NOTES6, sizeof(NOTES6));
      break;
      case 6:
        memcpy(modes, MODES7, sizeof(MODES7));
        memcpy(notes, NOTES7, sizeof(NOTES7));
      break;
      case 7:
        memcpy(modes, MODES8, sizeof(MODES8));
        memcpy(notes, NOTES8, sizeof(NOTES8));
      break;
      case 8:
        memcpy(modes, MODES9, sizeof(MODES9));
        memcpy(notes, NOTES9, sizeof(NOTES9));
      break;
      case 9:
        memcpy(modes, MODES10, sizeof(MODES10));
        memcpy(notes, NOTES10, sizeof(NOTES10));
      break;
      case 10:
        memcpy(modes, MODES11, sizeof(MODES11));
        memcpy(notes, NOTES11, sizeof(NOTES11));
      break;
      case 11:
        memcpy(modes, MODES12, sizeof(MODES12));
        memcpy(notes, NOTES12, sizeof(NOTES12));
      break;
      case 12:
        memcpy(modes, MODES13, sizeof(MODES13));
        memcpy(notes, NOTES13, sizeof(NOTES13));
      break;
      case 13:
        memcpy(modes, MODES14, sizeof(MODES14));
        memcpy(notes, NOTES14, sizeof(NOTES14));
      break;
      case 14:
        memcpy(modes, MODES15, sizeof(MODES15));
        memcpy(notes, NOTES15, sizeof(NOTES15));
      break;
      case 15:
        memcpy(modes, MODES16, sizeof(MODES16));
        memcpy(notes, NOTES16, sizeof(NOTES16));
      break;
      case 16:
        memcpy(modes, MODES17, sizeof(MODES17));
        memcpy(notes, NOTES17, sizeof(NOTES17));
      break;
      case 17:
        memcpy(modes, MODES18, sizeof(MODES18));
        memcpy(notes, NOTES18, sizeof(NOTES18));
      break;
      case 18:
        memcpy(modes, MODES19, sizeof(MODES19));
        memcpy(notes, NOTES19, sizeof(NOTES19));
      break;
     // case 19:
       // memcpy(modes, MODES20, sizeof(MODES20));
       // memcpy(notes, NOTES20, sizeof(NOTES20));
     // break;
/************************************************/   
//      case #:
//        memcpy(modes, MODES#, sizeof(MODES#));
//        memcpy(notes, NOTES#, sizeof(NOTES#));
//      break;
/************************************************/         
      default:
        #ifdef DEBUG
          Serial.println("End of show stop");
        #endif
      break;
    }
/************************************************/    
/*                    THE END                   */
/************************************************/
    if(network.available()){
      RF24NetworkHeader header;
      network.peek(header);
      network.read(header, &payload, sizeof(payload));
      receiver();
      if (payload.channel == GOCHANNEL){
        if ( payload.note == ELEC_GO && payload.message == NOTE_OFF){
          switch (MEMORIAS[showcount])
            {
              case 1:
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
               sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
              break;
              case 2:
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
               sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
              break;
              case 3:
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
               sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
                sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
              break;
            }
          showcount += 1;
          counter = 0;
          asked = false;
        #ifdef DEBUG
          Serial.println("GO");
        #endif          
        }
        if ( payload.note == ELEC_GO_BACK && payload.message == NOTE_OFF){
          switch (MEMORIAS[showcount-1])
            {
              case 1:
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
                sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
              break;
              case 2:
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
                sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
              break;
              case 3:
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
               sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note]+1, 127);        //Mainstage
                sendMIDI(NOTE_ON, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
             sendMIDI(NOTE_OFF, HEAD, notes[payload.channel-1][payload.note], 127);    //Resolume
              break;
            }
          showcount -= 1;
          counter = 0;
          asked = false;
        #ifdef DEBUG
          Serial.println("GO BACK");
        #endif           
        }
      }
    }
  #endif
}
