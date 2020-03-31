#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <RTClib.h>

#define MENU_BTN    7
#define UP_BTN      8
#define DOWN_BTN    9

#define NEO         3

extern volatile unsigned long timer0_millis;

enum {
  RAINBOW,
  RAINBOW_CYCLE,
  RED,
  BLUE,
  GREEN,
  PURPLE,
  PINK,
  YELLOW,
  SKY,
  WHITE,
  NUM_OF_EFFECT
};

RTC_DS1307 RTC;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(37, NEO, NEO_GRB + NEO_KHZ800); //mode 1 + clock 36
uint8_t ledMatrix[6][6] = {
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

Led leds[37];
uint8_t ledCnt = 0;

uint8_t effect = RAINBOW;
uint8_t bufH = 255, bufM = 255;
uint16_t effectUpdateTime[NUM_OF_EFFECT];

boolean pMenu, pUp, pDown, menu, up, down;

void setup() {
  Serial.begin(9600);

  Wire.begin();
  if (!RTC.begin()) {
   // while (1);
  }
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__)); //연결된 PC 시간이랑 연동
    //RTC.adjust(DateTime(2020, 1, 1, 10, 0));
  }

  strip.begin();
  strip.setBrightness(180);               //min : 30    max : 255
  strip.show();

  effectUpdateTime[RAINBOW] = 120;         //ms
  effectUpdateTime[RAINBOW_CYCLE] = 120;   //ms

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
  if (bufM > 250) return;
  DateTime now = RTC.now();
  int y = now.year();
  int m = now.month();
  int d = now.day();
  RTC.adjust(DateTime(y, m, d, bufH, bufM, 0));
  bufH = bufM = 255;
}

void selectConfig() {
  static uint8_t select = 0;

  menu = !digitalRead(MENU_BTN);
  up = !digitalRead(UP_BTN);
  down = !digitalRead(DOWN_BTN);
  
  if (menu && !pMenu) {
    select++;
    if (select > 4)select = 0;
  }

  switch (select) {
    case 0: timeUpdate(); strip.setPixelColor(0, 0x000000); strip.show(); break;
    case 1: setMode();    strip.setPixelColor(0, 0xFF0000); strip.show(); break;
    case 2: setBright();  strip.setPixelColor(0, 0xFF9900); strip.show(); break;
    case 3: setHour();    strip.setPixelColor(0, 0x00FF00); strip.show(); break;
    case 4: setMinute();  strip.setPixelColor(0, 0x0000FF); strip.show(); break;
  }

  pMenu = menu;
  pUp = up;
  pDown = down;
}

void setMode() {
  if (up && !pUp) {
    if (effect == NUM_OF_EFFECT - 1) effect = RAINBOW;
    else effect++;
  }
  else if (down && !pDown) {
    if (effect == RAINBOW) effect = NUM_OF_EFFECT - 1;
    else effect--;
  }
}

void setBright() {
  if (up && !pUp) {
    int brightness = strip.getBrightness() + 20;
    if (brightness > 255) brightness = 255;
    strip.setBrightness(brightness);
  }
  else if (down && !pDown) {
    int brightness = strip.getBrightness() - 20;
    if (brightness < 30) brightness = 30;
    strip.setBrightness(brightness);
  }
}

void setHour() {
  if (bufH > 250) {
    DateTime now = RTC.now();
    bufH = now.hour();
  }
  
  if (up && !pUp) {
    bufH++;
    if (bufH > 24) bufH = 1;
  }
  else if (down && !pDown) {
    bufH--;
    if (bufH < 1) bufH = 24;
  }
}

void setMinute() {
  if (bufM >250) {
    DateTime now = RTC.now();
    bufM = now.minute();
  }

  if (up && !pUp) {
    bufM++;
    if (bufM > 59) bufM = 0;
  }
  else if (down && !pDown) {
    bufM--;
    if (bufM < 0) bufM = 59;
  }
}

void updateClock() {
  DateTime now = RTC.now();
  int hour = bufH > 250 ? now.hour() : bufH;
  int minute = bufM > 250 ? now.minute() : bufM;
  int minute10 = minute / 10;
  int minute1 = minute % 10;

  ledOn(3, 0);
  if (hour < 13) {
    ledOn(4, 0);
  }
  else {
    ledOn(5, 0);
    hour -= 12;
  }

  ledOn(2, 5);
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
    case 12: ledOn(2, 2); ledOn(2, 4); break;
  }

  if (minute > 0)ledOn(5, 5);
  if (minute10) {
    ledOn(3, 5);
    if (minute10 > 1)ledOn(3, minute10 - 1);
  }
  if (minute1) {
    //ledOn(4, 5);
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
  static uint8_t color[NUM_OF_EFFECT] = {};
  static uint16_t effectTime = millis();
  static uint16_t timeSetBlinkTime = 0;
  static boolean state = false;
  for(uint16_t i=1; i<37; i++) strip.setPixelColor(i, 0);

  unsigned long currTime = millis();

  if (currTime - timeSetBlinkTime > 500) {
    timeSetBlinkTime = currTime;
    state = !state;
  }

  for (int i = 0; i < ledCnt; i++) {
    if ((bufH < 250) || (bufM < 250)) {
      if (bufM < 250) {
        if((leds[i].row < 3) || (leds[i].col < 1))leds[i].color = 0xFFFFFF;
        else leds[i].color = state ? 0 : 0xFFFFFF;
      }
      else if ((bufH < 250)) {
        if(leds[i].row > 2) leds[i].color = 0xFFFFFF;
        else leds[i].color = state ? 0 : 0xFFFFFF;
      }
    }
    else {
      switch (effect) {
        case RAINBOW:
          leds[i].color = wheel(color[RAINBOW]);
          break;
        case RAINBOW_CYCLE:
          leds[i].color = wheel(((i * 256 / ledCnt) + color[RAINBOW_CYCLE]) & 255);
          break;
        case RED:
          leds[i].color = 0xFF0000;
          break;
        case BLUE:
          leds[i].color = 0x0000FF;
          break;
        case GREEN:
          leds[i].color = 0x00FF00;
          break;
        case PURPLE:
          //leds[i].color=0x0B00FF;   //원본
          leds[i].color = 0x6F00CC; //25%어둡게
          break;
        case PINK:
          leds[i].color = 0xFF3399;
          break;
        case YELLOW:
          leds[i].color = 0xFF9900;
          break;
        case SKY:
          leds[i].color = 0x00FFFF;
          break;
        case WHITE:
          leds[i].color = 0xFFFFFF;
          break;
      }
    }

    strip.setPixelColor(ledMatrix[leds[i].row][leds[i].col], leds[i].color);
  }

  strip.show();
  for (int i = 0; i < ledCnt; i++) leds[i].row = leds[i].col = leds[i].color = 0;
  ledCnt = 0;


  if ((currTime - effectTime) > effectUpdateTime[effect]) {
    effectTime = currTime;
    switch (effect) {
      case RAINBOW: color[RAINBOW]++; break;
      case RAINBOW_CYCLE: color[RAINBOW_CYCLE]++; if (color[RAINBOW_CYCLE] >= 256 * 5)color[RAINBOW_CYCLE] = 0; break;
    }
  }
  if (currTime > 60000) timer0_millis = effectTime = timeSetBlinkTime = 0;
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
