#include <ESP8266WiFi.h>
#include <ESPAsyncWiFiManager.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssidAutoConnect = "SEMI_AUTO";
const char* ssidAP = "SEMI_Hangul_Clock";
const char* passwordAP = "1234567890";
bool wifiConnectOK = false;

byte brightness = 32;    //0~64
byte displayMode = 0;

AsyncWebServer server(80);
DNSServer dns;

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
      Serial.println("get_settings");
      Serial.println(message);
    }
    else if(request->hasParam("set_display_mode")){
      displayMode = request->getParam("set_display_mode")->value().toInt();
      message = "success";
      Serial.println("set_display_mode");
      Serial.println(displayMode);
    }
    else if(request->hasParam("set_brightness")){
      brightness = request->getParam("set_brightness")->value().toInt();
      message = "success";
      Serial.println("set_brightness");
      Serial.println(brightness);
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

void setup() {
  delay(1000);
  Serial.begin(115200);
  serverInit();
}

void loop() {
}
