#ifndef CONTROL   // Transmiter functions
  void sendRadio() {
    data[0] = HEAD;     //envia el canal (Editar thisrock)
    MPR121.updateAll();
    //delay(10);
    for(uint8_t n=0; n<SENSORS; n++){
      int16_t diff = MPR121.getFilteredData(n)-MPR121.getBaselineData(n);
      raw[n] = diff;
      if( diff > maxdiff[n]){maxdiff[n] = diff;}
      else if( diff < mindiff[n]){mindiff[n] = diff;}
      data[1] = n; //envia la nota (Editar thisrock)
      data[2] = CC;
      data[3] = map(diff,maxdiff[n],mindiff[n],0,127);
      if (n == 12){data[3] = map(diff,MAXPROX,MINPROX,0,127);}

      if (MPR121.isNewTouch(n) && n != 12 ) {     //check if there is a new touch on the electrode
        data[2] = NOTE_ON;
        radio.write(data, sizeof data);
        debug();
        touching[n] = true;
      }
      
      else if (MPR121.isNewRelease(n) && n != 12 ) {   //check if the touch has just been lifted
        data[2] = NOTE_OFF;
        radio.write(data, sizeof data);
        debug();
        touching[n] = false;
      }
      
      if (n == 12 ){
        if(data[3] >=25){
          if(windowCount[n] == WINDOW){
            data[3] = filteredCC[n] / WINDOW;
            windowCount[n] = 0;
            filteredCC[n] = 0; 
            radio.write(data, sizeof data);
            debug();
          }
          else{
            filteredCC[n] = filteredCC[n] + int16_t(data[3]);
            windowCount[n] += 1;
          }
        }
        touching[n] = false;
      }
      
      if(touching[n] == true){
        if(windowCount[n] == WINDOW){
          data[3] = filteredCC[n] / WINDOW;
          windowCount[n] = 0;
          filteredCC[n] = 0; 
          radio.write(data, sizeof data);
          debug();
        }
        else{
          filteredCC[n] = filteredCC[n] + int16_t(data[3]);
          windowCount[n] += 1;
        }
      }
    } // end of electrode for loop
  }
#endif
