void BLEmanager()
{
#ifdef DEBUG
  bleSerial.poll();
  if (bleSerial){if(connection_state == 0){connection_state = 1;}
    for( int k = 0; k < KEYS; k++){
      if(newtouch[k]){
        bleSerial.print(k);bleSerial.print("Touched |");
        newtouch[k] = false;
      }
      if(newrelease[k]){
        bleSerial.print(k);bleSerial.print("Released |");
        newrelease[k] = false;
      }
    }
  }
  else{if(connection_state == 1){connection_state = 0;}}
#else
  blePeripheral.poll();
  if (blePeripheral.central) {
    if(connection_state == 0){connection_state = 1;}
    //Prep the timestamp
    msOffset = millis();
    //Check if data exists coming in from the serial port
    for( int k = 0; k < KEYS; k++){
     if(newtouch[k]){
      Send_MIDI_BLE(1,NOTE_ON,notes[k],127);
      newrelease[k] = false;
      }
     else if(newrelease[k]){
      Send_MIDI_BLE(1,NOTE_OFF,notes[k],127);
      newtouch[k] = false;
      }
    }
  }
  else{if(connection_state == 1){connection_state = 0;}}
#endif
}

#ifndef DEBUG
void Send_MIDI_BLE(uint8_t channel, uint8_t status_Byte, uint8_t note, uint8_t velocity){
  uint8_t msgBuf[5];    
  uint16_t currentTimeStamp = millis() & 0x01FFF;
  msgBuf[0] = ((currentTimeStamp >> 7) & 0x3F) | 0x80; //6 bits plus MSB
  msgBuf[1] = (currentTimeStamp & 0x7F) | 0x80; //7 bits plus MSB
  msgBuf[2] = (status_Byte | ((channel - 1) & 0x0f));
  msgBuf[3] = note;
  msgBuf[4] = velocity;
  characteristic.setValue(msgBuf, 5);
}
#endif
