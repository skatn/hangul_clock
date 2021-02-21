#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESPAsyncWiFiManager.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <EEPROM.h>

#define DISPLAY_TYPE_HORIZONTAL

#define EEPROM_SIZE 512
#define EEPROM_ADDR_DISPLAYMODE 0
#define EEPROM_ADDR_BRIGHTNESS  1

#define TIME_CHANGE_NULL    0
#define TIME_CHANGE_SECOND  1
#define TIME_CHANGE_MINUTE  2
#define DISPLAY_MODE_STANDARD 0
#define DISPLAY_MODE_FADE     1

#define MAX 6
#define NTP_UPDATE_INTERVAL  3600000 //60m*60s*1000ms
int rawTime = 0, currTime = 0;
long timeOffset = 32400;

const char* ssidAutoConnect = "SEMI_AUTO";
const char* ssidAP = "SEMI_Hangul_Clock";
const char* passwordAP = "1234567890";
bool wifiConnectOK = false;

WiFiUDP udp;
NTPClient timeClient(udp, "kr.pool.ntp.org", timeOffset, NTP_UPDATE_INTERVAL);

AsyncWebServer server(80);
DNSServer dns;

bool display[MAX][MAX];
bool lastDisplay[MAX][MAX];
byte pwmAddress[MAX][MAX]={             //led Control address = pwmAddress + 0x25
  0x1C, 0x1F, 0x22, 0x01, 0x04, 0x07,
  0x1D, 0x20, 0x23, 0x02, 0x05, 0x08,
  0x1E, 0x21, 0x24, 0x03, 0x06, 0x09,
  0x1B, 0x18, 0x15, 0x10, 0x0D, 0x0A,
  0x1A, 0x17, 0x14, 0x11, 0x0E, 0x0B,
  0x19, 0x16, 0x13, 0x12, 0x0F, 0x0C
};
byte PWM_Gamma64[64]={
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
  0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
  0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
  0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
  0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
  0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
  0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
  0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff
};
byte brightness = 32;    //0~64
byte displayMode = DISPLAY_MODE_STANDARD;

void readSettings(){
  EEPROM.begin(512);

  displayMode = EEPROM.read(EEPROM_ADDR_DISPLAYMODE);
  brightness = EEPROM.read(EEPROM_ADDR_BRIGHTNESS);
  if(displayMode > 10) setDisplayMode(DISPLAY_MODE_STANDARD);
  if(brightness > 64) setBrightness(64);
}

void setBrightness(int value){
  brightness = value;
  EEPROM.write(EEPROM_ADDR_BRIGHTNESS, value);
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
  
  sendI2C(0x00, 1); 
  sendI2C(0x4B, 0); //frequency setting  0: 3kHz / 1: 22kHz
  sendI2C(0x4A, 0);
}

void sendI2C(int address, int data){
  Wire.beginTransmission(0b00111100);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission(true);
}

void copyDisplay(){
  for(byte y=0; y<MAX; y++){
    for(byte x=0; x<MAX; x++){
      lastDisplay[y][x] = display[y][x];
    }
  }
}

void clearDisplay(){
  for(byte y=0; y<MAX; y++){
    for(byte x=0; x<MAX; x++){
      display[y][x] = false;
    }
  }
}

void setDisplay(byte y, byte x){
  display[y][x] = true;
}

void showDisplay(){
  for(byte y=0; y<MAX; y++){
    for(byte x=0; x<MAX; x++){
      if(display[y][x]){
        sendI2C(pwmAddress[y][x]+0x25, 1);            //led register
        sendI2C(pwmAddress[y][x], PWM_Gamma64[brightness]);     //pwm register
      }
      else{
        sendI2C(pwmAddress[y][x]+0x25, 0);
        sendI2C(pwmAddress[y][x], 0);
      }
    }
  }
  sendI2C(0x25, 0);   //update setting
}

void ntpConnect(){  
  AsyncWiFiManager wifiManager(&server,&dns);
  //wifiManager.resetSettings();
  wifiManager.autoConnect(ssidAutoConnect);
  timeClient.begin();
  
  wifiConnectOK = true;
}

