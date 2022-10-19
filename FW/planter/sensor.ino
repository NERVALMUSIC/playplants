void MPRconfig(){
  /*
  * Para un mejor control de la sensibilidad y la deteccion de toques consultar:
  * Datasheet:https://www.sparkfun.com/datasheets/Components/MPR121.pdf
  * AN3889: https://www.nxp.com/docs/en/application-note/AN3889.pdf
  * AN3890: https://www.nxp.com/docs/en/application-note/AN3890.pdf
  */
  MPR121.goFast();                      //Set I2C speed
  MPR121.setProxMode(prox_mode);        //PROX_DISABLED 
                                        //PROX_0_1 
                                        //PROX_0_3 
                                        //PROX_0_11
  MPR121.setNumEnabledElectrodes(SENSORS);
  
  /*
  * Optional settings, test best configuration for each plant,
  * see datasheet and application notes to better understand how
  * to change these settings.
  */
  //MPR121.setCalibrationLock(CAL_LOCK_ENABLED); //CAL_LOCK_ENABLED 
                                                 //CAL_LOCK_DISABLED 
                                                 //CAL_LOCK_ENABLED_5_BIT_COPY 
                                                 //CAL_LOCK_ENABLED_10_BIT_COPY
  //MPR121.autoSetElectrodes(false);
  //MPR121.autoSetElectrodeCDC();
  MPR121.setGlobalCDC(52);
  MPR121.setGlobalCDT(CDT_2US);
  MPR121.setFFI(FFI_6);
  MPR121.setSamplePeriod(SAMPLE_INTERVAL_1MS);
  MPR121.setSFI(SFI_4);
  for (int i = 0; i < SENSORS; i++) {
    CCnotes[i] = CCNOTE + i;
  }
  delay(200);
}

//Poll the sensor inputs and convert any relevant data to midi format
void Sensormanager(){
  MPR121.updateAll();
  for (int i = 0; i < SENSORS; i++) {
    if (MPR121.isNewTouch(i)) {
      touching[i] = true;
      constrain(sensed += 20,0,255);
    } 
    else if (MPR121.isNewRelease(i)) {
      touching[i] = false;
      constrain(sensed -= 20,0,255);
      Send_MIDI_BLE(CHANN, NOTE_OFF, notes[i], 0);
    }
    if (touching[i])
    {
      int16_t diff = MPR121.getFilteredData(i)-MPR121.getBaselineData(i);
      raw[i] = diff;      
      if( diff > maxdiff[i]){maxdiff[i] = diff;}
      if(diff < SATVAL){ diff = SATVAL;}
      else if( diff < mindiff[i]){mindiff[i] = diff;}
      velocity[i] = uint8_t(map(diff,maxdiff[i],mindiff[i],0,127));
      if(windowCount[i] == WINDOW){
        velocity_filt[i] = filtered_data[i] / WINDOW;
        windowCount[i] = 0;
        filtered_data[i] = 0;
        Send_MIDI_BLE(CHANN, CC, notes[i], velocity_filt[i]);
      }
      else{
        filtered_data[i] = filtered_data[i] + velocity[i];
        windowCount[i] += 1;
      }
    }
    if(MPR121.isNewTouch(i)){
      Send_MIDI_BLE(CHANN, NOTE_ON, notes[i], velocity[i]);
    }
  }
}
