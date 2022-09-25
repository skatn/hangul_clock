void mainSetup(){
  wifiConnect();  
  ntpInit();
  displayInit();
}

void mainLoop(){
  static int targetBrightness = 0;
  byte timeChangeState = getTimeChangeState();
  if(timeChangeState == TIME_CHANGE_NULL && getAutoBrightness() == false) return;

  
  updateDisplay();
  if(getAutoBrightness()){
    targetBrightness = map(getCdsValue(), getCdsMin(), getCdsMax(), 1, 100);
//    Serial.println(targetBrightness);
    int b = getActiveBrightness();
    if(b < targetBrightness) setActiveBrightness(b+1);
    else if(b > targetBrightness) setActiveBrightness(b-1);
  }
  else{
    setActiveBrightness(getBrightness());
  }
  

  switch(displayMode){
    case DISPLAY_MODE_STANDARD:
      break;
    case DISPLAY_MODE_FADE:
      if(timeChangeState == TIME_CHANGE_MINUTE){
        fade();
      }
      break;
  }

  showDisplay();
}

void test(){
  displayInit();
  int target = 0;
  int b = 0;
  while(true){
    unsigned int value = getCdsValue();
    Serial.print(b);
    Serial.print(",");
    Serial.print(value);
    Serial.print(",");
    Serial.println(analogRead(A0));

    
    target = map(value, getCdsMin(), getCdsMax(), 1, 100);

    if(b < target) b++;
    else if(b > target) b--;


//    for(byte y=0; y<CELL_SIZE; y++)
      for(byte x=0; x<CELL_SIZE; x++)
          setLedCell(0, x, b);
      
    sendI2C(0x25, 0);   //update setting
    delay(10);
  }
}
