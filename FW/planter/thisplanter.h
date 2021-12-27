//Touch this
//

//MIDI Stuff

const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types


//Sensor Stuff


//Do NOT touch this
//

//Sensor stuf


//Radio stuff
BLEPeripheral blePeripheral;
BLEService service("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
BLECharacteristic characteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3", BLERead | BLEWriteWithoutResponse | BLENotify, 20 );
BLEDescriptor descriptor = BLEDescriptor("2902", 0);
unsigned long msOffset = 0;
#define MAX_MS 0x01FFF //13 bits, 8192 dec

//User Interface
#define RED_LED       	16  	// RGB LED R
#define GRN_LED       	14  	// RGB LED G
#define BLU_LED       	15  	// RGB LED B
RGBLed led(RED_LED, GRN_LED, BLU_LED, RGBLed::COMMON_ANODE);


#define BUTTON_PIN      04  	// User button
OneButton but(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);
//HW stuff
static bool sleep = false;
