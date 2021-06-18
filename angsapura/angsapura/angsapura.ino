#include "Arduino.h"

struct DATA {
  String data;
};

DATA myData;
int shootClock;
uint8_t _buf;
uint64_t prevMillis = 0;
bool start_btn;
bool add;
bool decrease;

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
  Serial.begin(9600);
  Serial.println("DEVICE READY");
  shootClock = 24;
}
void loop() {
  if (millis() - prevMillis >= 1000) {
    prevMillis=millis();
    myData.data = "("+String(shootClock)+")";
    SendStruct(&myData, sizeof(myData));
    shootClock-=1;
    (shootClock<0)?shootClock=24:0;
  }
  //if (Serial.available()) {
  //GetStruct(&myData, sizeof(myData));
  //Serial.println(myData.command);
  //}
}
