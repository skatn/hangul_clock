#include <ESP8266WebServer.h>
#include <AutoConnect.h>

ESP8266WebServer Server;
AutoConnect Portal(Server);

void serverInit(ESP8266WebServer& Server_);

void rootPage(){
  char content[] = "Hello, World!";
  Server.send(200, "text/plain", content);
}

void wifiConnect(){
//  Server.on("/", rootPage);
  serverInit(Server);
  Portal.begin();
  Serial.println("Web server started: " + WiFi.localIP().toString());
}

void handleClient(){
  Portal.handleClient();
}
