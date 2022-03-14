#include "AT42QT.h"

/*============================================================================
Name    :  AT42QT
------------------------------------------------------------------------------
Purpose : class constructor
============================================================================*/
AT42QT::AT42QT() :
	wire(&Wire)
{
  
}

/*============================================================================
Name    : begin
------------------------------------------------------------------------------
Purpose : initialize AT42QT
============================================================================*/
uint8_t AT42QT::begin() {
  
  wire->begin();
  
  if(reset_pin != PIN_UNCONNECTED) {
    pinMode(reset_pin, OUTPUT);
    digitalWrite(reset_pin, HIGH);
	// Reset QT device
	reset();
  }
  
  // Check communication is ready and able to read Chip ID
  if(!GetCommsReady()) {
	  return false;
  }
  // Read setup block
  if(!readSetup()) {
	  return false;
  }
  return true;
}

/*============================================================================
Name    :   Init
------------------------------------------------------------------------------
Purpose :   Configure Sensor
============================================================================*/
uint8_t AT42QT::init() {

	return writeSetup();
}

/*============================================================================
Name    :   writeSetup & readSetup
------------------------------------------------------------------------------
Purpose : Write and Readsetup register block 
============================================================================*/
uint8_t AT42QT::writeSetup() {

	return write(QT_SETUPS_BLOCK_ADDR, (uint8_t *)&setup_block, sizeof(setup_block));
}

uint8_t AT42QT::readSetup() {

	return read(QT_SETUPS_BLOCK_ADDR, (uint8_t *)&setup_block, sizeof(setup_block));
}

/*============================================================================
Name    :   readStatus
------------------------------------------------------------------------------
Purpose :  Read status register
============================================================================*/
uint8_t AT42QT::readStatus(void) {

  return read(QT_STATUS_ADDR, QtStatus, sizeof(QtStatus));
}

/*============================================================================
Name    :   getKey
------------------------------------------------------------------------------
Purpose :  Read Address 3 or 4 to get Key value
============================================================================*/
uint8_t AT42QT::getKey(uint8_t num) {
	if(num>15) return 0;
	     if(num < 8)  return bitRead(QtStatus[1], num);
	else if(num < 16) return bitRead(QtStatus[2], num-8);
	else return 0;
}

/*============================================================================
Name    :   calibrate
------------------------------------------------------------------------------
Purpose :   Write Address 6 to perform a calibration
============================================================================*/
uint8_t AT42QT::calibrate() {
	if(writeReg(QT_CALIBRATE, 1)) return true;
	return false;
}

/*============================================================================
Name    :   reset
------------------------------------------------------------------------------
Purpose :   Write Address 7 to perform a SW reset
============================================================================*/
uint8_t AT42QT::reset(void)  {
	if(writeReg(QT_RESET, 1)) return true;
	return false;
}

/*============================================================================
Name    :   setLP
------------------------------------------------------------------------------
Purpose :  Write Address 8 to set power mode
============================================================================*/

uint8_t AT42QT::setLP(uint8_t value) {
	if(writeReg(QT_LP, value)) return true;
	return false;
}

/*============================================================================
Name    :   setTTD
------------------------------------------------------------------------------
Purpose :  Write Address 9 to set Toward Touch Drift
============================================================================*/

uint8_t AT42QT::setTTD(uint8_t value) {
	if(writeReg(QT_TTD, value)) return true;
	return false;
}

/*============================================================================
Name    :   setATD
------------------------------------------------------------------------------
Purpose :  Write Address 10 to set Away Touch Drift
============================================================================*/

uint8_t AT42QT::setATD(uint8_t value) {
	if(writeReg(QT_ATD, value)) return true;
	return false;
}

/*============================================================================
Name    :   setDI
------------------------------------------------------------------------------
Purpose :  Write Address 11 to set 
============================================================================*/

uint8_t AT42QT::setDI(uint8_t value) {
	if(writeReg(QT_DI, value)) return true;
	return false;
}

/*============================================================================
Name    :   setTRD
------------------------------------------------------------------------------
Purpose :  Write Address 12 to set 
============================================================================*/

uint8_t AT42QT::setTRD(uint8_t value) {
	if(writeReg(QT_TRD, value)) return true;
	return false;
}

/*============================================================================
Name    :   setDHT
------------------------------------------------------------------------------
Purpose :  Write Address 13 to set 
============================================================================*/

uint8_t AT42QT::setDHT(uint8_t value) {
	if(writeReg(QT_DHT, value)) return true;
	return false;
}

/*============================================================================
Name    :   setCT
------------------------------------------------------------------------------
Purpose :  Write Address 15 to set 
============================================================================*/

uint8_t AT42QT::setCT(uint8_t value) {
	if(writeReg(QT_CT, value)) return true;
	return false;
}
/*============================================================================
Name    :   setKeyDTHR
------------------------------------------------------------------------------
Purpose :  Write Address 16-27 to set individual key 
============================================================================*/

void AT42QT::setKeyDTHR(uint8_t num, uint8_t value) {
	if(num < 12) num += QT_KEY0_DTHR;
	if(num < QT_KEY0_DTHR || num > QT_KEY11_DTHR) return;
	writeReg(num, value);
}

void AT42QT::setKeyDTHR(uint8_t numFrom, uint8_t numTo, uint8_t value) {
	if(numTo < numFrom) numTo = numFrom;
	for(uint8_t i=numFrom; i<=numTo; i++) setKeyDTHR(i, value);
}

