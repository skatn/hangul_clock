#include <EEPROM.h>
#include <Wire.h>

#define DISPLAY_TYPE_HORIZONTAL

#define EEPROM_SIZE 512
#define EEPROM_ADDR_DISPLAYMODE 0
#define EEPROM_ADDR_BRIGHTNESS  1

#define DISPLAY_MODE_STANDARD 0
#define DISPLAY_MODE_FADE     1
#define DISPLAY_MODE_COUNT    2

#define CELL_SIZE 6
#define GAMMA_MAX 64
//#define USE_GAMMA

bool display[CELL_SIZE][CELL_SIZE];
bool lastDisplay[CELL_SIZE][CELL_SIZE];
byte pwmAddress[CELL_SIZE][CELL_SIZE]={             //led Control address == pwmAddress + 0x25
  0x1C, 0x1F, 0x22, 0x01, 0x04, 0x07,
  0x1D, 0x20, 0x23, 0x02, 0x05, 0x08,
  0x1E, 0x21, 0x24, 0x03, 0x06, 0x09,
  0x1B, 0x18, 0x15, 0x10, 0x0D, 0x0A,
  0x1A, 0x17, 0x14, 0x11, 0x0E, 0x0B,
  0x19, 0x16, 0x13, 0x12, 0x0F, 0x0C
};
byte PWM_Gamma64[GAMMA_MAX]={
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
  0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
  0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
  0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
  0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
  0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
  0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
  0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff
};
byte brightness = 50;    //0~100
byte displayMode = DISPLAY_MODE_STANDARD;

void displayInit(){
  EEPROM.begin(512);
  
  i2cInit();
  readSettings();
}

void readSettings(){
  displayMode = EEPROM.read(EEPROM_ADDR_DISPLAYMODE);
  brightness = EEPROM.read(EEPROM_ADDR_BRIGHTNESS);
  
  if(displayMode >= DISPLAY_MODE_COUNT) setDisplayMode(DISPLAY_MODE_STANDARD);
  if(brightness >= 100) setBrightness(100);
}

void setBrightness(int value){
  if(value < 0) value = 0;
  else if(value > 100) value = 100;
  brightness = value;
  EEPROM.write(EEPROM_ADDR_BRIGHTNESS, brightness);
  EEPROM.commit();
}

void setDisplayMode(int value){
  displayMode = value;
  EEPROM.write(EEPROM_ADDR_DISPLAYMODE, value);
  EEPROM.commit();
}

void i2cInit(){
  Wire.begin(4, 5);
  Wire.setClock(400000);
  delay(1000);
  
  sendI2C(0x00, 1); //Shutdown Register   0: Software shutdown mode / 1: Normal operation 
  sendI2C(0x4B, 0); //frequency setting  0: 3kHz / 1: 22kHz
  sendI2C(0x4A, 0); //Global Control Register   0: Normar operation / 1: Shutdown all LEDs
}

void sendI2C(int address, int data){
  Wire.beginTransmission(0b00111100);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission(true);
}

void copyDisplay(){
  for(byte y=0; y<CELL_SIZE; y++){
    for(byte x=0; x<CELL_SIZE; x++){
      lastDisplay[y][x] = display[y][x];
    }
  }
}

void clearDisplay(){
  for(byte y=0; y<CELL_SIZE; y++){
    for(byte x=0; x<CELL_SIZE; x++){
      display[y][x] = false;
    }
  }
}

void setDisplay(byte y, byte x){
  display[y][x] = true;
}

void setLedCell(byte y, byte x, byte brightness){
  if(brightness == 0)
    sendI2C(pwmAddress[y][x]+0x25, 0);            //led register
  else 
    sendI2C(pwmAddress[y][x]+0x25, 1);
    
#if defined(USE_GAMMA)
  int gamma = brightness/100.0 * (GAMMA_MAX-1);
  sendI2C(pwmAddress[y][x], PWM_Gamma64[gamma]);     //pwm register
#else
  int value = brightness/100.0 * 255;
  sendI2C(pwmAddress[y][x], value);
#endif
}

void showDisplay(){
  for(byte y=0; y<CELL_SIZE; y++){
    for(byte x=0; x<CELL_SIZE; x++){
      if(display[y][x]){
        setLedCell(y, x, brightness);
      }
      else{
        setLedCell(y, x, 0);
      }
    }
  }
  sendI2C(0x25, 0);   //update setting
}

void updateDisplay(){
  int hour=getHourFromNtp();
  int minute=getMinutesFromNtp();
  int minute10 = minute / 10;
  int minute1 = minute % 10;
  int second=getSecondsFromNtp();

  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);
  
  copyDisplay();
  clearDisplay();

  if(second < 5){
    setDisplay(3, 0);
    if (hour < 12)
      setDisplay(4, 0);
    else 
      setDisplay(5, 0);
  }

  setDisplay(2, 5);
  if(hour > 12) hour -= 12;
  switch (hour) {
    case 1:  setDisplay(0, 0);                    break;
    case 2:  setDisplay(0, 1);                    break;
    case 3:  setDisplay(0, 2);                    break;
    case 4:  setDisplay(0, 3);                    break;
    case 5:  setDisplay(0, 4); setDisplay(0, 5);  break;
    case 6:  setDisplay(1, 0); setDisplay(1, 1);  break;
    case 7:  setDisplay(1, 2); setDisplay(1, 3);  break;
    case 8:  setDisplay(1, 4); setDisplay(1, 5);  break;
    case 9:  setDisplay(2, 0); setDisplay(2, 1);  break;
    case 10: setDisplay(2, 2);                    break;
    case 11: setDisplay(2, 2); setDisplay(2, 3);  break;
    case 0:
    case 12: setDisplay(2, 2); setDisplay(2, 4);  break;
  }

  if (minute > 0)setDisplay(5, 5);
  if (minute10 > 0) {
    setDisplay(3, 5);
    if (minute10 > 1)setDisplay(3, minute10 - 1);
  }
  if (minute1 > 0) {
    if (minute1 < 6)setDisplay(4, minute1);
    else setDisplay(5, minute1 - 5);
  }
}

void fade(){
  int animBrightness;
  for(float t=1; t>=0; t-=0.01){
    animBrightness = t*brightness;
    for(byte y=0; y<CELL_SIZE; y++)for(byte x=0; x<CELL_SIZE; x++){ 
      if(lastDisplay[y][x]){
        setLedCell(y, x, animBrightness);
      }
      else{
        setLedCell(y, x, 0);
      }
    }
    sendI2C(0x25, 0);   //update setting
    delay(10);
  }
  
  for(float t=0; t<1.009; t+=0.01){
    animBrightness = t*brightness;
    for(byte y=0; y<CELL_SIZE; y++)for(byte x=0; x<CELL_SIZE; x++){ 
      if(display[y][x]){
        setLedCell(y, x, animBrightness);
      }
      else{
        setLedCell(y, x, 0);
      }
    }
    sendI2C(0x25, 0);   //update setting
    delay(10);
  }
}

void test(){
  i2cInit();
  while(true){
    for(int i=0; i<100; i++){
      setLedCell(2, 2, i);
      sendI2C(0x25, 0);   //update setting
      delay(25);
    }
    for(int i=100; i>0; i--){
      setLedCell(2, 2, i);
      sendI2C(0x25, 0);   //update setting
      delay(25);
    }
  }
}

int getDisplayMode(){
  return displayMode;
}

int getBrightness(){
  return brightness;
}
