void BLEmanager()
{
#ifdef DEBUG
  blePeripheral.poll();
  if (blePeripheral){
    if(connection_state == 0){connection_state = 1;}
#else
  BLECentral central = blePeripheral.central();
  if (central && central.connected()) {
    if(connection_state == 0){connection_state = 1;}
#endif
    for( int k = 0; k < KEYS; k++){
      if(newtouch[k]){
        Send_MIDI_BLE(1, NOTE_ON, notes[k], 127);
        newtouch[k] = false;
      }
      if(newrelease[k]){
        Send_MIDI_BLE(1, NOTE_OFF, notes[k], 0);
        newrelease[k] = false;
      }
    }
  }
  else{if(connection_state == 1){connection_state = 0;}}
}

void Send_MIDI_BLE(uint8_t channel, uint8_t status_Byte, uint8_t note, uint8_t velocity){
#ifdef DEBUG
  blePeripheral.print(note);  blePeripheral.print(": 0x"); blePeripheral.println(status_Byte,HEX);
#else
  uint8_t msgBuf[5];    
  uint16_t currentTimeStamp = millis() & 0x01FFF;
  msgBuf[0] = ((currentTimeStamp >> 7) & 0x3F) | 0x80; //6 bits plus MSB
  msgBuf[1] = (currentTimeStamp & 0x7F) | 0x80; //7 bits plus MSB
  msgBuf[2] = (status_Byte | ((channel - 1) & 0x0f));
  msgBuf[3] = note;
  msgBuf[4] = velocity;
  characteristic.setValue(msgBuf, 5);
#endif
}
