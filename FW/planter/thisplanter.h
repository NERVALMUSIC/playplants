
//Uncomment for BLE-SERIAL (comment for BLE-MIDI)
//#define DEBUG

//MIDI Stuff
const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types

//Sensor stuf
AT42QT touch;
#define KEYS        12
uint8_t TOUCH_INT_flag = false;
const uint8_t notes[12] = {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71};
bool newtouch[12];
bool newrelease[12];


//Radio stuff
#ifdef DEBUG
  BLESerial bleSerial;
#else
  BLEPeripheral blePeripheral;
  BLEService service("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
  BLECharacteristic characteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3", BLERead | BLEWriteWithoutResponse | BLENotify, 20 );
  BLEDescriptor descriptor = BLEDescriptor("2902", 0);
  BLECentral central = blePeripheral.central();
#endif

unsigned long msOffset = 0;
#define MAX_MS 0x01FFF //13 bits, 8192 dec


//HW stuff
int bat_adc = 0;
bool charge_state = false;
int connection_state = 0;    //0: Adversising, 1: Connected, 2: Sending
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
static bool sleep = false;
