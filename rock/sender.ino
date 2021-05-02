#ifndef CONTROL   // Transmiter functions
  void sender() {
    MPR121.updateAll();
    //delay(10);
    for(uint8_t n=0; n<SENSORS; n++){
      
      int16_t diff = MPR121.getFilteredData(n)-MPR121.getBaselineData(n);
      raw[n] = diff;
      if( diff > maxdiff[n]){maxdiff[n] = diff;}
      if(diff < SATVAL){ diff = SATVAL;}
      else if( diff < mindiff[n]){mindiff[n] = diff;}
      payload.channel = HEAD;
      payload.note = n;
      payload.message = CC;
      payload.velocity = uint8_t(map(diff,maxdiff[n],mindiff[n],0,127));
      
      if (n == 12){payload.velocity = uint8_t(map(diff,MAXPROX,MINPROX,0,127));}

      if (MPR121.isNewTouch(n) && n != 12 ) {     //check if there is a new touch on the electrode
        payload.message = NOTE_ON;
        sendRadio();
        debug();
        touching[n] = true;
      }
      
      else if (MPR121.isNewRelease(n) && n != 12 ) {   //check if the touch has just been lifted
        payload.message = NOTE_OFF;
        sendRadio();
        debug();
        touching[n] = false;
      }
      
      if (n == 12 ){
        if(payload.velocity >=25){
          if(windowCount[n] == WINDOW){
            payload.velocity = filteredCC[n] / WINDOW;
            windowCount[n] = 0;
            filteredCC[n] = 0; 
            sendRadio();
            debug();
          }
          else{
            filteredCC[n] = filteredCC[n] + uint8_t(payload.velocity);
            windowCount[n] += 1;
          }
        }
        touching[n] = false;
      }
      
      if(touching[n] == true){
        if(windowCount[n] == WINDOW){
          payload.velocity = filteredCC[n] / WINDOW;
          windowCount[n] = 0;
          filteredCC[n] = 0; 
          sendRadio();
          debug();
        }
        else{
          filteredCC[n] = filteredCC[n] + uint8_t(payload.velocity);
          windowCount[n] += 1;
        }
      }
    } // end of electrode for loop
  }

void sendRadio(){
  if (!mesh.write(&payload, 'M', sizeof(payload))) {
    // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
      //refresh the network address
      Serial.println("Renewing Address");
      mesh.renewAddress();
    } else {
      Serial.println("Send fail, Test OK");
    }
  } else {
    Serial.print("Send OK: ");
  }
}
 
#endif
