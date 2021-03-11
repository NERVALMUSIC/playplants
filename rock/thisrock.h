//Touch this
//

//Defines
#define CONTROL true	// <-- Comentar para emisor
#define DEBUG //Uncomment for debug

//Sensor Stuff
#ifndef CONTROL
  #define HEAD 1			  // <-- Hardcode value between 1 and 4
  #define TOUCH 20
  #define RELEASE 10
  mpr121_proxmode_type  prox_mode = PROX_0_3; //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11 (no proximidad, 2 sensores, 4 sensores, 12 sensores)
#else
  #define HEAD 10  // <-- Midi channel for control keypad
  #define TOUCH 20
  #define RELEASE 10
  mpr121_proxmode_type  prox_mode = PROX_DISABLED;
#endif

//Midi stuff
const uint8_t NOTES1[ 4 ][ 12 ] = { {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} , {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} };
const uint8_t MODES1[4] = {0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
const uint8_t NOTES2[ 4 ][ 12 ] = { {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} , {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} };
const uint8_t MODES2[4] = {1, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
const uint8_t NOTES3[ 4 ][ 12 ] = { {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} , {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} };
const uint8_t MODES3[4] = {2, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
const uint8_t NOTES4[ 4 ][ 12 ] = { {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} , {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} };
const uint8_t MODES4[4] = {3, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
const uint8_t NOTES5[ 4 ][ 12 ] = { {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} , {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95}, {60, 64, 67, 71, 74, 77, 81, 84, 88, 91, 95, 95} };
const uint8_t MODES5[4] = {4, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random

#ifdef CONTROL
const uint8_t MODENOMIDI = 1;       //<-- Electrode used for MODE change with midi feedback
const uint8_t MODEMIDI = 0;       //<-- Electrode used for MODE change with midi feedback

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
#define WINDOW 25
#define SENSORS 13 // number of sensors + proximity
#define MINPROX -25
#define MAXPROX 6

//Radio stuff
const uint64_t PIPES[4] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL};
#define PINCE 9
#define PINCS 10
#define BUFF 4
