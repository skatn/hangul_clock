#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include "RTClib.h"

#define BRIGHTNESS_ADDRESS  0
#define EFFECT_ADDRESS      1

#define MENU_BTN    7
#define UP_BTN      8
#define DOWN_BTN    9

#define NEO_PIN         3
#define BRIGHTNESS_STEP 20
#define DEFAULT_BRIGHTNESS 180
#define BRIGHTNESS_MIN  35
#define BRIGHTNESS_MAX  255

extern volatile unsigned long timer0_millis;

#define RAINBOW       0
#define RED           1
#define BLUE          2
#define GREEN         3
#define YELLOW_GREEN  4
#define PURPLE        5
#define PINK          6
#define YELLOW        7
#define SKY           8
#define WHITE         9
#define EFFECT_COUNT  10

RTC_DS1307 rtc;
DateTime nowTime;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(37, NEO_PIN, NEO_GRB + NEO_KHZ800); //mode 1 + clock 36
const uint8_t ledMatrix[36] PROGMEM = {
  6,  5,  4,  3,  2,  1,
  7,  8,  9,  10, 11, 12,
  18, 17, 16, 15, 14, 13,
  19, 20, 21, 22, 23, 24,
  30, 29, 28, 27, 26, 25,
  31, 32, 33, 34, 35, 36
};

typedef struct _led {
  uint8_t row;
  uint8_t col;
  uint32_t color;
} Led;

Led leds[10];
uint8_t ledCnt = 0;

#define effectUpdateTime 120
uint8_t effect = RAINBOW;
int brightness = 180;

boolean pMenu, pUp, pDown, menu, up, down, timeChanged;

typedef enum {
  TIME_UPDATE, SET_MODE, SET_BRIGHT, SET_HOUR, SET_MINUTE, CONFIG_COUNT
}Config;
Config config = TIME_UPDATE;

void setup() {
  if (!rtc.begin()) {
    while (1);
  }
  //rtc.adjust(DateTime(__DATE__, __TIME__)); //연결된 PC 시간이랑 연동, 업로드시 주석해제후 1회 업로드한뒤 주석하고 다시 1회 업로드
  
  brightness = EEPROM.read(BRIGHTNESS_ADDRESS);
  if(brightness < BRIGHTNESS_MIN){
    brightness = DEFAULT_BRIGHTNESS;
    EEPROM.update(BRIGHTNESS_ADDRESS, brightness);
  }

  effect = EEPROM.read(EFFECT_ADDRESS);
  if(effect >= EFFECT_COUNT){
    effect = RAINBOW;
    EEPROM.update(EFFECT_ADDRESS, effect);
  }
  
  strip.begin();
  strip.setBrightness(brightness);              //min : 30    max : 255
  strip.show();

  pinMode(MENU_BTN, INPUT_PULLUP);
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
}

void loop() {
  selectConfig();
  delay(5);

  updateClock();
  delay(5);
}

void timeUpdate() {
  if (timeChanged == false) return;
  timeChanged = false;
  rtc.adjust(DateTime(nowTime.year(), nowTime.month(), nowTime.day(), nowTime.hour(), nowTime.minute(), 0));
}

void selectConfig() {
  menu = !digitalRead(MENU_BTN);
  up = !digitalRead(UP_BTN);
  down = !digitalRead(DOWN_BTN);
  
  if (menu && !pMenu) {
    config = (Config)(config+1);
    if (config >= CONFIG_COUNT)config = TIME_UPDATE;
  }

  switch (config) {
    case TIME_UPDATE: timeUpdate(); strip.setPixelColor(0, 0x000000); strip.show(); break;
    case SET_MODE:    setMode();    strip.setPixelColor(0, 0xFF0000); strip.show(); break;
    case SET_BRIGHT:  setBright();  strip.setPixelColor(0, 0xFF9900); strip.show(); break;
    case SET_HOUR:    setHour();    strip.setPixelColor(0, 0x00FF00); strip.show(); break;
    case SET_MINUTE:  setMinute();  strip.setPixelColor(0, 0x0000FF); strip.show(); break;
  }

  pMenu = menu;
  pUp = up;
  pDown = down;
}

void setMode() {
  if (up && !pUp) {
    if (effect == EFFECT_COUNT - 1) effect = RAINBOW;
    else effect++;
    EEPROM.update(EFFECT_ADDRESS, effect);
  }
  else if (down && !pDown) {
    if (effect == RAINBOW) effect = EFFECT_COUNT - 1;
    else effect--;
    EEPROM.update(EFFECT_ADDRESS, effect);
  }
}

void setBright() {
  if (up && !pUp) {
    brightness += BRIGHTNESS_STEP;
    if (brightness > BRIGHTNESS_MAX) brightness = BRIGHTNESS_MAX;
    strip.setBrightness(brightness);
    EEPROM.update(BRIGHTNESS_ADDRESS, brightness);
  }
  else if (down && !pDown) {
    brightness -= BRIGHTNESS_STEP;
    if (brightness < BRIGHTNESS_MIN) brightness = BRIGHTNESS_MIN;
    strip.setBrightness(brightness);
    EEPROM.update(BRIGHTNESS_ADDRESS, brightness);
  }
}

