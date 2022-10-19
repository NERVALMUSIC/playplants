#include "src/nRF5x_lowPower/Arduino_nRF5x_lowPower.h"
#include "src/OneButton/OneButton.h"
#include "src/jled/jled.h"
#include "nrf52.h"
#include "src/BLESerial/BLESerial.h"
#include <BLEPeripheral.h>
#include "src/MPR121/MPR121.h"
#include "thisplanter.h"

void setup()
{  
  //Memory setup
/*
  if (!bond.hasData()){
    for (int i = 0; i < SENSORS; i++) {
      params[i] = notes[i];
    }
    params[12] = CHANN;
    params[13] = TOUCH;
    params[14] = RELEASE;
    params[15] = SATVAL;
    params[16] = WINDOW;
    bond.putData(params,0,STORED);
  }
  bond.getData(params,0,STORED);    
  for (int i = 0; i < SENSORS; i++) {
    notes[i] = params[i];
  }
  CHANN = params[12];
  TOUCH = params[13];
  RELEASE = params[14];
  SATVAL = params[15];
  WINDOW = params[16];
*/
  
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
  nRF5x_lowPower.enableWakeupByInterrupt(USB, RISING);
  nRF5x_lowPower.enableWakeupByInterrupt(BUTTON_PIN, FALLING);
  //setup battery manager
  analogReference(AR_VDD4);   // Full adc range (0 - VDD)
  pinMode(POW_EN, OUTPUT);    // sets power enable as Output
  pinMode(USB, INPUT);        // sets charge detect pin as Input
  pinMode(CHG, INPUT);        // sets charge detect pin as Input
  attachInterrupt(digitalPinToInterrupt(USB), charge_change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHG), charge_change, CHANGE);
  charge_change();
  readBattery();
  
  //Setup radio
  char BLEname[] = "PLANTER   ";
  BLEname[8] = String(CHANN)[0];
  if (CHANN > 9){
    BLEname[9] = String(CHANN)[1];
  }
  blePeripheral.setLocalName(BLEname);            //local name sometimes used by central
  blePeripheral.setDeviceName(BLEname);           //device name sometimes used by central
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
  if(!MPR121.begin(I2CADDR, TOUCH, RELEASE, INT)){
    power_off();
  }
  MPRconfig();
}

void loop()
{
  but.tick();
  led_update();
  Sensormanager();
  BLEmanager();
}
