#include <NTPClient.h>
#include <WiFiUdp.h>

#define NTP_UPDATE_INTERVAL  3600000 //60m*60s*1000ms
#define TIME_CHANGE_NULL    0
#define TIME_CHANGE_SECOND  1
#define TIME_CHANGE_MINUTE  2

int rawTime = 0, currTime = 0;
long timeOffset = 32400;

WiFiUDP udp;
NTPClient timeClient(udp, "kr.pool.ntp.org", timeOffset, NTP_UPDATE_INTERVAL);

void ntpConnect(){
  timeClient.begin();
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
