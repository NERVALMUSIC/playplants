//Touch this
//
//Communications
#define ROLE 0			//<-- 0 for receiver, 1 for transmitter
#define HEAD 5			//<-- 5 for receiver, 0 to 4 for transmitter
//Thresholds
#define TOUCH 30		//<-- optimize for plant
#define RELEASE 5		//<-- optimize for plant


//Do NOT touch this
//
//Sensor stuf
#define I2CADDR 0x05
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif
//Radio stuff
#define PIPES {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL}
#define PINCE 9
#define PINCS 10
#define BUFF 5
//Midi stuff
#define BASENOTE 48
