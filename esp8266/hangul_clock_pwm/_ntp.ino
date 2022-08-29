#include <NTPClient.h>
#include <WiFiUdp.h>

#define GMT2SEC(x) (x*3600)

#define NTP_UPDATE_INTERVAL  3600000 //60m*60s*1000ms
#define TIME_CHANGE_NULL    0
#define TIME_CHANGE_SECOND  1
#define TIME_CHANGE_MINUTE  2

int rawTime = 0, currTime = 0;
char currentGmt = 9;    //GMT+9 한국시간

WiFiUDP udp;
NTPClient timeClient(udp, "kr.pool.ntp.org", GMT2SEC(currentGmt), NTP_UPDATE_INTERVAL);

void ntpInit(){
  int gmt = EEPROM.read(EEPROM_ADDR_TIMEZONE);
  setGmt(gmt);
  
  timeClient.begin();
}

void setGmt(int gmt){
  if(gmt < -11 || gmt > 12) gmt = 9;
  currentGmt = gmt;
  timeClient.setTimeOffset(GMT2SEC(gmt));
  
  EEPROM.write(EEPROM_ADDR_TIMEZONE, (char)gmt);
  EEPROM.commit();
}

byte getGmt(){
  return currentGmt;
}

byte getTimeChangeState(){
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

int getHourFromNtp(){
  return timeClient.getHours();
}

int getMinutesFromNtp(){
  return timeClient.getMinutes();
}

int getSecondsFromNtp(){
  return timeClient.getSeconds();
}
