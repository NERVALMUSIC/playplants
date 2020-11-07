//Touch this
//

//Defines
#define CONTROL true	// <-- Comentar para emisor
#define DEBUG //Uncomment for debug

//Sensor Stuff
#ifndef CONTROL
  #define HEAD 1			  // <-- Hardcode value between 1 and 4
  #define TOUCH 20   //<-- optimize for PLANT
  #define RELEASE 2   //<-- optimize for PLANT
#else
  #define HEAD 10
  #define TOUCH 20   //<-- optimize for PLANT
  #define RELEASE 10   //<-- optimize for PLANT
#endif

//Midi stuff
const uint8_t NOTES[ 4 ][ 12 ] = { {50, 53, 57, 60, 62, 65, 69, 72, 74, 77, 81, 84}, {60, 63, 67, 70, 72, 75, 79, 82, 84, 87, 91, 94} , {70, 73, 77, 80, 82, 85, 89, 92, 94, 97, 101, 104}, {60, 63, 67, 70, 72, 75, 79, 82, 84, 87, 91, 94} };
const uint8_t NOTES2[ 4 ][ 12 ] = { {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60} , {70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70}, {80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80} };
//<-- notes to play for this rock (set also for receiver in case it needs to be used)

#ifdef CONTROL
const uint8_t MODES[4] {0, 3, 4, 6};      //<-- Electrodes used for mode control (3 needed) up to 6 can be used
const uint8_t ROCKS[4] {1, 2, 5, 7}; //<-- Electrodes used for rock selection (4 maximum rocks)
#endif
const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t KEY_PRESSURE = 0xA0;      //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types
const uint8_t PROGRAM_CHANGE = 0xC0;    //<-- Used Midi message types
const uint8_t CHANNEL_PRESSURE = 0xD0;  //<-- Used Midi message types
const uint8_t PITCH_BEND = 0xE0;        //<-- Used Midi message types

//Do NOT touch this
//

//Sensor stuf
#define I2CADDR 0x5A
#define SENSORS 13
#define WINDOW 100

//Radio stuff
const uint64_t PIPES[4] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL};
#define PINCE 9
#define PINCS 10
#define BUFF 4
