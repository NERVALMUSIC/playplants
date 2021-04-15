#include "src/RF24/RF24.h"
#include "src/MPR121/MPR121.h"
#include <SPI.h>


#define PINCE 9
#define PINCS 10
#define I2CADDR 0x5A
RF24 radio(PINCE, PINCS);
// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
#define Sender True


#ifdef Sender
  int16_t counter = 0;
  int16_t data[1];
  void setup(void)
  {
     Serial.begin(115200);
     Serial.println(MPR121.begin(I2CADDR));
     radio.begin();
     delay(10);
     radio.setDataRate(RF24_1MBPS);
     radio.openWritingPipe(pipe);
  }
   
  void loop(void)
  {
     MPR121.updateAll();
     data[0] = MPR121.getFilteredData(0);
     //data[0] = counter;
     radio.write(data, sizeof data);
     Serial.println(data[0]);
     counter += 1;
  }
#else
  int16_t data[2];
  void setup(void)
  {
     Serial.begin(115200);
     radio.begin();
      Serial.println(radio.getDataRate());
     radio.setDataRate(RF24_1MBPS);
     radio.openReadingPipe(1,pipe);
     radio.startListening();
      Serial.println(radio.getDataRate());
  }
   
  void loop(void)
  {
     if (radio.available())
     {
      radio.read(data, sizeof data);
      Serial.print("Received: " );
      Serial.println(data[0]);
     }
  }
#endif
