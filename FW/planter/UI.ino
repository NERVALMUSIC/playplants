void led_update(){
  if (sensed != 0){
    if(!red.IsRunning() && !green.IsRunning() && !blue.IsRunning())
    {
      red.Set(0); green.Set(sensed); blue.Set(0); //BREATHE BLUE
    }   
  }
  else{
    switch(connection_state){
      case 0:     //advertising
        if(!red.IsRunning() && !green.IsRunning() && !blue.IsRunning())
        {
          red.Set(0); green.Set(0); blue.Breathe(SLOW).Repeat(1); //BREATHE BLUE
        }
        break;
      case 1:     //connected
        if(!red.IsRunning() && !green.IsRunning() && !blue.IsRunning())
        {
          red.Set(0); green.Set(0); blue.Set(127); //SOLID BLUE
        }
        break;
      case 2:     //charging
        if(!red.IsRunning() && !green.IsRunning() && !blue.IsRunning())
        {
          red.Set(255); green.Set(169); blue.Set(0); //SOLID ORANGE
        }
        break;
      case 3:     //charged
        if(!red.IsRunning() && !green.IsRunning() && !blue.IsRunning())
        {
          red.Set(0); green.Set(255); blue.Set(0); //SOLID GREEN
        }
        break;
    }    
  }

  red.Update();  green.Update();  blue.Update();
}

// This function will be called when the button is pressed once.
void singleclick() {
    //TO DO: Assign function to single click
    red.Off(); green.Breathe(FAST).Repeat(1); blue.Breathe(FAST).Repeat(1);  // BLINK Purple
    MPR121.begin(I2CADDR, TOUCH, RELEASE, INT);
    MPRconfig();
} // singleclick 

// This function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick() {
  readBattery();
} // doubleclick

// This function will be called once, when the button is released after beeing pressed for a long time.
void longPressStop() {
  power_off();
} // longPressStop
