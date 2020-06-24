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
const bool role = true;
const uint8_t head = 0;     //0 to 4 for sendig nodes only
const uint64_t pipes[5] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL, 0xF0F0F0F096LL };

const uint8_t bufferSize = 6;
uint8_t data[bufferSize];

void setup(void) {
   radio.begin();
   MIDI.begin();
   Serial.begin(115200);
   setRF(role);
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
void setRF(bool role){
  if(role == true){
  for(int i = 0; i < 5; i += 1) {
   radio.openReadingPipe(i+1,pipes[i]);
  }
   radio.startListening();
  }
  else{
  radio.openWritingPipe(pipes[head]);
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
