#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <MIDI.h>
//recieve de DAVID
MIDI_CREATE_DEFAULT_INSTANCE();

const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);
 
const uint64_t talking_pipes[5] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL };

const uint8_t bufferSize = 6;
uint8_t data[bufferSize];

const uint8_t notes[12] = {59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48};

void setup(void) {
   MIDI.begin();
   Serial.begin(115200);
   radio.begin();
   radio.openReadingPipe(1,talking_pipes[0]);
   radio.openReadingPipe(2,talking_pipes[1]);
   radio.openReadingPipe(3,talking_pipes[2]);
   radio.openReadingPipe(4,talking_pipes[3]);
   radio.openReadingPipe(5,talking_pipes[4]);
   radio.startListening();
}

void show(uint8_t data[]) {
  Serial.print("[0]=");
  Serial.println(data[0]);
  Serial.print("[1]=");
  Serial.println(data[1]);
  Serial.print("[2]=");
  Serial.println(data[2]);
  Serial.print("[3]=");
  Serial.println(data[3]);
  Serial.println("---");
}

void play(uint8_t data[]) {
  const uint8_t track = data[0] + 1;
  const uint8_t note = data[1];
  bool pressed = data[2];
  const uint8_t velocity = data[3];
  if (pressed) {
    MIDI.sendNoteOn(note, velocity, track);
    delay(10);
  } else {
    MIDI.sendNoteOff(note, 0, track);
  }
}
 
void loop(void) {
   if (radio.available()) {
      radio.read(data, sizeof data);
        for (int i = 0; i < bufferSize; i += 1) {
          //show(data);
           play(data);
      }
   }
}
