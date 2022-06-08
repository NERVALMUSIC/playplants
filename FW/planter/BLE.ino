void BLEmanager()
{
  BLECentral central = blePeripheral.central();
  if (central && central.connected()) {
    if (connection_state != 1) {connection_state = 1;}
    if (characteristic.written()) {
      process_package();
    }
  }
  else {
    if (connection_state == 1) {connection_state = 0;}
  }
}

void Send_MIDI_BLE(uint8_t channel, uint8_t status_Byte, uint8_t note, uint8_t velocity) {
  uint8_t msgBuf[5];
  uint16_t currentTimeStamp = millis() & 0x01FFF;
  msgBuf[0] = ((currentTimeStamp >> 7) & 0x3F) | 0x80; //6 bits plus MSB
  msgBuf[1] = (currentTimeStamp & 0x7F) | 0x80; //7 bits plus MSB
  msgBuf[2] = (status_Byte | ((channel - 1) & 0x0f));
  msgBuf[3] = note;
  msgBuf[4] = velocity;
  characteristic.setValue(msgBuf, 5);
}

void process_package()
{
    //Receive the written packet and parse it out here.
    uint8_t * buffer = (uint8_t*)characteristic.value();
    uint8_t bufferSize = characteristic.valueLength();

    //Pointers used to search through payload.
    uint8_t lPtr = 0;
    uint8_t rPtr = 0;
    
    //Decode first packet -- SHALL be "Full MIDI message"
    lPtr = 2; //Start at first MIDI status -- SHALL be "MIDI status"
    
    //Variables to send once the buffer is read
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
    
    //While statement contains incrementing pointers and breaks when buffer size exceeded.
    while(1){
        status = buffer[lPtr];
        if( (status < 0x80) ){ //Status message not present, bail without sending MIDI message
            return;
        }
        //Point to next non-data byte
        rPtr = lPtr;
        while( (buffer[rPtr + 1] < 0x80)&&(rPtr < (bufferSize - 1)) ){
            rPtr++;
        }
        //look at l and r pointers and decode by size.
        if( rPtr - lPtr < 1 ){
          data1 = 0;
          data2 = 0;
        } 
        else if( rPtr - lPtr < 2 ) {
          data1 = buffer[lPtr + 1];
          data2 = 0;
        } 
        else if( rPtr - lPtr < 3 ) {
          data1 = buffer[lPtr + 1];
          data2 = buffer[lPtr + 2];
        }
        //Point to next status
        lPtr = rPtr + 2;
        if(lPtr >= bufferSize){ //Send last midi message received
            Get_MIDI_BLE( status, data1, data2);
            red.Off(); green.Breathe(FAST).Repeat(1); blue.Breathe(FAST).Repeat(1);  // BLINK Purple
            return;
        }
    }
}

void Get_MIDI_BLE( uint8_t status, uint8_t data1, uint8_t data2 )
{
  uint8_t channel = status & 0x0F;
  channel++;
  uint8_t command = (status & 0xF0) >> 4;
  switch (command)
  {
    case 0x08: //Note off
      if(channel > 0 && channel <= 12){
        notes[channel] = data1;
      }
      break;
    case 0x0B: //Control Change
      switch (data1)
      {
        case 0x66: //CC 102
        if(data2 > 0 && data2 <= 16){
          CHANN = data2; 
        }
        break;
        case 0x67: //CC 103
        TOUCH = map(data2,0,127,2,100);
        RELEASE = map(data2,0,127,1,60);
        MPR121.setTouchThreshold(TOUCH);
        MPR121.setReleaseThreshold(RELEASE);
        break;
        case 0x68: //CC 104
        break;
        case 0x69: //CC 105
        break;
        case 0x6A: //CC 106
        break;
        case 0x6B: //CC 107
        break;
        case 0x6C: //CC 108
        break;
        case 0x6D: //CC 109
        break;
        case 0x6E: //CC 110
        break;
        case 0x6F: //CC 111
        break;
        case 0x70: //CC 112
        break;
        case 0x71: //CC 113
        break;
        case 0x72: //CC 114
        break;
        case 0x73: //CC 115
        break;
        case 0x74: //CC 116
        break;
      }
      break;
    default:
      break;
  }
}
