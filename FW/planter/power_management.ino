
void readBattery(){
  digitalWrite(POW_EN, HIGH); // turn on battery monitor
  delay(10);
  bat_adc = analogRead(BAT_MON);
  digitalWrite(POW_EN, LOW); // turn off battery monitor
  if( bat_adc <= BAT_LOW){ red.Breathe(SLOW).Repeat(1);green.Off();blue.Off();  }// SLOW BLINK RED
  else if( bat_adc <= BAT_HIGH){red.Breathe(SLOW).Repeat(1);green.Breathe(SLOW).Repeat(1);blue.Off();  }// SLOW BLINK YELLOW
  else{red.Off();green.Breathe(SLOW).Repeat(1);blue.Off();  }// SLOW BLINK GREEN
}

void charge_change(){
  if(digitalRead(CHG) == HIGH && digitalRead(USB) == HIGH){connection_state = 3;}
  else if(digitalRead(CHG) == LOW && digitalRead(USB) == HIGH){connection_state = 2;}
  else{connection_state = 0;}
}

void power_off(){
  /*
  //memory update
  if(update){
    for (int i = 0; i < SENSORS; i++) {
      params[i] = char(notes[i]);
    }
    params[12] = char(CHANN);
    params[13] = char(TOUCH);
    params[14] = char(RELEASE);
    params[15] = char(SATVAL);
    params[16] = char(WINDOW);
    bond.putData(params,0,STORED);
  }
  */
  MPR121.stop();
  red.Breathe(SLOW).Repeat(1);  
  green.Breathe(SLOW).Repeat(1);  
  blue.Breathe(SLOW).Repeat(1);// SLOW BLINK WHITE
  while(red.IsRunning()){red.Update();green.Update();blue.Update();}  //wait for led show before powering off
  pinMode(RED_LED, INPUT_PULLUP);
  pinMode(GRN_LED, INPUT_PULLUP);
  pinMode(BLU_LED, INPUT_PULLUP);
  digitalWrite(POW_EN, LOW);
  nRF5x_lowPower.powerMode(POWER_MODE_OFF);     //Power off if long press has been detected
}
