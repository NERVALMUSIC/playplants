void BLEmanager()
{
  BLECentral central = blePeripheral.central();
  if (central && central.connected()) {
    if (connection_state != 1) {connection_state = 1;}
    if (characteristic.written()) {
      Get_MIDI_BLE();
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
            transmitMIDIonDIN( lastStatus, 0, 0 );
        } else if( rPtr - lPtr < 2 ) {
            transmitMIDIonDIN( lastStatus, buffer[lPtr + 1], 0 );
        } else if( rPtr - lPtr < 3 ) {
            transmitMIDIonDIN( lastStatus, buffer[lPtr + 1], buffer[lPtr + 2] );
        } else {
            //Too much data
            //If not System Common or System Real-Time, send it as running status
            switch( buffer[lPtr] & 0xF0 )
            {
            case 0x80:
            case 0x90:
            case 0xA0:
            case 0xB0:
            case 0xE0:
                for(int i = lPtr; i < rPtr; i = i + 2){
                    transmitMIDIonDIN( lastStatus, buffer[i + 1], buffer[i + 2] );
                }
                break;
            case 0xC0:
            case 0xD0:
                for(int i = lPtr; i < rPtr; i = i + 1){
                    transmitMIDIonDIN( lastStatus, buffer[i + 1], 0 );
                }
                break;
            default:
                break;
            }
        }
        //Point to next status
        lPtr = rPtr + 2;
        if(lPtr >= bufferSize){
            //end of packet
            return;
        }
    }
}

void transmitMIDIonDIN( uint8_t status, uint8_t data1, uint8_t data2 )
{
  uint8_t channel = status & 0x0F;
  channel++;
  uint8_t command = (status & 0xF0) >> 4;
  switch (command)
  {
    case 0x08: //Note off
      break;
    case 0x09: //Note on
      if(channel > 0 && channel <= 12){
        notes[channel] = data1;
        red.Off(); green.Breathe(FAST).Repeat(1); blue.Breathe(FAST).Repeat(1);  // BLINK Purple
      }
      break;
    case 0x0A: //Polyphonic Pressure
      break;
    case 0x0B: //Control Change
      switch (data1)
      {
        case 0x66: //CC 102
        if(data2 > 0 && data2 <= 16){
          CHANN = data2; 
          red.Off(); green.Breathe(FAST).Repeat(1); blue.Breathe(FAST).Repeat(1);  // BLINK Purple
        }
        break;
        case 0x67: //CC 103
        TOUCH = map(data2,0,127,5,60);
        red.Off(); green.Breathe(FAST).Repeat(1); blue.Breathe(FAST).Repeat(1);  // BLINK Purple
        break;
        case 0x68: //CC 104
        RELEASE = map(data2,0,127,2,50);
        red.Off(); green.Breathe(FAST).Repeat(1); blue.Breathe(FAST).Repeat(1);  // BLINK Purple
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
        case 0x75: //CC 117
        break;
        case 0x76: //CC 118
        break;
        case 0x77: //CC 119
        break;
      }
      break;
    case 0x0C: //Program Change
      break;
    case 0x0D: //Channel Pressure
      break;
    case 0x0E: //Pitch Bend
      break;
    case 0x0F: //System
      switch (status)
      {
        case 0xF1: //MTC Q frame
          break;
        case 0xF2: //Song position
          break;
        case 0xF3: //Song select
          break;
        case 0xF6: //Tune request
          break;
        case 0xF8: //Timing Clock
        case 0xFA: //Start
        case 0xFB: //Continue
        case 0xFC: //Stop
        case 0xFE: //Active Sensing
        case 0xFF: //Reset
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
