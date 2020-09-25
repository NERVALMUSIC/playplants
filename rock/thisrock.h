//Touch this
//

//Communications
//  #define CONTROL true	//<-- Comment this line for transmitter

#ifndef CONTROL
  #define HEAD 3			  //<-- Hardcode value between 1 and 5
#endif

//Thresholds
#define TOUCH 20		//<-- optimize for plant   //36 con tierra
#define RELEASE 0		//<-- optimize for plant

//Midi stuff
#ifndef CONTROL
  const uint8_t NOTES[12]{41,53,45,57,44,56,65,77,69,81,38,50};
#else
  const uint8_t NOTES[2]{0,1};
#endif
const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;

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