void serverInit(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssidAP, passwordAP);
  Serial.print(F("\n\nIP : "));
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello, world");
  });

  // Send a GET request to <IP>/get?message=<message>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if(request->hasParam("get_settings")){
      message = String(displayMode);
      message += "," + String(brightness);
    }
    else if(request->hasParam("set_display_mode")){
      setDisplayMode(request->getParam("set_display_mode")->value().toInt());
      message = "success";
    }
    else if(request->hasParam("set_brightness")){
      setBrightness(request->getParam("set_brightness")->value().toInt());
      message = "success";
    }
    else {
      message = "No message sent";
      Serial.println(message);
    }

    request->send(200, "text/plain", message);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });
  server.begin();
}

byte isTimeChanged(){
  timeClient.update();
  static int prevSec = timeClient.getSeconds();
  
  int currSec = timeClient.getSeconds();
  if(prevSec != currSec){
    prevSec = currSec;
    return currSec==0? TIME_CHANGE_MINUTE : TIME_CHANGE_SECOND;
  }
  return TIME_CHANGE_NULL;
}

int getTime(){
  return timeClient.getHours()*100 + timeClient.getMinutes();
}

void updateDisplay(){
  int hour=timeClient.getHours();
  int minute=timeClient.getMinutes();
  int minute10 = minute / 10;
  int minute1 = minute % 10;
  int second=timeClient.getSeconds();
  
  copyDisplay();
  clearDisplay();

#if defined(DISPLAY_TYPE_HORIZONTAL)
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
  
#else
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
#endif
}

void fade(){
  int gamma;
  for(float t=1; t>=0; t-=0.01){
    gamma = t*brightness;
    for(byte y=0; y<MAX; y++)for(byte x=0; x<MAX; x++){ 
      if(lastDisplay[y][x]){
        sendI2C(pwmAddress[y][x]+0x25, 1);            //led register
        sendI2C(pwmAddress[y][x], PWM_Gamma64[gamma]);     //pwm register
      }
      else{
        sendI2C(pwmAddress[y][x]+0x25, 0);
        sendI2C(pwmAddress[y][x], 0);
      }
    }
    sendI2C(0x25, 0);   //update setting
    delay(10);
  }
  
  for(float t=0; t<1.009; t+=0.01){
    gamma = t*brightness;
    for(byte y=0; y<MAX; y++)for(byte x=0; x<MAX; x++){ 
      if(display[y][x]){
        sendI2C(pwmAddress[y][x]+0x25, 1);            //led register
        sendI2C(pwmAddress[y][x], PWM_Gamma64[gamma]);     //pwm register
      }
      else{
        sendI2C(pwmAddress[y][x]+0x25, 0);
        sendI2C(pwmAddress[y][x], 0);
      }
    }
    sendI2C(0x25, 0);   //update setting
    delay(10);
  }
}

void test(){
  i2cInit();
  while(true){
    for(int i=0; i<64; i++){
      sendI2C(pwmAddress[2][2]+0x25, 1);            //led register
      sendI2C(pwmAddress[2][2], PWM_Gamma64[i]);     //pwm register
      sendI2C(0x25, 0);   //update setting
      delay(25);
    }
    for(int i=63; i>0; i--){
      sendI2C(pwmAddress[2][2]+0x25, 1);            //led register
      sendI2C(pwmAddress[2][2], PWM_Gamma64[i]);     //pwm register
      sendI2C(0x25, 0);   //update setting
      delay(25);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  Serial.begin(115200);
  
  //test();
  ntpConnect();
  serverInit();
  i2cInit();
  readSettings();
}

void loop() {
  delay(8);
  byte whichChanged = isTimeChanged();
  if(whichChanged == TIME_CHANGE_NULL) return;
  updateDisplay();

  switch(displayMode){
    case DISPLAY_MODE_STANDARD:
      break;
    case DISPLAY_MODE_FADE:
      if(whichChanged == TIME_CHANGE_MINUTE){
        fade();
      }
      break;
  }

  showDisplay();
}
