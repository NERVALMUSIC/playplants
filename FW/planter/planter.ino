#include "src/nRF5x_lowPower/Arduino_nRF5x_lowPower.h"
#include "src/OneButton/OneButton.h"
#include "src/jled/jled.h"
#include "src/AT42QT/AT42QT2120.h"
#include "nrf52.h"
#include "src/BLESerial/BLESerial.h"
#include <BLEPeripheral.h>
#include "thisplanter.h"

void setup()
{  
  //Setup led
  red.Off().LowActive();
  green.Off().LowActive();
  blue.Off().LowActive();
  
  //Setup user button
  but.attachClick(singleclick);
  but.attachDoubleClick(doubleclick);
  but.attachLongPressStop(longPressStop);
  nRF5x_lowPower.enableWakeupByInterrupt(BUTTON_PIN, FALLING);
  nRF5x_lowPower.enableWakeupByInterrupt(CHG, RISING);

  //setup battery manager
  analogReference(AR_VDD4);   // Full adc range (0 - VDD)
  pinMode(POW_EN, OUTPUT);    // sets power enable as Output
  pinMode(CHG, INPUT);        // sets charge detect pin as Input
  attachInterrupt(digitalPinToInterrupt(CHG), charge_change, CHANGE);
  readBattery();
  
  //Setup radio
#ifdef DEBUG
  bleSerial.setLocalName("ATTEC-DEBUG");
  bleSerial.setDeviceName("ATTEC-DEBUG");           //device name sometimes used by central
  bleSerial.begin();
#else
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
#endif

  //Setup Sensor
  pinMode(INT, INPUT);        // sets interrupt pin as Input
  touch_sensor.begin();  
  touch_sensor.reset();
  delay(RESET_DELAY);
  touch_sensor.triggerCalibration();
  delay(CALIBRATION_LOOP_DELAY);
}

void loop()
{
  msOffset = millis();
  but.tick();
  red.Update();
  green.Update();
  blue.Update();
  if(charge_state){
    red.Set(255);
    green.Set(127);
    blue.Set(0);
  }
  else{
    if(!red.IsRunning()){red.Off();}
    if(!green.IsRunning()){green.Off();}
  }
  if(sleep){
    delay(1000);
    nRF5x_lowPower.powerMode(POWER_MODE_OFF);     //Power off if long press has been detected
  }
  Sensormanager();
  BLEmanager();
}

void BLEmanager()
{
#ifdef DEBUG
  bleSerial.poll();
  if (bleSerial)
  {
    if(!charge_state && !sending ){blue.On();}
    //SOLID BLUE
    for (uint8_t key=0; key < touch_sensor.KEY_COUNT; ++key)
    {
      if(touch_sensor.touched(plantstatus,key)){
        bleSerial.println(" Touched");
      }
    }
  }
  else
  {
    if(!blue.IsRunning()){blue.Breathe(SLOW).Repeat(1);}
  }
#else

#endif
}

//Poll the sensor inputs and convert any relevant data to midi format
void Sensormanager()
{
  if (touch_sensor.communicating())
  {
    plantstatus = touch_sensor.getStatus();
  }
}


// This function will be called when the button is pressed once.
void singleclick() {
#ifdef DEBUG
  if (bleSerial) {bleSerial.println("Single click");}
#endif
  green.Breathe(FAST).Repeat(1);
  if(green.IsRunning()){blue.Off();}
  // BLINK GREEN
} // singleclick

// This function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick() {
#ifdef DEBUG
  if (bleSerial) {bleSerial.println("Double click");}
#endif
  readBattery();
  // BLINK GREEN twice
} // doubleclick

// This function will be called once, when the button is released after beeing pressed for a long time.
void longPressStop() {
  sleep = true;
  red.Breathe(SLOW).Repeat(1);
  green.Breathe(SLOW).Repeat(1);
  blue.Breathe(SLOW).Repeat(1);
  while(red.IsRunning())
    {
      red.Update();
      green.Update();
      blue.Update();
    }
} // longPressStop

void readBattery(){
      red.Off();
      green.Off();
      blue.Off();
      red.Update();
      green.Update();
      blue.Update();
  digitalWrite(POW_EN, HIGH); // turn on battery monitor
  delay(10);
  bat_adc = analogRead(BAT_MON);
  digitalWrite(POW_EN, LOW); // turn on battery monitor
  if( bat_adc <= BAT_LOW){
    red.Breathe(SLOW).Repeat(1);
    while(red.IsRunning())
    {
      red.Update();
    }
      // SLOW BLINK RED
  }
  else if( bat_adc <= BAT_HIGH){
    red.Breathe(SLOW).Repeat(1);
    green.Breathe(SLOW).Repeat(1);
    while(red.IsRunning())
    {
      red.Update();
      green.Update();
    }
      // SLOW BLINK ORANGE
  }
  else{
    green.Breathe(SLOW).Repeat(1);
    while(green.IsRunning())
    {
      green.Update();
    }
      // SLOW BLINK GREEN
  }
}

void charge_change(){
  if(digitalRead(CHG) == HIGH){charge_state = true;}
  else{charge_state = false;}
}
