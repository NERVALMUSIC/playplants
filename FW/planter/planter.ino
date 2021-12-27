#include "src/nRF5x_lowPower/Arduino_nRF5x_lowPower.h"
#include "src/OneButton/OneButton.h"
#include "src/RGBLed/RGBLed.h"
#include "src/AT42QT/AT42QT.h"
#include "nrf52.h"
#include <BLEPeripheral.h>
#include "thisplanter.h"

void setup()
{
  //Setup user button
  but.attachClick(singleclick);
  but.attachDoubleClick(doubleclick);
  but.attachLongPressStop(longPressStop);
  nRF5x_lowPower.enableWakeupByInterrupt(BUTTON_PIN, FALLING);

  //Setup Sensor

  //Setup radio
  blePeripheral.setLocalName("ATEEC PLANTER");            //local name sometimes used by central
  blePeripheral.setDeviceName("ATEEC PLANTER");           //device name sometimes used by central
  blePeripheral.setAppearance(0x0340);                    //default is 0x0000, Check Bluetooth spec
  blePeripheral.setAdvertisedServiceUuid(service.uuid()); //Set advertiser MIDI UUID
  blePeripheral.addAttribute(service);                    //Add servie
  blePeripheral.addAttribute(characteristic);             //Add characteristics
  blePeripheral.addAttribute(descriptor);                 //Add descriptors
  characteristic.setValue(0);                             //Set initial value¡
  blePeripheral.begin();                                  //Initialize de periferal
  BLECentral central = blePeripheral.central();           //Start Advertising

  
  //Setup led
  led.fadeIn(RGBLed::WHITE, 100, 1000); // Fade in with 5 steps during 100ms
  led.fadeOut(RGBLed::WHITE, 100, 1000); // Fade in with 5 steps during 100ms
}

void loop()
{
  msOffset = millis();
  but.tick();
  if(sleep){
    delay(1000);
    nRF5x_lowPower.powerMode(POWER_MODE_OFF);     //Power off if long press has been detected
  }
  Sensormanager();
  BLEmanager();
}

void BLEmanager()
{

}

//Poll the sensor inputs and convert any relevant data to midi format
void Sensormanager()
{

}

// This function will be called when the button is pressed once.
void singleclick() {
  led.flash(RGBLed::GREEN, 200); // Light on during 400ms and flash with interval (200ms)
} // singleclick


// This function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick() {
  led.flash(RGBLed::GREEN, 200, 200); // Light on during 400ms and flash with interval (200ms)
  led.flash(RGBLed::GREEN, 200, 200); // Light on during 400ms and flash with interval (200ms)
} // doubleclick


// This function will be called once, when the button is released after beeing pressed for a long time.
void longPressStop() {
  sleep = true;
  led.fadeIn(RGBLed::RED, 100, 1000); // Fade in with 5 steps during 100ms
  led.fadeOut(RGBLed::RED, 100, 1000); // Fade in with 5 steps during 100ms
} // longPressStop
