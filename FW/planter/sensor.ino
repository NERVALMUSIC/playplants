void MPRconfig(){
  /*
  * Para un mejor control de la sensibilidad y la deteccion de toques consultar:
  * Datasheet:https://www.sparkfun.com/datasheets/Components/MPR121.pdf
  * AN3889: https://www.nxp.com/docs/en/application-note/AN3889.pdf
  * AN3890: https://www.nxp.com/docs/en/application-note/AN3890.pdf
  */
  MPR121.goFast();                      //Set I2C speed
  MPR121.setTouchThreshold(TOUCH);      //Set Touch Threshold
  MPR121.setReleaseThreshold(RELEASE);  //Set Release Threshold
  MPR121.setProxMode(prox_mode);        //PROX_DISABLED 
                                        //PROX_0_1 
                                        //PROX_0_3 
                                        //PROX_0_11
  
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
  //MPR121.setGlobalCDC(60);
  //MPR121.setGlobalCDT(CDT_4US);
}

//Poll the sensor inputs and convert any relevant data to midi format
void Sensormanager(){
  MPR121.updateAll();
  for (int i = 0; i < SENSORS; i++) {
    if (MPR121.isNewTouch(i)) {
      newtouch[i] = true;
    } 
    else if (MPR121.isNewRelease(i)) {
      newrelease[i] = true;
    }
  }
}
