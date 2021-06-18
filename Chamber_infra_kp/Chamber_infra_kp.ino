#include <EEPROM.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/fixednums7x15.h>
#include <MD_YX5300.h>

#define DISPLAY_WIDTH 1
#define DISPLAY_HEIGHT 1
#define UV 3
#define PUMP 4
#define GREEN_LED 8
#define RED_LED 7
#define BLUE_LED 9
#define UV_BTN 18
#define SENSOR 2

SoftDMD dmd(DISPLAY_WIDTH, DISPLAY_HEIGHT, 10, 11, 12, 13, 14, 15); //(Width,Height,OE,A,B,SCK,CLK,data)
//SoftDMD dmd(DISPLAY_WIDTH, DISPLAY_HEIGHT);
DMD_TextBox box(dmd);
MD_YX5300 mp3(1, 0);

uint64_t previousMillis = 0;
uint64_t prevOneMin = 0;
uint64_t uvMillis;
uint32_t countFlag;
uint32_t count;
bool sensor;
bool uv_lastState = HIGH;
bool uvButton;

void setup() {
  Serial.begin(9600);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(UV, OUTPUT); //UV
  pinMode(PUMP, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);
  pinMode(UV_BTN, INPUT_PULLUP);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  delay(200);
  mp3.begin();
  delay(200);
  mp3.volume(30);
  delay(200);

  //EEPROM.update(0,0);
  dmd.setBrightness(50);
  dmd.selectFont(fixednums7x15);
  count = EEPROM.read(0);
  if (count == 0) {
    dmd.begin();
    delay(100);
    dmd.drawString((32 - dmd.stringWidth(String(count))) / 2, (3 / 2), String(count));
  }
  //tombol reset dan UV
}

void Sanitize(bool s = false) {
  previousMillis = millis();
  while (!sensor || s == true) {
    Serial.println("B");
    sensor = digitalRead(SENSOR);
    (!sensor || s == true) ? digitalWrite(BLUE_LED, HIGH) : digitalWrite(BLUE_LED, LOW);
    if (millis() - previousMillis >= 1500 || s == true) {
      dmd.end();
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      mp3.wakeUp();
      //Serial.print("Proses Sterilisasi...");
      mp3.playTrack(1);
      digitalWrite(PUMP, HIGH);
      digitalWrite(UV, HIGH);
      if (millis() - prevOneMin >= 60000) {
        delay(11000);
      }
      else {
        delay(8000);
      }
      digitalWrite(PUMP, LOW);
      digitalWrite(UV, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      mp3.playTrack(random(2, 4));
      digitalWrite(BLUE_LED,HIGH);
      //Serial.println("Selesai");
      prevOneMin = millis();
      count += 1;
      EEPROM.update(0, count);
      delay(5000);
      previousMillis = millis();
      sensor=digitalRead(SENSOR);
      //Serial.println("C");
      while (!sensor && s) {
        sensor=digitalRead(SENSOR);
        if (millis() - previousMillis >= 2000) {
          mp3.playTrack(4);
          delay(5000);
          previousMillis = millis();
        }
      }
      s = false;
      digitalWrite(BLUE_LED, LOW);
    }
  }
}

void loop() {
  uvButton = digitalRead(UV_BTN);
  if (uv_lastState == HIGH && uvButton == LOW) {
    uvMillis = millis();
  }
  else if (uv_lastState == LOW && uvButton == HIGH) {
    if (millis() - uvMillis >= 150 && millis() - uvMillis < 1000) {
      Sanitize(true);
    }
    else if (millis() - uvMillis >= 1000 && millis() - uvMillis < 3000) {
      dmd.end();
      mp3.playTrack(5);
      digitalWrite(UV, HIGH);
      delay(6000);
      digitalWrite(UV, LOW);
      mp3.playTrack(6);
      delay(1000);
      dmd.begin();
      dmd.clearScreen();
      dmd.drawString((32 - dmd.stringWidth(String(count))) / 2, (3 / 2), String(count));
    }
    else if (millis() - uvMillis >= 3000) {
      EEPROM.update(0, 0);
      delay(500);
      count = EEPROM.read(0);
    }
  }
  uv_lastState = uvButton;
  //Serial.println(jarak);
  sensor = digitalRead(SENSOR);
  if (!sensor) {
    //Serial.println("A");
    Sanitize();
  }
  else {
    if (count != countFlag) {
      dmd.begin();
      dmd.clearScreen();
      dmd.drawString((32 - dmd.stringWidth(String(count))) / 2, (3 / 2), String(count));
      countFlag = count;
      //Serial.println(count);
    }
  }
  /*  for(int i=1; i<9;i++){
      mp3.playTrack(i);
      delay(5000);
    }
  */
}
