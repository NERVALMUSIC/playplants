#include "src/nRF5x_lowPower/Arduino_nRF5x_lowPower.h"

#define LOOP_CYCLES     5
#define CYCLE_TIME_MS   1000

volatile int cycles = 0;

void setup() {

    //pinMode(BUTTON_PIN, INPUT);
 
    //nRF5x_lowPower.enableWakeupByInterrupt(BUTTON_PIN, FALLING);
}

void loop() {
  
  if (cycles >= LOOP_CYCLES) {
      nRF5x_lowPower.powerMode(POWER_MODE_OFF);
  }
   
  cycles++;
  delay(CYCLE_TIME_MS);
}
