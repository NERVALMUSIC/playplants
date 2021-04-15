#ifdef CONTROL   //Receiver funcions

  void receiver() {
    switch (modes[payload.channel-1])   //choose mode for the channel received based on last time it was configured
    {
      case 0:
        if( payload.note != 12 && (payload.message == NOTE_ON || payload.message == NOTE_OFF)){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][payload.note], payload.velocity); 
//Send notes when touched
        }
      break;
      case 1:
        if( payload.note != 12){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][payload.note], payload.velocity);     //Send CC based on intensity
        }
      break;
      case 2:
        if( payload.note == 12){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][0], payload.velocity);     //Send CC based on proximity to rock
        }
      break;
      case 3:
        if( payload.note != 12 && (payload.message == NOTE_OFF)){
          sendMIDI(NOTE_ON, payload.channel, notes[payload.channel-1][0]+counter, 127);     //Send increasing array
          sendMIDI(NOTE_OFF, payload.channel, notes[payload.channel-1][0]+counter, 0);     //Send increasing array
          counter += 1;
          if (counter == notes[payload.channel-1][1])
          {
            counter = 0;
          }
        }
      break;
      case 4:
        if(payload.note != 12){
          sendMIDI(payload.message, payload.channel, notes[payload.channel-1][random(0, 11)], payload.velocity);     //Send random values while touching
          counter += 1;
        }
      break;
      default:
      #ifdef DEBUG
        Serial.println("Mode not implemented!!");
      #endif
      break;
    }
  }

  void sendMIDI(uint8_t messageType, uint8_t channel, uint8_t data1, uint8_t data2){
    messageType &= 0b11110000;                   // Message type is high nibble
    channel     &= 0b00001111;                   // Channel is low nibble
    uint8_t statusByte = messageType | channel;  // Combine the messageType (high nibble) with the channel (low nibble)
    statusByte  |= 0b10000000;                   // Set the most significant bit of the status byte
    data1       &= 0b01111111;                   // Clear the most significant bit of the data bytes
    data2       &= 0b01111111;
    #ifdef DEBUG
      Serial.print("Channel");Serial.print("\t|");Serial.print("Message");Serial.print("\t|");Serial.print("Byte 1");Serial.print("\t|");Serial.println("Byte 2");
      Serial.print(channel);Serial.print("\t|");Serial.print(messageType,HEX);Serial.print("\t\t|");Serial.print(data1);Serial.print("\t|");Serial.println(data2);
    #else
      statusByte -= 1;        // to correct hairlessmidi offset
      Serial.write(statusByte);                    // Send over Serial
      Serial.write(data1);
      Serial.write(data2);
    #endif
  }
#endif
