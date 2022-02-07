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
Name    :   calibrate
------------------------------------------------------------------------------
Purpose :   Performs a calibration of the QT device
============================================================================*/
uint8_t AT42QT::calibrate() {
	if(writeReg(QT_CALIBRATE, 1)) return true;
	return false;
}

/*============================================================================
Name    :   reset
------------------------------------------------------------------------------
Purpose :   Performs a software reset of the QT device
============================================================================*/
uint8_t AT42QT::reset(void)  {
	if(writeReg(QT_RESET, 1)) return true;
	return false;
}

/*============================================================================
Name    :   writeReg & readReg
------------------------------------------------------------------------------
Purpose : Write register 
============================================================================*/
uint8_t AT42QT::writeReg(uint8_t regAddr, uint8_t value) {
	if(write(regAddr, &value, sizeof(value))) {
		if(regAddr >= QT_SETUPS_BLOCK_ADDR) {
			*((uint8_t*)(&setup_block)-QT_SETUPS_BLOCK_ADDR+regAddr) = value;
		}
		return true;
	}
	return false;
}

uint8_t AT42QT::readReg(uint8_t regAddr) {
	uint8_t value = 0;
	if(read(regAddr, &value, sizeof(value))) {
		if(regAddr >= QT_SETUPS_BLOCK_ADDR) {
			*((uint8_t*)(&setup_block)-QT_SETUPS_BLOCK_ADDR+regAddr) = value;
		}
	}
	return value;
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
Name    :   IRQ_handler
------------------------------------------------------------------------------
Purpose : External Interrupt from AT42QT
============================================================================*/
void AT42QT::IRQ_handler(void) {
  // read all status-bytes
  static uint32_t lastTouchMillis = 0;
	  readStatus();
}

/*============================================================================
Name    :   printSetup & printStatus
------------------------------------------------------------------------------
Purpose :  Print Setup and Status functions
============================================================================*/

#ifdef QT_DEBUG
void AT42QT::printSetup() {
    print("QT setup block info:");
	
		print("LP_Mode 	   ", setup_block.LP_Mode);
		print("TTD  	     ", setup_block.TTD);
		print("ATD 		 	   ", setup_block.ATD);
		print("DI 	  	   ", setup_block.DI);
		print("TRD         ", setup_block.TRD);
		print("DHT         ", setup_block.DHT);
		print("reserved		 ", setup_block.slider_reserved);
		print("wheel_en 	 ", setup_block.slider_wheel_en);
		print("Slider_en   ", setup_block.slider_en);
		print("charge_time ", setup_block.charge_time);
		
		print("key0_DTHR	 ", &setup_block.key0_DTHR,  12);
	
			
	    print("QT setup block end");
}

void AT42QT::printStatus() {
    print("QT status info:");
		print("QT_GENERAL_STATUS ", QtStatus[0]);
		print("QT_KEY_STATUS_1   ", QtStatus[1]);
		print("QT_KEY_STATUS_2   ", QtStatus[2]);
		print("QT_SLIDER_POSITION", QtStatus[3]);
		print("QT_GPIO_READ      ", QtStatus[4]);
    print("QT status end");
}
#endif 

/*============================================================================
Name    :   setKey<something>
------------------------------------------------------------------------------
Purpose :  Change individual key setup
============================================================================*/

void AT42QT::setKeyDTHR(uint8_t num, uint8_t value) {
	if(num < 12) num += QT_KEY0_DTHR;
	if(num < QT_KEY0_DTHR || num > QT_KEY11_DTHR) return;
	*((uint8_t*)(&setup_block)-QT_SETUPS_BLOCK_ADDR+num) = constrain(value, 0, 255);
}

void AT42QT::setKeyDTHR(uint8_t numFrom, uint8_t numTo, uint8_t value) {
	if(numTo < numFrom) numTo = numFrom;
	for(uint8_t i=numFrom; i<=numTo; i++) setKeyDTHR(i, value);
}



/*============================================================================
Name    :   getKey
------------------------------------------------------------------------------
Purpose :  get Key value
============================================================================*/
uint8_t AT42QT::getKey(uint8_t num) {
	if(num>15) return 0;
	     if(num < 8)  return bitRead(QtStatus[1], num);
	else if(num < 16) return bitRead(QtStatus[2], num-8);
	else return 0;
}

/*============================================================================
Name    :   getKeyMask
------------------------------------------------------------------------------
Purpose :  get Key Mask
============================================================================*/
uint16_t AT42QT::getKeyMask() {
	uint16_t value = QtStatus[2];
	value <<= 8;
	value |= QtStatus[1];
	return value;
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

/*============================================================================
Name    :   print
------------------------------------------------------------------------------
Purpose :  Print via QT_DEBUG
============================================================================*/

#ifdef QT_DEBUG
void AT42QT::print(const char* str, uint8_t* valuePtr, uint8_t size) {
	QT_DEBUG.print(DBG); QT_DEBUG.print(str);
	if(size) QT_DEBUG.print(" :");
	for(uint8_t i=0; i<size; i++) {
		QT_DEBUG.print(" ");
		QT_DEBUG.print(*(valuePtr + i));
	} QT_DEBUG.println();
}

void AT42QT::print(const char* str, uint8_t value)  {

	print(str, &value, 1);
}
#endif