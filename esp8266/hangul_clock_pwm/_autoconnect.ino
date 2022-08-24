#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <AutoConnect.h>

ESP8266WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig config;

void serverInit(ESP8266WebServer& Server_);

void rootPage(){
  char content[] = "Hello, World!";
  Server.send(200, "text/plain", content);
}

void wifiConnect(){
  Serial.println("wifiConnect..");
  serverInit(Server);

//  config.apip = 0x011CD9AC;
  config.preserveAPMode = true;
//  config.autoRise = false;
//  config.immediateStart = true;
  
  Portal.config(config);  
  Portal.begin();
  Serial.println("Web server started: " + WiFi.localIP().toString());
}

void handleClient(){
  Portal.handleClient();
}
