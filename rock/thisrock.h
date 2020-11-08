//Touch this
//

//Defines
#define CONTROL true	// <-- Comentar para emisor
//#define DEBUG //Uncomment for debug

//Sensor Stuff
#ifndef CONTROL
  #define HEAD 4			  // <-- Hardcode value between 1 and 4
  #define TOUCH 20
  #define RELEASE 10
#else
  #define HEAD 10  // <-- Midi channel for control keypad
  #define TOUCH 20
  #define RELEASE 10
#endif

//Midi stuff
const uint8_t NOTES1[ 4 ][ 12 ] = { {50, 53, 57, 60, 62, 65, 69, 72, 74, 77, 81, 84}, {60, 63, 67, 70, 72, 75, 79, 82, 84, 87, 91, 94} , {70, 73, 77, 80, 82, 85, 89, 92, 94, 97, 101, 104}, {60, 63, 67, 70, 72, 75, 79, 82, 84, 87, 91, 94} };
const uint8_t NOTES2[ 4 ][ 12 ] = { {50, 53, 57, 60, 62, 65, 69, 72, 74, 77, 81, 84}, {60, 63, 67, 70, 72, 75, 79, 82, 84, 87, 91, 94} , {70, 73, 77, 80, 82, 85, 89, 92, 94, 97, 101, 104}, {60, 63, 67, 70, 72, 75, 79, 82, 84, 87, 91, 94} };
const uint8_t NOTESCC[ 4 ][ 12 ] = { {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31}, {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31} , {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31}, {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31} };
const uint8_t NOTESPROX[ 4 ] = { 110, 111, 112, 113 };
//<-- notes to play for this rock (set also for receiver in case it needs to be used)

#ifdef CONTROL
const uint8_t MODES[4] = {0, 3, 4, 6};      //<-- Electrodes used for mode control (3 minimum modes)
const uint8_t ROCKS[4] = {1, 2, 5, 10};      //<-- Electrodes used for rock selection (4 maximum rocks)
const uint8_t STOP = 11;       //<-- Electrode used for Control Change commands

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
#define WINDOW 25

//Radio stuff
const uint64_t PIPES[4] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL};
#define PINCE 9
#define PINCS 10
#define BUFF 4
