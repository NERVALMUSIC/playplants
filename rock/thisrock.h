//Touch this
//
//Communications
#define ROLE 1			//<-- 0 for receiver, 1 for transmitter
#define HEAD 0			//<-- 5 for receiver, 0 to 4 for transmitter
//Thresholds
#define TOUCH 10		//<-- optimize for plant   //36 con tierra
#define RELEASE 0		//<-- optimize for plant


//Do NOT touch this
//
//Sensor stuf
#define I2CADDR 0x5A
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif
//Radio stuff
//#define PIPES {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL}
#define PINCE 9
#define PINCS 10
#define BUFF 5
//Midi stuff
enum {
  NOTE1 = 41,
  NOTE2 = 53,
  NOTE3 = 45,
  NOTE4 = 57,
  NOTE5 = 44,
  NOTE6 = 56,
  NOTE7 = 65,
  NOTE8 = 77,
  NOTE9 = 69,
  NOTE10 = 81,
  NOTE11 = 38,
  NOTE12 = 50,
};

#define INSTRUMENT 0//<-- 0 for piano, 1 always on
#define SUSTAIN 127 //<-- 0 Off, 127 On
