#include <ESP8266WiFi.h>

#define LOOP_TIME 8
unsigned long lastMillis = 0;

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  Serial.begin(115200);
  
  mainSetup();
}

void loop() {
  handleClient();

  if(millis() - lastMillis > LOOP_TIME){
    mainLoop();
    lastMillis = millis();
  }
}
