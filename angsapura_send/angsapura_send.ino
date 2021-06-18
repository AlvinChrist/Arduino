#include "Arduino.h"
#define GND 11

String data;
int shootClock;
uint64_t prevMillis = 0;
uint64_t btnMillis = 0;
bool start_flag = false;
byte button;
byte c;
int buttonArr[] = {5, 6, 7, 8, 9, 10, 12};

/*
  bool SendStruct(const void *TheStructure, uint16_t size_) {
  _buf = Serial.write((uint8_t*) TheStructure, size_);
  delay(50);
  return (_buf == size_);
  }

  bool GetStruct(const void *TheStructure, uint16_t size_) {
  _buf = Serial.readBytes((uint8_t*) TheStructure, size_);
  delay(50);
  return (_buf == size_);
  }
*/
/*void userInput() {
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case '#':
        stringEnd = true;
        stringStart = false;
        break;
      case '*':
        inputString = "";
        stringStart = true;
        break;
    }
    if (stringStart) {
      inputString = inChar;
    }
  }
  }
*/
void setup() {
  Serial.begin(19200);
  for (int i = 0; i < 7; i++) {
    pinMode(buttonArr[i], INPUT_PULLUP);
  }
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  shootClock = 24;
  data = "(" + String(shootClock) + ")";
  Serial.print(data);
}

void loop() {
  if (start_flag) {
    if (millis() - prevMillis >= 500) {
      if (shootClock > 0 && c == 2) {
        shootClock -= 1;
        c = 0;
      }
      else{
        data = "(" + String(shootClock) + ")";
        Serial.print(data);
        prevMillis = millis();
        c++;
        if (shootClock == 0&&c==2) {
          digitalWrite(A0, LOW);
          data = "(" + String(shootClock) + ")";
          Serial.print(data);
          prevMillis = millis();
          delay(800);
          digitalWrite(A0, HIGH);
          start_flag = false;
        }
      }
    }
  }
  for (int i = 0; i < 7; i++) {
    button = bitWrite(button, i, !digitalRead(buttonArr[i]));
  }
  switch (button) {
    case 1:
      btnMillis = millis();
      while (!digitalRead(5)) {

      }
      if (millis() - btnMillis > 100) {
        shootClock = 24;
        data = "(" + String(shootClock) + ")";
        Serial.print(data);
      }
      break;
    case 2:
      btnMillis = millis();
      while (!digitalRead(6)) {

      }
      if (millis() - btnMillis > 100) {
        shootClock = 14;
        data = "(" + String(shootClock) + ")";
        Serial.print(data);
        delay(300);
        Serial.print(data);
      }
      break;
    case 4:
      btnMillis = millis();
      while (!digitalRead(7)) {

      }
      if (millis() - btnMillis > 100) {
        shootClock = 0  ;
        data = "(" + String(shootClock) + ")";
        Serial.print(data);
        delay(300);
        Serial.print(data);
        start_flag = false;
      }
      break;
    case 8:
      btnMillis = millis();
      while (!digitalRead(8)) {

      }
      if (millis() - btnMillis > 100&&shootClock!=0) {
        start_flag = !start_flag;
      }
      break;
    case 16:
      btnMillis = millis();
      while (!digitalRead(9)) {

      }
      if (millis() - btnMillis > 100) {
        shootClock = 60;
        data = "(" + String(shootClock) + ")";
        Serial.print(data);
        delay(300);
        Serial.print(data);
      }
      break;
    case 32:
      btnMillis = millis();
      while (!digitalRead(10)) {

      }
      if (millis() - btnMillis > 100) {
        shootClock = 30;
        data = "(" + String(shootClock) + ")";
        Serial.print(data);
        delay(300);
        Serial.print(data);
      }
      break;
    case 64:
      btnMillis = millis();
      while (!digitalRead(12) && millis - btnMillis > 100) {
        digitalWrite(A0, LOW);
      }
      digitalWrite(A0, HIGH);
  }
}
