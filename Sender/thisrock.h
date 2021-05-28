//Touch this
//
//#define DEBUG //Uncomment for debug

//Sensor Stuff
#define HEAD 2			  // <-- Hardcode value between 1 and 9 
#define TOUCH 12
#define RELEASE 4
mpr121_proxmode_type  prox_mode = PROX_DISABLED; //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11 (no proximidad, 2 sensores, 4 sensores, 12 sensores)

const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types

//Do NOT touch this
//

//Sensor stuf
#define I2CADDR 0x5A
#define WINDOW 25
#define SENSORS 13 // number of sensors + proximity
#define MINPROX -25
#define MAXPROX 6
#define SATVAL -100

//Radio stuff
#define PINCE 9
#define PINCS 10
