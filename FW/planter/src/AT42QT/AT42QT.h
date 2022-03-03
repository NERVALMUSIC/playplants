#ifndef _AT42QT_H_
#define _AT42QT_H_

#include <Arduino.h>

#include <Wire.h>

#include "QT2120.h"

#ifdef DEBUG
	#define QT_DEBUG bleSerial
	#define DBG "[DBG] "
#endif

/*============================================================================
Macros
============================================================================*/

#define PIN_UNCONNECTED        255

class AT42QT {
  
  private: //-------------------------------------------------------

  uint8_t GetCommsReady(void);
  
  uint8_t writeReg(uint8_t regAddr, uint8_t value);
  uint8_t readReg(uint8_t regAddr);  
  
  uint8_t write(uint8_t regAddr, uint8_t* dataPtr = 0, uint8_t dataSize = 0);
  uint8_t read(uint8_t regAddr, uint8_t* dataPtr, uint8_t dataSize);
  
#ifdef QT_DEBUG
  void print(const char* str, uint8_t* valuePtr=0, uint8_t size=0);
  void print(const char* str, uint8_t value);
#endif

  uint8_t QtStatus[QT_STATUS_SIZE]; // application storage for QT device-status
  
  SetupBlock setup_block; // Setup block structure

  uint8_t addr = QT_I2C_ADDRESS;
  uint8_t reset_pin = PIN_UNCONNECTED;

  TwoWire* wire;

  public: //--------------------------------------------------------
  
  AT42QT();
  
  uint8_t begin();
  
  uint8_t init();
  
  //Address 2  
  uint8_t readStatus(void);
  //Address 3-4 
  uint8_t getKey(uint8_t num);
  //Address 6   
  uint8_t calibrate();
  //Address 7  
  uint8_t reset();
  
  //SetupBlock
  //Address 8  
  uint8_t    setLP(uint8_t value);		//default 1 
  //Address 9-10  
  uint8_t    setTTD(uint8_t value); //default 20 (3.2s)
  uint8_t    setATD(uint8_t value); //default 5 (0.8s) 
  //Address 11
  uint8_t    setDI(uint8_t value); //default 4 (Max = 32) 
  //Address 12
  uint8_t    setTRD(uint8_t value); //default 255 (0.16s * 255)
  //Address 13
  uint8_t    setDHT(uint8_t value); //default 25 (0.16s * 25)
  //Address 15
  uint8_t    setCT(uint8_t value); //default 0 (1us * 0)
  //Address 16-27  
  void    setKeyDTHR(uint8_t num, uint8_t value);  
  void    setKeyDTHR(uint8_t numFrom, uint8_t numTo, uint8_t value);
  //Address 40-51  
  void    setKeyPS(uint8_t num, uint8_t value);  
  void    setKeyPS(uint8_t numFrom, uint8_t numTo, uint8_t value);
// end of setup block
  
  //Address 52-75 
  uint16_t getKeySignal(uint8_t num);  
  //Address 76-99 
  uint16_t getKeyReference(uint8_t num); 
  
  uint8_t writeSetup();
  uint8_t readSetup();

  void    IRQ_handler(void);

#ifdef QT_DEBUG
  void    printSetup();
  void    printStatus();
#endif

};

#endif // _AT42QT_H_

