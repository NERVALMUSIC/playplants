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
   
  uint8_t calibrate();
  
  uint8_t reset();
  
  uint8_t writeReg(uint8_t regAddr, uint8_t value);
  uint8_t readReg(uint8_t regAddr);
  
  uint8_t writeSetup();
  uint8_t readSetup();
  
  uint8_t readStatus(void);

  void    IRQ_handler(void);

#ifdef QT_DEBUG
  void    printSetup();
  void    printStatus();
#endif
  
  void    setKeyDTHR(uint8_t num, uint8_t value);  
  void    setKeyDTHR(uint8_t numFrom, uint8_t numTo, uint8_t value);
  

  
  uint8_t getKey(uint8_t num);
  uint16_t getKeyMask(void);

};

#endif // _AT42QT_H_

