#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESPAsyncWiFiManager.h>

const char* autoConnectName = "한글시계 WiFi설정";
const char* autoConnectPw = "1234567890";
const char* ssidAP = "한글시계 설정";
const char* passwordAP = "1234567890";

AsyncWebServer server(80);
DNSServer dns;

void serverInit(AsyncWebServer& server_);

void wifiConnect(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssidAP, passwordAP);
  
  AsyncWiFiManager wifiManager(&server, &dns);
//  wifiManager.setWiFiAutoReconnect(true);
  wifiManager.autoConnect(autoConnectName, autoConnectPw);

  if(!MDNS.begin("hgclock")){
    Serial.println("Error setting up MDNS responder!");
  }

  serverInit(server);
  server.begin();
}


void handleClient(){
//  server.handleClient();
}
