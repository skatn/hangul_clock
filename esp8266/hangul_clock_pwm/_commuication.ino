#include "html.h"

void serverInit(ESP8266WebServer& Server_){
  static ESP8266WebServer& Server = Server_;
  
  Server.on("/", [](){
    Serial.println("/");
    Server.send(200, "text/html", index_html);
  });

  Server.on("/get_settings", [](){
    Serial.println("/get_settings");
    String message = String(getDisplayMode());
    message += "," + String(getBrightness());
    message += "," + String(getGmt());
    Server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    Server.send(200, "text/plain", message);
  });

  Server.on("/set_display_mode", [](){
    Serial.println("/set_display_mode");
    if(Server.hasArg("mode") == false){
        Server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
      Server.send(200, "text/plain", "ERR_REQUEST");
      return;
    }
    setDisplayMode(Server.arg("mode").toInt());
    
    Server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    Server.send(200, "text/plain", "SUCCESS");
  });
  
  Server.on("/set_brightness", [](){
    Serial.println("/set_brightness");
    if(Server.hasArg("brightness") == false){
    Server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
      Server.send(200, "text/plain", "ERR_REQUEST");
      return;
    }
    setBrightness(Server.arg("brightness").toInt());
    Server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    Server.send(200, "text/plain", "SUCCESS");
  });
  
  Server.on("/set_gmt", [](){
    Serial.println("/set_gmt");
    if(Server.hasArg("gmt") == false){
    Server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
      Server.send(200, "text/plain", "ERR_REQUEST");
      return;
    }
    setGmt(Server.arg("gmt").toInt());
    Server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    Server.send(200, "text/plain", "SUCCESS");
  });
}
