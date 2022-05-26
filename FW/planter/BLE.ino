void BLEmanager()
{
  BLECentral central = blePeripheral.central();
  if (central && central.connected()) {
    if(connection_state != 1){connection_state = 1;}
    if (characteristic.written()) {
      Get_MIDI_BLE();
    }    
    for( int k = 0; k < SENSORS; k++){
      if(newrelease[k]){
        Send_MIDI_BLE(CHANN, NOTE_OFF, notes[k], 0);
        newrelease[k] = false;
      }
      if(newtouch[k]){
        Send_MIDI_BLE(CHANN, NOTE_ON, notes[k], velocity[k]);
        newtouch[k] = false;
      }
    }
  }
  else{if(connection_state == 1){connection_state = 0;}}
}

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

void Get_MIDI_BLE()
{
    //Receive the written packet and parse it out here.
    uint8_t * buffer = (uint8_t*)characteristic.value();
    uint8_t bufferSize = characteristic.valueLength();

    //Pointers used to search through payload.
    uint8_t lPtr = 0;
    uint8_t rPtr = 0;
    //lastStatus used to capture runningStatus 
    uint8_t lastStatus;
    //Decode first packet -- SHALL be "Full MIDI message"
    lPtr = 2; //Start at first MIDI status -- SHALL be "MIDI status"
    //While statement contains incrementing pointers and breaks when buffer size exceeded.
    while(1){
        lastStatus = buffer[lPtr];
        if( (buffer[lPtr] < 0x80) ){
            //Status message not present, bail
            return;
        }
        //Point to next non-data byte
        rPtr = lPtr;
        while( (buffer[rPtr + 1] < 0x80)&&(rPtr < (bufferSize - 1)) ){
            rPtr++;
        }
        //look at l and r pointers and decode by size.
        if( rPtr - lPtr < 1 ){
            //Time code or system
            //transmitMIDIonDIN( lastStatus, 0, 0 );
        } else if( rPtr - lPtr < 2 ) {
            //transmitMIDIonDIN( lastStatus, buffer[lPtr + 1], 0 );
        } else if( rPtr - lPtr < 3 ) {
            //transmitMIDIonDIN( lastStatus, buffer[lPtr + 1], buffer[lPtr + 2] );
        }
    }
}
