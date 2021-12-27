//Touch this
//
#define DEBUG
//MIDI Stuff

const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types


//Sensor Stuff


//Do NOT touch this
//

//Radio stuff
#ifdef DEBUG
  BLESerial bleSerial;
#else
  BLEPeripheral blePeripheral;
  BLEService service("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
  BLECharacteristic characteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3", BLERead | BLEWriteWithoutResponse | BLENotify, 20 );
  BLEDescriptor descriptor = BLEDescriptor("2902", 0);
#endif

unsigned long msOffset = 0;
#define MAX_MS 0x01FFF //13 bits, 8192 dec

//HW stuff
#define RED_LED         16    // RGB LED R
#define GRN_LED         14    // RGB LED G
#define BLU_LED         15    // RGB LED B
#define CHG             03    // Charge indication
#define BUTTON_PIN      04    // User button
#define POW_EN          05    // Enable battery readings
#define INT             06    // Sensor Interrupt pin
int bat_mon = A0;             // Analog input from battery
#define SDA             00    // SDA for sensor
#define SCL             01    // SCL for sensor

//User Interface
RGBLed led(RED_LED, GRN_LED, BLU_LED, RGBLed::COMMON_ANODE);
OneButton but(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);
static bool sleep = false;


//Sensor stuf
AT42QT2120 touch_sensor;
AT42QT2120::Status plantstatus;
const int RESET_DELAY = 2000;
const int CALIBRATION_LOOP_DELAY = 50;
