#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>

const char* autoConnectName = "한글시계 WiFi설정";
const char* autoConnectPw = "1234567890";
const char* ssidAP = "한글시계 설정";
const char* passwordAP = "1234567890";

ESP8266WebServer Server;

void serverInit(ESP8266WebServer& Server_);

void rootPage(){
  char content[] = "Hello, World!";
  Server.send(200, "text/plain", content);
}

void wifiConnect(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssidAP, passwordAP);
  
  WiFiManager wifiManager;
  wifiManager.setWiFiAutoReconnect(true);
  wifiManager.autoConnect(autoConnectName, autoConnectPw);

  if(!MDNS.begin("hgclock")){
    Serial.println("Error setting up MDNS responder!");
  }

  serverInit(Server);
  Server.begin();
}


void handleClient(){
  Server.handleClient();
}
