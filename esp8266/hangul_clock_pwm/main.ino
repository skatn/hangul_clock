void mainSetup(){
  wifiConnect();  
  ntpConnect();
  displayInit();
}

void mainLoop(){
  byte timeChangeState = getTimeChangeState();
  if(timeChangeState == TIME_CHANGE_NULL) return;
  updateDisplay();

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
