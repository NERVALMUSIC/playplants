#include "src/nRF5x_lowPower/Arduino_nRF5x_lowPower.h"
#include "src/OneButton/OneButton.h"
#include "src/jled/jled.h"
#include "src/AT42QT/AT42QT.h"
#include "nrf52.h"
#include "src/BLESerial/BLESerial.h"
#include <BLEPeripheral.h>
#include "thisplanter.h"

void setup()
{  
  //Setup led
  red.Off();
  green.Off();
  blue.Off();
  red.Update();
  green.Update();
  blue.Update();
  
  //Setup user button
  but.attachClick(singleclick);
  but.attachDoubleClick(doubleclick);
  but.attachLongPressStop(longPressStop);
  nRF5x_lowPower.enableWakeupByInterrupt(BUTTON_PIN, FALLING);
  nRF5x_lowPower.enableWakeupByInterrupt(CHG, FALLING);
  //setup battery manager
  analogReference(AR_VDD4);   // Full adc range (0 - VDD)
  pinMode(POW_EN, OUTPUT);    // sets power enable as Output
  pinMode(CHG, INPUT);        // sets charge detect pin as Input
  attachInterrupt(digitalPinToInterrupt(CHG), charge_change, CHANGE);
  charge_change();
  readBattery();
  
  //Setup radio
  blePeripheral.setLocalName("ATEEC PLANTER");            //local name sometimes used by central
  blePeripheral.setDeviceName("ATEEC PLANTER");           //device name sometimes used by central
  blePeripheral.setAppearance(0x054E);                    //default is 0x0000, Check Bluetooth spec
#ifndef DEBUG  
  blePeripheral.setAdvertisedServiceUuid(service.uuid()); //Set advertiser MIDI UUID
  blePeripheral.addAttribute(service);                    //Add servie
  blePeripheral.addAttribute(characteristic);             //Add characteristics
  blePeripheral.addAttribute(descriptor);                 //Add descriptors
  characteristic.setValue(0);                             //Set initial value
#endif
  blePeripheral.begin();                                  //Initialize de periferal

  //Setup Sensor
  pinMode(INT, INPUT_PULLUP);        // sets interrupt pin as Input
  touch.begin();
  touch.reset();
  delay(100);
  touch.init();
  delay(100);
  attachInterrupt(INT, TOUCH_interrupt, CHANGE);
}

void loop()
{
  but.tick();
  led_update();
  Sensormanager();
  BLEmanager();
}

//Poll the sensor inputs and convert any relevant data to midi format
void Sensormanager(){
  if(TOUCH_INT_flag) {
    for( int k = 0; k < KEYS; k++)
    { 
      touching[k] = touch.getKey(k);
      if((lastouch[k] != touching[k]) && touching[k]){newtouch[k] = true;}
      if((lastouch[k] != touching[k]) && !touching[k]){newrelease[k] = true;}
      lastouch[k] = touching[k];
    }
    TOUCH_INT_flag = false;
  }
}

// This function will be called when a sensor is touched.
void TOUCH_interrupt() { 
  touch.IRQ_handler();
  red.Off(); green.Breathe(FAST).Repeat(1); blue.Off();  // BLINK GREEN
  TOUCH_INT_flag = true; 
}