void setHour() {
  if (up && !pUp) {
    timeChanged = true;
    if(nowTime.hour()%24==23) rtc.adjust(nowTime-TimeSpan(0, 23, 0, 0));
    else rtc.adjust(nowTime+TimeSpan(0, 1, 0, 0));
  }
  else if (down && !pDown) {
    timeChanged = true;
    if(nowTime.hour()%24==0) rtc.adjust(nowTime+TimeSpan(0, 23, 0, 0));
    else rtc.adjust(nowTime-TimeSpan(0, 1, 0, 0));
  }
}

void setMinute() {
  if (up && !pUp) {
    timeChanged = true;
    if(nowTime.minute()==59) rtc.adjust(nowTime-TimeSpan(0, 0, 59, 0));
    else rtc.adjust(nowTime+TimeSpan(0, 0, 1, 0));
  }
  else if (down && !pDown) {
    timeChanged = true;
    if(nowTime.minute()==0) rtc.adjust(nowTime+TimeSpan(0, 0, 59, 0));
    else rtc.adjust(nowTime-TimeSpan(0, 0, 1, 0));
  }
}

void updateClock() {
  nowTime = rtc.now();
  uint8_t hour = nowTime.hour()%24;
  uint8_t minute = nowTime.minute();
  uint8_t minute10 = minute / 10;
  uint8_t minute1 = minute % 10;

  if((nowTime.second() < 5) || ((config == SET_HOUR) || (config == SET_MINUTE))){
    ledOn(3, 0);
    if (hour < 12)
      ledOn(4, 0);
    else 
      ledOn(5, 0);
  }

  ledOn(2, 5);
  if(hour > 12) hour -= 12;
  switch (hour) {
    case 1:  ledOn(0, 0);              break;
    case 2:  ledOn(0, 1);              break;
    case 3:  ledOn(0, 2);              break;
    case 4:  ledOn(0, 3);              break;
    case 5:  ledOn(0, 4); ledOn(0, 5); break;
    case 6:  ledOn(1, 0); ledOn(1, 1); break;
    case 7:  ledOn(1, 2); ledOn(1, 3); break;
    case 8:  ledOn(1, 4); ledOn(1, 5); break;
    case 9:  ledOn(2, 0); ledOn(2, 1); break;
    case 10: ledOn(2, 2);              break;
    case 11: ledOn(2, 2); ledOn(2, 3); break;
    case 0:
    case 12: ledOn(2, 2); ledOn(2, 4); break;
  }

  if (minute > 0)ledOn(5, 5);
  if (minute10) {
    ledOn(3, 5);
    if (minute10 > 1)ledOn(3, minute10 - 1);
  }
  if (minute1) {
    if (minute1 < 6)ledOn(4, minute1);
    else ledOn(5, minute1 - 5);
  }
  
  showTime();
}

void ledOn(uint8_t row, uint8_t col) {
  leds[ledCnt].row = row;
  leds[ledCnt].col = col;
  ledCnt++;
}

void showTime() {
  static uint8_t color = 0;
  static uint16_t effectTime = millis(), timeChangeBlinkTime = 0;
  static boolean stripState = false;
  for(uint8_t i=1; i<37; i++) strip.setPixelColor(i, 0);

  uint16_t currTime = millis();

  if (currTime - timeChangeBlinkTime > 500) {
    timeChangeBlinkTime = currTime;
    stripState = !stripState;
  }

  for (register uint8_t i = 0; i < ledCnt; i++) {
    if ((config == SET_HOUR) || (config == SET_MINUTE)) {
      if (config == SET_MINUTE) {
        if((leds[i].row < 3) || (leds[i].col < 1))leds[i].color = 0xFFFFFF;
        else leds[i].color = stripState ? 0 : 0xFFFFFF;
      }
      else if (config == SET_HOUR) {
        if(leds[i].row > 2) leds[i].color = 0xFFFFFF;
        else leds[i].color = stripState ? 0 : 0xFFFFFF;
      }
    }
    else {
      switch (effect) {
        case RAINBOW:       leds[i].color = wheel(color);         break;
        case RED:           leds[i].color = 0xFF0000;             break;
        case BLUE:          leds[i].color = 0x0000FF;             break;
        case GREEN:         leds[i].color = 0x00FF00;             break;
        case YELLOW_GREEN:  leds[i].color = 0x57A639;             break;
        case PURPLE:        leds[i].color = 0x6F00CC;             break;
        case PINK:          leds[i].color = 0xFF3399;             break;
        case YELLOW:        leds[i].color = 0xFF9900;             break;
        case SKY:           leds[i].color = 0x00FFFF;             break;
        case WHITE:         leds[i].color = 0xFFFFFF;             break;
      }
    }
    strip.setPixelColor(pgm_read_byte(ledMatrix+leds[i].row*6+leds[i].col), leds[i].color);
  }

  strip.show();
  for (uint8_t i = 0; i < ledCnt; i++) leds[i].row = leds[i].col = leds[i].color = 0;
  ledCnt = 0;


  if ((currTime - effectTime) > effectUpdateTime) {
    effectTime = currTime;
    color++;
  }
  if (currTime > 60000) timer0_millis = effectTime = timeChangeBlinkTime = 0;
}

uint32_t wheel(uint8_t n) {
  if (n < 85) {
    return strip.Color(n * 3, 255 - n * 3, 0);
  } else if (n < 170) {
    n -= 85;
    return strip.Color(255 - n * 3, 0, n * 3);
  } else {
    n -= 170;
    return strip.Color(0, n * 3, 255 - n * 3);
  }
}
