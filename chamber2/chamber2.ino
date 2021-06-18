#include <EEPROM.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/fixednums7x15.h>
#include <MD_YX5300.h>
#include <VL53L0X.h>
#include <Wire.h>

#define DISPLAY_WIDTH 1
#define DISPLAY_HEIGHT 1
#define UV 2
#define PUMP 3
#define GREEN_LED 5
#define RED_LED 4
#define BLUE_LED 10
#define UV_BTN 14

//SoftDMD dmd(DISPLAY_WIDTH, DISPLAY_HEIGHT, 10, 11, 12, 13, 14, 15); //(Width,Height,OE,A,B,SCK,CLK,data)
SoftDMD dmd(DISPLAY_WIDTH, DISPLAY_HEIGHT);
DMD_TextBox box(dmd);
MD_YX5300 mp3(1, 0);
VL53L0X sensor;

uint64_t previousMillis = 0;
uint64_t prevOneMin = 0;
uint64_t uvMillis;
uint32_t countFlag;
uint32_t count;
uint32_t jarak;
bool uv_lastState = HIGH;
bool uvButton;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sensor.setTimeout(500);
  if (!sensor.init()) {
    //Serial.println("ERROR SENSOR");
    digitalWrite(BLUE_LED, HIGH);
  }
  sensor.startContinuous();
  pinMode(BLUE_LED, OUTPUT);
  pinMode(UV, OUTPUT); //UV
  pinMode(PUMP, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);
  pinMode(UV_BTN, INPUT_PULLUP);
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);
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

void (* resetFunc)(void)=0;

void Sanitize(bool s = false) {
  previousMillis = millis();
  while (jarak > 80 && jarak < 100 || s == true) {
    jarak = sensor.readRangeContinuousMillimeters();
    if (jarak > 80 && jarak < 100 || s == true) {
      digitalWrite(BLUE_LED, HIGH);
    }
    else {
      digitalWrite(BLUE_LED, LOW);
    }
    if (millis() - previousMillis >= 1500 || s == true) {
      dmd.end();
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      //mp3.wakeUp();
      //Serial.print("Proses Sterilisasi...");
      mp3.playTrack(1);
      digitalWrite(PUMP, HIGH);
      digitalWrite(UV, HIGH);
      if (millis() - prevOneMin >= 60000) {
        delay(7500);
      }
      else {
        delay(5000);
      }
      digitalWrite(PUMP, LOW);
      digitalWrite(UV, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      mp3.playTrack(random(2, 4));
      //digitalWrite(BLUE_LED,HIGH);
      //Serial.println("Selesai");
      prevOneMin = millis();
      count += 1;
      EEPROM.update(0, count);
      delay(5000);
      previousMillis = millis();
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
  jarak = sensor.readRangeContinuousMillimeters();
  //Serial.println(jarak);
  if (jarak > 80 && jarak < 100) {
    //Serial.println(jarak);
    Sanitize();
  }
  else if(jarak==65535){
    resetFunc();
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
