#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define EEPROM_SIZE 512
#define EEPROM_ADDR_DISPLAYMODE 0
#define EEPROM_ADDR_BRIGHTNESS  1
#define EEPROM_ADDR_TIMEZONE    2

#define LOOP_TIME 8
unsigned long lastMillis = 0;

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  
  test();
  mainSetup();
}

void loop() {
  handleClient();

  if(millis() - lastMillis > LOOP_TIME){
    mainLoop();
    lastMillis = millis();
  }
}
