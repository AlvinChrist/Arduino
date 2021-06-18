#include "Arduino.h"
/*
  struct DATA {
  char startByte,endByte;
  int shootClock;
  };

  DATA myData;
  uint8_t _buf;
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
bool stringEnd;
bool stringStart;
String inputString;

void setup() {
  Serial.begin(19200 );
  Serial.println("DEVICE READY");
}

void loop() {
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case ')':
        stringEnd = true;
        stringStart = false;
        Serial.println(inputString);
        break;
      case '(':
        inputString = "";
        stringStart = true;
        break;
      default:
        (stringStart) ? inputString += inChar : "";
    }
  }
}
