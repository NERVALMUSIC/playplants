#include "src/RF24/RF24.h"
#include "src/RF24Network/RF24Network.h"
#include "src/RF24Mesh/RF24Mesh.h"
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

//Midi instance
  uint8_t showcount = 0;      //variable to control show modes
  uint8_t modes[7];
  uint8_t notes[ 7 ][ 8 ];
  uint8_t counter = 0;
  bool asked = false;

void setup(void){     //call to all setup functions and start serial port
  randomSeed(analogRead(0));
  Serial.begin(115200);
  mesh.setNodeID(0);
  mesh.begin();
  #ifdef DEBUG
    Serial.print("Control: ");  Serial.println(mesh.getNodeID());
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
      case 19:
        memcpy(modes, MODES20, sizeof(MODES20));
        memcpy(notes, NOTES20, sizeof(NOTES20));
      break;
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
}

void receiver() {
    switch (modes[payload.channel-1])   //choose mode for the channel received based on last time it was configured
    {
      case 0:
        if( payload.note != 12 && (payload.message == NOTE_ON || payload.message == NOTE_OFF)){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][payload.note], payload.velocity);   //Send notes when touched
        }
      break;
      case 1:
        if( payload.note > 0 && payload.note < 12 && (payload.message == NOTE_ON || payload.message == NOTE_OFF)){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][payload.note], payload.velocity);   //Send notes when touched
        }
        if( payload.note == 0 && payload.message == CC){
          sendMIDI(payload.message, payload.channel, CCNOTE, payload.velocity);   //Send CC based on intensity
        }
      break;
      case 2:
        if( payload.note == 12){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][0], payload.velocity);              //Send CC based on proximity to rock
        }
      break;
      case 3:
        if( payload.note != 12 && (payload.message == NOTE_ON)){   
          if (notes[payload.channel-1][1] == 2)
          {
            if (asked == true){
              sendMIDI(NOTE_ON, payload.channel, notes[payload.channel-1][0]+counter, 127);                         //Respond
              sendMIDI(NOTE_OFF, payload.channel, notes[payload.channel-1][0]+counter, 0);  
              counter += 1;
              asked = false;
            }
          }
          else{
            if (asked == false){
              sendMIDI(NOTE_ON, payload.channel, notes[payload.channel-1][0]+counter, 127);                         //Ask
              sendMIDI(NOTE_OFF, payload.channel, notes[payload.channel-1][0]+counter, 0);
              asked = true;
            }
          }
        }
      break;
      case 4:
        if(payload.note != 12){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][random(0, 11)], payload.velocity);     //Send random values while touching
          counter += 1;
        }
      break;
      default:
      #ifdef DEBUG
        Serial.println("Mode not implemented!!");
      #endif
      break;
    }
  }

void sendMIDI(uint8_t messageType, uint8_t channel, uint8_t data1, uint8_t data2){
    messageType &= 0b11110000;                   // Message type is high nibble
    channel     &= 0b00001111;                   // Channel is low nibble
    uint8_t statusByte = messageType | channel;  // Combine the messageType (high nibble) with the channel (low nibble)
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