/*============================================================================
Name    :   setKeyPS
------------------------------------------------------------------------------
Purpose :  Write Address 40-51 to set individual key 
============================================================================*/

void AT42QT::setKeyPS(uint8_t num, uint8_t value) {
	if(num < 12) num += QT_KEY0_PULSE_SCALE;
	if(num > QT_KEY11_PULSE_SCALE) return;
	writeReg(num, value);
}

void AT42QT::setKeyPS(uint8_t numFrom, uint8_t numTo, uint8_t value) {
	if(numTo < numFrom) numTo = numFrom;
	for(uint8_t i=numFrom; i<=numTo; i++) setKeyPS(i, value);
}

/*============================================================================
Name    :   getKeySignal
------------------------------------------------------------------------------
Purpose :  Read Address 52-75 to get Key Signal value
============================================================================*/
uint16_t AT42QT::getKeySignal(uint8_t num) {
	
	if(num < 12) num = 2*num + QT_KEY0_SIGNAL;
	if(num > QT_KEY11_SIGNAL) return 0;
	uint16_t variable = readReg(num) << 8 ;
	variable |= readReg(num+1);
	return variable;
}

/*============================================================================
Name    :   getKeyReference
------------------------------------------------------------------------------
Purpose :  Read Address 76-99 to get Key Reference value
============================================================================*/
uint16_t AT42QT::getKeyReference(uint8_t num) {
	
	if(num < 12) num = 2*num + QT_KEY0_REFERENCE;
	if(num > QT_KEY11_REFERENCE) return 0;
	uint16_t variable = readReg(num) << 8 ;
	variable |= readReg(num+1);
	return variable;
}
/*============================================================================
Name    :   IRQ_handler
------------------------------------------------------------------------------
Purpose : External Interrupt from AT42QT
============================================================================*/
void AT42QT::IRQ_handler(void) {
  // read all status-bytes
  static uint32_t lastTouchMillis = 0;
	  readStatus();
}

/*=================================PRIVATE====================================*/

/*============================================================================
Name    :   GetCommsReady
------------------------------------------------------------------------------
Purpose :   Check communication is ready and able to read Chip ID
============================================================================*/
uint8_t AT42QT::GetCommsReady(void) {

	return (readReg(QT_CHIP_ID)==QT_DEVICE_ID);
}

/*============================================================================
Name    :   writeReg & readReg
------------------------------------------------------------------------------
Purpose : Generic Write and read register fucntions
============================================================================*/
uint8_t AT42QT::writeReg(uint8_t regAddr, uint8_t value) {
	if(write(regAddr, &value, sizeof(value))) {
		if((regAddr >= QT_SETUPS_BLOCK_ADDR) && (regAddr < QT_SIGNAL_ADDR)) {
			*((uint8_t*)(&setup_block)-QT_SETUPS_BLOCK_ADDR+regAddr) = value;
		}
		return true;
	}
	return false;
}

uint8_t AT42QT::readReg(uint8_t regAddr) {
	uint8_t value = 0;
	if(read(regAddr, &value, sizeof(value))) {
		if((regAddr >= QT_SETUPS_BLOCK_ADDR) && (regAddr < QT_SIGNAL_ADDR)) {
			*((uint8_t*)(&setup_block)-QT_SETUPS_BLOCK_ADDR+regAddr) = value;
		}
	}
	return value;
}

/*============================================================================
Name    :   write
------------------------------------------------------------------------------
Purpose : Write register block
============================================================================*/

uint8_t AT42QT::write(uint8_t regAddr, uint8_t* dataPtr, uint8_t dataSize) {
	uint8_t error = 0;
    uint8_t counts = 0;
    uint8_t pointer = 0;
    uint8_t counter = 0;
    uint8_t bytesPerWrite = 30;
	if(!dataSize) {
		wire->beginTransmission(addr);
	    wire->write(regAddr);
	    error = wire->endTransmission();
		delay(1);
		return !error;
	}
    while(counts<dataSize) {
	    counts += bytesPerWrite;
		if(counts > dataSize) bytesPerWrite = dataSize - (bytesPerWrite*counter);
	    wire->beginTransmission(addr);
	    wire->write(regAddr + pointer);
	    wire->write(&dataPtr[pointer], bytesPerWrite);
	    error = wire->endTransmission();
		delay(1);
	    if(error) return false;
	    pointer += bytesPerWrite;
	    counter++;
	}
	return !error;
}

/*============================================================================
Name    :   read
------------------------------------------------------------------------------
Purpose : Read register block
============================================================================*/

uint8_t AT42QT::read(uint8_t regAddr, uint8_t* dataPtr, uint8_t dataSize) {
	uint8_t error = 0;
    uint8_t counts = 0;
    uint8_t pointer = 0;
    uint8_t counter = 0;
    uint8_t result_lenght = 0;
    uint8_t bytesPerRead = 30;
    while(counts<dataSize) {
	    counts += bytesPerRead;
		if(counts > dataSize) bytesPerRead = dataSize - (bytesPerRead*counter);
		if(!write(regAddr + pointer)) return false;
		wire->requestFrom(addr, bytesPerRead);
		while(wire->available()) {
		  dataPtr[result_lenght++] = wire->read();
		}
	    pointer += bytesPerRead;
	    counter++;
	}
    return result_lenght;
}
