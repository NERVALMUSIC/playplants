#include "src/nRF5x_lowPower/Arduino_nRF5x_lowPower.h"
#include "src/EasyButton/EasyButton.h"
#include "src/RGBLed/RGBLed.h"
#include "nrf52.h"
#include <BLEPeripheral.h>


RGBLed led(16, 14, 15, RGBLed::COMMON_ANODE);
#define BUTTON_PIN       04   // User button 

unsigned long msOffset = 0;
#define MAX_MS 0x01FFF //13 bits, 8192 dec


//Create Button instance
int debounce = 50;
bool pullup = true;
bool invert = true;
int duration = 2000;

EasyButton powerButton(BUTTON_PIN, debounce, pullup, invert);

void LongPush() {
  led.off();
  nRF5x_lowPower.powerMode(POWER_MODE_OFF);
}

void buttonISR()
{
  //When button is being used through external interrupts, parameter INTERRUPT must be passed to read() function
  powerButton.read(); 
}

// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral;
BLEService service("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
BLECharacteristic characteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3", BLERead | BLEWriteWithoutResponse | BLENotify, 20 );
BLEDescriptor descriptor = BLEDescriptor("2902", 0);


void setup() {
    //Setup diag leds
    led.setColor(RGBLed::RED);
    delay(1000);
    led.off();
    //Setup user button
    powerButton.begin();
    powerButton.onPressedFor(duration, LongPush);
    nRF5x_lowPower.enableWakeupByInterrupt(BUTTON_PIN, RISING);
    
    setupBLE();


}

void loop()
{
  powerButton.read();
  led.flash(RGBLed::BLUE, 100);
  BLECentral central = blePeripheral.central();
  if (central) {
      led.setColor(RGBLed::BLUE);
      //Prep the timestamp
      msOffset = millis();
      // central connected to peripheral
      if (central.connected()) {
        //Check if data exists coming in from BLE
        if (characteristic.written()) {
        }
        parseMIDIonDIN();
      }
      else{
        //No longer connected.  Turn off the LEDs.
        delay(100);       
      }
    }
}

//This function is called to check if MIDI data has come in through the serial port.  If found, it builds a characteristic buffer and sends it over BLE.
void parseMIDIonDIN()
{
    uint8_t msgBuf[5]; //Outgoing buffer

    //Calculate timestamp.
    unsigned long currentMillis = millis();
    if(currentMillis < 5000){
        if(msOffset > 5000){
            //it's been 49 days! millis rolled.
            while(msOffset > 5000){
                //roll msOffset - this should preserve current ~8 second count.
                msOffset += MAX_MS;
            }
        }
    }
    //if the offset is more than 2^13 ms away, move it up in 2^13 ms intervals
    while(currentMillis >= (unsigned long)(msOffset + MAX_MS)){
        msOffset += MAX_MS;
    }
    unsigned long currentTimeStamp = currentMillis - msOffset;
    msgBuf[0] = ((currentTimeStamp >> 7) & 0x3F) | 0x80; //6 bits plus MSB
    msgBuf[1] = (currentTimeStamp & 0x7F) | 0x80; //7 bits plus MSB

    //Check MIDI object for new data.
    bool touch = true;
    int data1 = 0;
    int data2 = 0;
    int statusByte = 12;
    if (touch)
    {
      switch (statusByte)
      {
            //2 Byte Channel Messages
            case 0x80 :
            case 0x90 :
            //case midi::AfterTouchPoly :
            case 0xB0 :
            //case midi::PitchBend :
                msgBuf[2] = statusByte;
                msgBuf[3] = data1;
                msgBuf[4] = data2;
                characteristic.setValue(msgBuf, 5);
                break;
            //1 Byte Channel Messages
            //case midi::ProgramChange :
            //case midi::AfterTouchChannel :
            //    msgBuf[2] = statusByte;
            //    msgBuf[3] = MIDI.getData1();
            //    characteristic.setValue(msgBuf, 4);
            //    break;
            default:
                break;
        }
    }

}

void setupBLE()
{
    blePeripheral.setLocalName("ATEEC PLANTER"); //local name sometimes used by central
    blePeripheral.setDeviceName("ATEEC PLANTER"); //device name sometimes used by central
    blePeripheral.setAppearance(0x0340); //default is 0x0000, what should this be?
    blePeripheral.setAdvertisedServiceUuid(service.uuid()); //Advertise MIDI UUID

    // add attributes (services, characteristics, descriptors) to peripheral
    blePeripheral.addAttribute(service);
    blePeripheral.addAttribute(characteristic);
    blePeripheral.addAttribute(descriptor);

    // set initial value
    characteristic.setValue(0);

    blePeripheral.begin();
}
