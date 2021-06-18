#include <MCUFRIEND_kbv.h>
#include <EEPROM.h>

#define LCD_CS A3
 #define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#include <TouchScreen.h>
MCUFRIEND_kbv tft;

#define ORIENTATION 1 //0 PORTRAIT 1 LANDSCAPE
#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define XP 8
#define XM A2
#define YP A3
#define YM 9

#define TS_LEFT 72
#define TS_RT 885
#define TS_TOP 944
#define TS_BOT 94

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_GFX_Button button_next, button_prev, incr_btn, decr_btn, save_btn, info_btn, back_btn, start_btn, settings_btn;

int pixel_x, pixel_y;
word pos = 0;
word minute = 0;
byte c;
char btnArr[3] = {'A', 'B', 'C'};
bool A, B, C, prevState, state, x, _home, btn;
word settings[3] = {1, 3, 15};
unsigned long prevMillis;

void setup() {
  pinMode(1, INPUT);//C
  pinMode(11, INPUT);//A
  pinMode(12, INPUT);//D
  pinMode(13, INPUT);//B
  pinMode(0, OUTPUT); //lampu led
  pinMode(10, OUTPUT); //Relay
  digitalWrite(1, LOW);
  digitalWrite(0, HIGH);
  tft.begin(tft.readID());
  tft.setRotation(1);
  for (byte i = 0; i < 3; i++) {
    settings[i] = EEPROM.read(i);
  }
  digitalWrite(0, LOW);
  delay(100);
  digitalWrite(0, HIGH);
  delay(100);
  digitalWrite(0, LOW);
  delay(100);
  digitalWrite(0, HIGH);
  delay(100);
  digitalWrite(0, LOW);
  delay(100);
  digitalWrite(0, HIGH);
  delay(100);
  Home();
}


void Save() {
  tft.fillScreen(BLACK);
  for (byte i = 0; i < 3; i++) {
    EEPROM.update(i, settings[i]);
  }
  showmsg(70, 110, 2, GREEN, "Settings Saved!");
  delay(2000);
  Settings();
}

bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.y, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    pixel_y = map(p.x, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

void showmsg(int x , int y, byte sz, uint16_t c, const char *msg) {
  tft.setCursor(x, y);
  tft.setTextSize(sz);
  tft.setTextColor(c);
  tft.print(msg);
  delay(50);
}

void showBtn() {
  (pos < 0) ? pos = 2 : (pos > 2) ? pos = 0 : 0;
  tft.fillRect(110, 70, 120, 14, BLACK);
  tft.fillRect(120, 130, 60, 14, BLACK);
  tft.setCursor(110, 70);
  tft.print("Tombol ");
  tft.print(btnArr[pos]);
  tft.setCursor((320 - 10 * String(settings[pos]).length()) / 2, 130);
  tft.print(settings[pos]);
  delay(300);
}

void Settings() {
  _home = false;
  tft.fillScreen(BLACK);
  back_btn.initButton(&tft, 40, 30, 60, 40, BLACK, DARKGREY, BLACK, "<-", 3);
  incr_btn.initButton(&tft, 220, 150 , 40, 40, BLUE, BLUE, WHITE, "+", 2);
  decr_btn.initButton(&tft, 100, 150 , 40, 40, BLUE, BLUE, WHITE, "-", 2);
  save_btn.initButton(&tft, 260, 220, 90, 40, OLIVE, OLIVE, NAVY, "Save", 2);
  info_btn.initButton(&tft, 60, 220, 90, 40, DARKGREY, DARKGREY, BLUE, "Info", 2);
  button_next.initButton(&tft, 270, 80 , 40, 40, BLUE, BLUE, WHITE, ">", 2);
  button_prev.initButton(&tft, 50, 80 , 40, 40, BLUE, BLUE, WHITE, "<", 2);
  incr_btn.drawButton();
  back_btn.drawButton();
  decr_btn.drawButton();
  save_btn.drawButton();
  info_btn.drawButton();
  button_next.drawButton();
  button_prev.drawButton();
  showmsg(130, 150, 2, GREEN, "menit");
  showBtn();
  while (true) {
    bool down = Touch_getXY();
    save_btn.press(down && save_btn.contains(pixel_x, pixel_y));
    back_btn.press(down && back_btn.contains(pixel_x, pixel_y));
    info_btn.press(down && info_btn.contains(pixel_x, pixel_y));
    decr_btn.press(down && decr_btn.contains(pixel_x, pixel_y));
    incr_btn.press(down && incr_btn.contains(pixel_x, pixel_y));
    button_next.press(down && button_next.contains(pixel_x, pixel_y));
    button_prev.press(down && button_prev.contains(pixel_x, pixel_y));
    (incr_btn.justPressed()) ? tambahKurang(1) : nope();
    (decr_btn.justPressed()) ? tambahKurang(0) : nope();
    if (info_btn.justPressed()) {
      Info();
      break;
    }
    if (digitalRead(10) == LOW) {
      (save_btn.justPressed()) ? Save() : nope();
    }
    if (button_next.justPressed()) {
      pos += 1;
      showBtn();
    }
    if (button_prev.justPressed()) {
      pos -= 1;
      showBtn();
    }
    if (back_btn.justPressed()) {
      Home();
      break;
    }
  }
}
void tambahKurang(bool t) {
  //tft.fillRect((tft.width() - 15 * String(i).length()) / 2, tft.height()/ 4, String(i).length() * 14, 14, BLACK);
  tft.fillRect(120, 130, 60, 14, BLACK);
  minute = settings[pos];
  if (t) {
    minute += 1;
  }
  else {
    minute -= 1;
  }
  if (minute > 120) {
    minute = 1;
  }
  else if (minute == 0) {
    minute = 120;
  }
  settings[pos] = minute;
  tft.setCursor((320 - 10 * String(settings[pos]).length()) / 2, 130);
  tft.print(settings[pos]);
  delay(120);
}

void Info() {
  tft.fillScreen(BLACK);
  _home = false;
  //back_btn.initButton(&tft, tft.width() * 1 / 8, tft.height() * 29 / 32, 60, 40, BLACK, DARKGREY, BLACK, "<-", 3);
  back_btn.initButton(&tft, 40, 218, 60, 40, BLACK, DARKGREY, BLACK, "<-", 3);
  back_btn.drawButton();
  //showmsg((tft.width() - 12 * 14) / 2, tft.height() * 2 / 5, 2, GREENYELLOW , "---DOMOTECH---");
  showmsg(33, 76, 3, GREENYELLOW , "---DOMOTECH---");
  //tft.drawFastHLine(0, tft.height() * 2 / 5 + 20, tft.width(), OLIVE);
  tft.drawFastHLine(0, 106, 320, OLIVE);
  //showmsg((tft.width() - 12 * 19) / 2 * ORIENTATION, tft.height() * 2 / 5 + 30 * 1, 2, WHITE, "Call Us 081807928888");
  showmsg(46, 126, 2, WHITE, "Call Us 081807928888");
  //tft.setCursor((tft.width() - 12 * 19) / 2 * ORIENTATION, tft.height() * 2 / 5 + 30 * 2);
  tft.setCursor(46, 156);
  tft.print("IG @domotechbymeraki");
  while (true) {
    bool down = Touch_getXY();
    back_btn.press(down && back_btn.contains(pixel_x, pixel_y));
    if (back_btn.justPressed()) {
      Settings();
      break;
    }
  }
}

void nope() {}

void Home() {
  _home = true;
  tft.fillScreen(BLACK);
  showmsg(52, 10, 2, GREENYELLOW, "UV Tower By DOMOTECH");
  tft.drawFastHLine(0, 10 + 20, tft.width(), OLIVE);
  if (state) {
    start_btn.initButton(&tft, 95, 140, 90, 40, RED, RED, BLACK, "Stop", 2);
  }
  else {
    start_btn.initButton(&tft, 95, 140, 90, 40, OLIVE, OLIVE, NAVY, "Start", 2);
  }
  settings_btn.initButton(&tft, 215, 140, 95, 40, DARKGREY, DARKGREY, BLUE, "Settings", 2);
  start_btn.drawButton();
  settings_btn.drawButton();
}

bool checkHome(bool y) {
  if (y) {
    if (_home) {
      tft.fillRect(55, 120, 90, 40, BLACK);
      start_btn.initButton(&tft, 95, 140, 90, 40, RED, RED, BLACK, "Stop", 2);
      start_btn.drawButton();
    }
  }
  else {
    if (_home) {
      tft.fillRect(55, 120, 90, 40, BLACK);
      start_btn.initButton(&tft, 95, 140, 90, 40, OLIVE, OLIVE, NAVY, "Start", 2);
      start_btn.drawButton();
    }
  }
}
void loop() {
  bool down = Touch_getXY();
  start_btn.press(down && start_btn.contains(pixel_x, pixel_y));
  settings_btn.press(down && settings_btn.contains(pixel_x, pixel_y));
  if (state) {
    if (millis() - prevMillis >= 10000 && x) {
      digitalWrite(10, HIGH);
      x = false;
      prevMillis = millis();
    }
    if (start_btn.justPressed()) {
      x = false;
      tft.fillRect(55, 100, 90, 40, BLACK);
      start_btn.initButton(&tft, 95, 140, 90, 40, OLIVE, OLIVE, NAVY, "Start", 2);
      start_btn.drawButton();
      digitalWrite(10, LOW);
      digitalWrite(0, HIGH);
      state = false;
      A = false;
      B = false;
      C = false;
      delay(500);
    }
  }
  else {
    if (start_btn.justPressed()) {
      tft.fillRect(55, 120, 90, 40, BLACK);
      start_btn.initButton(&tft, 95, 140, 90, 40, RED, RED, BLACK, "Stop", 2);
      start_btn.drawButton();
      digitalWrite(0, LOW);
      prevMillis = millis();
      state = true;
      x = true;
      delay(500);
    }
  }
  (settings_btn.justPressed()) ? Settings() : nope();
  if (digitalRead(10) == LOW) {
    if (digitalRead(13) == HIGH) { // Button B pressed
      if (prevState == LOW) {
        c = 13;
        B = true;
        prevMillis = millis();
        digitalWrite(10, HIGH);
        digitalWrite(0, LOW);
        state = true;
        checkHome(true);
      }
    }
    if (digitalRead(11) == HIGH && prevState == LOW) { // Button A pressed
      if (prevState == LOW) {
        c = 11;
        A = true;
        prevMillis = millis();
        digitalWrite(10, HIGH);
        digitalWrite(0, LOW);
        state = true;
        checkHome(true);
      }
    }
    if (digitalRead(1) == HIGH && prevState == LOW) { // Button C pressed
      if (prevState == LOW) {
        c = 1;
        C = true;
        prevMillis = millis();
        digitalWrite(10, HIGH);
        digitalWrite(0, LOW);
        state = true;
        checkHome(true);
      }
    }
  }
  if (digitalRead(12) == HIGH && prevState == LOW ) { // Button D pressed
    if (prevState == LOW) {
      c = 12;
      digitalWrite(10, LOW);
      digitalWrite(0, HIGH);
      state = false;
      checkHome(false);
    }
  }
  (c > 0) ? prevState = digitalRead(c) : 0;
  if (A) {
    if (millis() - prevMillis >= settings[0] * 60000) {
      digitalWrite(10, LOW);
      digitalWrite(0, HIGH);
      prevMillis = millis();
      A = false;
      state = false;
      checkHome(false);
    }
  }
  if (B) {
    if (millis() - prevMillis >= settings[1] * 60000) {
      digitalWrite(10, LOW);
      digitalWrite(0, HIGH);
      prevMillis = millis();
      B = false;
      state = false;
      checkHome(false);
    }
  }
  if (C) {
    if (millis() - prevMillis >= settings[2] * 60000) {
      digitalWrite(10, LOW);
      digitalWrite(0, HIGH);
      prevMillis = millis();
      C = false;
      state = false;
      checkHome(false);
    }
  }
}
