
//Uncomment for BLE-SERIAL (comment for BLE-MIDI)
//#define DEBUG

//MIDI Stuff
const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types

//Sensor stuf
#define I2CADDR 0x5A
#define CHANN 5
#define TOUCH 20
#define RELEASE 15
#define SENSORS 12 
mpr121_proxmode_type  prox_mode = PROX_DISABLED; //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11 (no proximidad, 2 sensores, 4 sensores, 12 sensores)
const uint8_t notes[12] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61};
bool newtouch[12] = {false, false, false, false, false, false, false, false, false, false, false, false};
bool newrelease[12] = {false, false, false, false, false, false, false, false, false, false, false, false};



//Radio stuff
#ifdef DEBUG
  BLESerial blePeripheral;
#else
  BLEPeripheral blePeripheral;
  BLEService service("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
  BLECharacteristic characteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3", BLERead | BLEWriteWithoutResponse | BLENotify, 20 );
  BLEDescriptor descriptor = BLEDescriptor("2902", 0);
#endif

unsigned long msOffset = 0;
#define MAX_MS 0x01FFF //13 bits, 8192 dec


//HW stuff
int bat_adc = 0;
bool charge_state = false;
int connection_state = 0;    //0: Adversising, 1: Connected, 2: Charging
#define BAT_LOW       835
#define BAT_HIGH      930

//User Interface
#define SLOW    2000
#define FAST    250
auto red = JLed(RED_LED).Off().LowActive();
auto green = JLed(GRN_LED).Off().LowActive();
auto blue = JLed(BLU_LED).Off().LowActive();

OneButton but(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);
