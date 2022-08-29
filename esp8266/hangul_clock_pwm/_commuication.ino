#include "html.h"

void myResponse(AsyncWebServerRequest* request, int code, String contentType, String content);

void myResponse(AsyncWebServerRequest* request, int code, String contentType, String content){
  AsyncWebServerResponse *response_ = request->beginResponse(code, contentType, content);
  response_->addHeader(F("Access-Control-Allow-Origin"), F("*"));
  request->send(response_);
}

void serverInit(AsyncWebServer& server_){
  static AsyncWebServer& server = server_;
  
  server.on("/", [](AsyncWebServerRequest* request){
    Serial.println("/");
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get_settings", [](AsyncWebServerRequest* request){
    Serial.println("/get_settings");
    String message = String(getDisplayMode());
    message += "," + String(getBrightness());
    message += "," + String(getGmt());

    myResponse(request, 200, "text/plain", message);
  });

  server.on("/set_display_mode", [](AsyncWebServerRequest* request){
    Serial.println("/set_display_mode");
    if(request->hasParam("mode") == false){
      myResponse(request, 200, "text/plain", "ERR_REQUEST");
      return;
    }
    setDisplayMode(request->getParam("mode")->value().toInt());
    myResponse(request, 200, "text/plain", "SUCCESS");
  });
  
  server.on("/set_brightness", [](AsyncWebServerRequest* request){
    Serial.println("/set_brightness");
    if(request->hasParam("brightness") == false){
      myResponse(request, 200, "text/plain", "ERR_REQUEST");
      return;
    }
    setBrightness(request->getParam("brightness")->value().toInt());
    myResponse(request, 200, "text/plain", "SUCCESS");
  });
  
  server.on("/set_gmt", [](AsyncWebServerRequest* request){
    Serial.println("/set_gmt");
    if(request->hasParam("gmt") == false){
      myResponse(request, 200, "text/plain", "ERR_REQUEST");
      return;
    }
    setGmt(request->getParam("gmt")->value().toInt());
    myResponse(request, 200, "text/plain", "SUCCESS");
  });

  server.onNotFound([](AsyncWebServerRequest* request){
    request->send(404, "text/plain", "404 Not found");
  });
}
