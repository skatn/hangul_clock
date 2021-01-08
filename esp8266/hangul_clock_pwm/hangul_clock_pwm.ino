#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESPAsyncWiFiManager.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>

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

byte display[MAX][MAX];
byte lastDisplay[MAX][MAX];
byte pwmAddress[MAX][MAX]={             //led Control address = pinsAddress + 0x25
  0x1C, 0x1F, 0x22, 0x01, 0x04, 0x07,
  0x1D, 0x20, 0x23, 0x02, 0x05, 0x08,
  0x1E, 0x21, 0x24, 0x03, 0x06, 0x09,
  0x1B, 0x18, 0x15, 0x10, 0x0D, 0x0A,
  0x1A, 0x17, 0x14, 0x11, 0x0E, 0x0B,
  0x19, 0x16, 0x13, 0x12, 0x0F, 0x0C
};
int brightness = 255;

void i2cInit(){
  sendI2C(0x4B, 1);
}

void sendI2C(int address, int data){
  Wire.beginTransmission(address);
  Wire.write(data);
  Wire.endTransmission(true);
}

void copyDisplay(byte (*from)[MAX], byte (*to)[MAX]){
  for(byte y=0; y<MAX; y++){
    for(byte x=0; x<MAX; x++){
      to[y][x] = from[y][x];
    }
  }
}

void clearDisplay(){
  for(byte y=0; y<MAX; y++){
    for(byte x=0; x<MAX; x++){
      display[y][x] = 0;
    }
  }
}

void setDisplay(byte y, byte x, byte brightness){
  display[y][x] = brightness;
}

void showDisplay(byte (*display)[MAX]){
  for(byte y=0; y<MAX; y++){
    for(byte x=0; x<MAX; x++){
      if(display[y][x]){
        sendI2C(pwmAddress[y][x]+0x25, 1);            //led register
        sendI2C(pwmAddress[y][x], display[y][x]);     //pwm register
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
    if(request->hasParam("set_bright_mode")){
      //brightMode = request->getParam("set_bright_mode")->value().toInt();
      message = "success";
    }
    else {
      message = "No message sent";
    }
    
    request->send(200, "text/plain", "Hello, GET: " + message);
  });
  
  /*
  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    String message;
    if(request->hasParam("set_bright_mode", true)){
      //String data = request->getParam("set_bright_mode", true)->value();
      brightMode = request->getParam("set_bright_mode", true)->value().toInt();
      message = "success";
    }
    else {
      message = "No message sent";
    }
    request->send(200, "text/plain", message);
  });*/
  
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });
  server.begin();
}

bool isTimeChanged(){
  static int prevSec = 0;
  timeClient.update();

  int currSec = timeClient.getSeconds();
  if(prevSec != currSec){
    prevSec = currSec;
    return true;
  }
  return false;
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
  
  copyDisplay(display, lastDisplay);
  clearDisplay();
  
  if(second < 5){
    setDisplay(3, 0, brightness);
    if (hour < 12)
      setDisplay(4, 0, brightness);
    else 
      setDisplay(5, 0, brightness);
  }

  setDisplay(2, 5, brightness);
  if(hour > 12) hour -= 12;
  switch (hour) {
    case 1:  setDisplay(0, 0, brightness);                                break;
    case 2:  setDisplay(0, 1, brightness);                                break;
    case 3:  setDisplay(0, 2, brightness);                                break;
    case 4:  setDisplay(0, 3, brightness);                                break;
    case 5:  setDisplay(0, 4, brightness); setDisplay(0, 5, brightness);  break;
    case 6:  setDisplay(1, 0, brightness); setDisplay(1, 1, brightness);  break;
    case 7:  setDisplay(1, 2, brightness); setDisplay(1, 3, brightness);  break;
    case 8:  setDisplay(1, 4, brightness); setDisplay(1, 5, brightness);  break;
    case 9:  setDisplay(2, 0, brightness); setDisplay(2, 1, brightness);  break;
    case 10: setDisplay(2, 2, brightness);                                break;
    case 11: setDisplay(2, 2, brightness); setDisplay(2, 3, brightness);  break;
    case 0:
    case 12: setDisplay(2, 2, brightness); setDisplay(2, 4, brightness);  break;
  }

  if (minute > 0)setDisplay(5, 5, brightness);
  if (minute10 > 0) {
    setDisplay(3, 5, brightness);
    if (minute10 > 1)setDisplay(3, minute10 - 1, brightness);
  }
  if (minute1 > 0) {
    if (minute1 < 6)setDisplay(4, minute1, brightness);
    else setDisplay(5, minute1 - 5, brightness);
  }
}

void smoothMove(){
  byte animeDisplay[MAX][MAX];
  copyDisplay(display, animeDisplay);
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  ntpConnect();
  serverInit();
  Wire.begin(4, 5);
  i2cInit();
}

void loop() {
  if(isTimeChanged()){
    updateDisplay();
  }

  showDisplay(display);
  delay(10);
}
