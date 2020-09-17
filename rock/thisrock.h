//Touch this
//

//Communications
//#define CONTROL true	//<-- Comment this line for transmitter

#ifndef CONTROL
  #define HEAD 0			  //<-- Hardcode value between 0 and 4
#endif

//Thresholds
#define TOUCH 10		//<-- optimize for plant   //36 con tierra
#define RELEASE 0		//<-- optimize for plant

//Midi stuff
#ifndef CONTROL
  const int NOTES[12]{41,53,45,57,44,56,65,77,69,81,38,50};
  #define SUSTAIN 127 //<-- 0 Off, 127 On
#else
  const int NOTES[12]{0,1,2,3,4,5,6,7,8,9,10,11};
#endif

//Do NOT touch this
//

//Sensor stuf
#define I2CADDR 0x5A
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

//Radio stuff
const int PIPES[5] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL};
#define PINCE 9
#define PINCS 10
#define BUFF 5
