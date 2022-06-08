//MIDI Stuff
const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types

//Sensor stuf
#define I2CADDR 0x5A
uint8_t CHANN = 5;
uint8_t TOUCH = 20;
uint8_t RELEASE = 10;
#define SENSORS 12 
#define LOWEST 60
#define CCNOTE 65
int16_t SATVAL = -200;
#define WINDOW 25
mpr121_proxmode_type  prox_mode = PROX_DISABLED; //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11 (no proximidad, 2 sensores, 4 sensores, 12 sensores)
uint8_t notes[SENSORS];
uint8_t CCnotes[SENSORS];
//variables for touch detection
bool newtouch[SENSORS];
bool newrelease[SENSORS];
bool touching[SENSORS];
//variables for velocity data
int16_t raw[SENSORS];
int16_t maxdiff[SENSORS];
int16_t mindiff[SENSORS];
uint8_t velocity[SENSORS];
//variables for filtered data
int16_t filtered_data[SENSORS];
uint8_t windowCount[SENSORS];
uint8_t velocity_filt[SENSORS];


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
int connection_state = 0;    //0: Adversising, 1: Connected, 2: Charging, 3: Charged
#define BAT_LOW       835
#define BAT_HIGH      930

//User Interface
#define SLOW    1000
#define FAST    250
uint8_t sensed = 0;
auto red = JLed(RED_LED).Off().LowActive();
auto green = JLed(GRN_LED).Off().LowActive();
auto blue = JLed(BLU_LED).Off().LowActive();

OneButton but(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);
uint8_t counter = 0;
