//Touch this
//

//Communications
//  #define CONTROL true	// <-- Comentar para emisor

#ifndef CONTROL
  #define HEAD 3			  // <-- Hardcode value between 1 and 5
#endif

//Thresholds
#define TOUCH 15		//<-- optimize for plant
#define RELEASE 1		//<-- optimize for plant

//Midi stuff
const uint8_t NOTES[12]{41,53,45,57,44,56,65,77,69,81,38,50};   //<-- notes to play for this rock (set also for receiver in case it needs to be used)
#ifdef CONTROL
  const uint8_t MODES[3]{1,3,5};        //<-- Electrodes used for mode control (3 needed) up to 6 can be used
  const uint8_t ROCKS[5]{7,8,9,10,11};  //<-- Electrodes used for rock selection (5 maximum rocks)
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
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

//Radio stuff
const uint64_t PIPES[5] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL};
#define PINCE 9
#define PINCS 10
#define BUFF 4
