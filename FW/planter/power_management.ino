
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
  if(digitalRead(CHG) == HIGH){connection_state = 2;}
  else{connection_state = 0;}
}

void power_off(){
  digitalWrite(POW_EN, LOW);
  //TO DO: shut down sensor
  red.Breathe(SLOW).Repeat(1);  
  green.Breathe(SLOW).Repeat(1);  
  blue.Breathe(SLOW).Repeat(1);// SLOW BLINK WHITE
  while(red.IsRunning()){red.Update();green.Update();blue.Update();}  //wait for led show before powering off
  pinMode(RED_LED, INPUT_PULLUP);
  pinMode(GRN_LED, INPUT_PULLUP);
  pinMode(BLU_LED, INPUT_PULLUP);
  nRF5x_lowPower.powerMode(POWER_MODE_OFF);     //Power off if long press has been detected
}
