#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD2.h>
#include <SystemFont5x7.h>

SPIDMD dmd(1, 1);
DMD_TextBox box(dmd);

char inputString;
bool stringEnd, stringStart;
word skorA = 0;
word skorB = 0;
int shootTime = 24; //in second(s)
char matchTime[6];
unsigned long previousMillis = 0;
unsigned long prevMatchMillis = 0;
char shootClockStart = 0;
word matchTimeArr[] = {5, 10, 15};
int matchSec;
byte pos = 0;
byte startMatch = 0;
SoftwareSerial module_bluetooth(0, 1); // pin RX | TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dmd.setBrightness(25);
  dmd.selectFont(SystemFont5x7);
  dmd.begin();
  delay(200);
  dmdUpdate();
}

void timeSelect(bool t) {
  t ? (pos == 2) ? pos = 0 : pos += 1 : (pos == 0) ? pos = 2 : pos -= 1;
  Serial.println(matchTimeArr[pos]);
  matchSec = matchTimeArr[pos] * 60;
  delay(300);
}

void dmdUpdate() {
  dmd.clearScreen();
  dmd.drawString((32 - dmd.stringWidth(String(skorA))) / 32, (16 - 7), String(skorA));
  dmd.drawLine(0, 8, 31, 8);
  dmd.drawString((32 - dmd.stringWidth(String(shootTime))) / 2, (16 - 7) / 10, String(shootTime));
  dmd.drawString((32 - dmd.stringWidth(String(skorB))) * 1, (16 - 7), String(skorB));
}

void userInput() {
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

void loop() {
  switch (startMatch) {
    case 1:
      if (millis() - prevMatchMillis >= 1000) {
        prevMatchMillis = millis();
        if (matchTime < 0) {
          startMatch = 0;
          break;
          }
        sprintf(matchTime,"%02u:%02u",matchSec/60,matchSec%60);
        //dmdUpdate matchTime
      }
      break;
    case 2:
      matchSec = matchTimeArr[pos] * 60;
      startMatch=0;
      break;
  }
  switch (shootClockStart) {
    case 2:
      //Serial.println("Reset shootclock");
      shootTime = 24;
      shootClockStart = 1;
      break;
    case 1:
      if (millis() - previousMillis >= 1000) {
        previousMillis = millis();
        if (shootTime < 0) {
          shootClockStart = false;
          shootTime = 24;
          break;
        }
        //Serial.println(shootTime);
        dmdUpdate();
        shootTime--;
      }
      break;
  }
  userInput();
  if (stringEnd) {
    Serial.println(inputString);
    switch (inputString) {
      case 'a':
        skorA++;
        Serial.print(F("Tim A: "));
        Serial.println(skorA);
        dmdUpdate();
        break;
      case 'b':
        skorB++;
        Serial.print(F("Tim B: "));
        Serial.println(skorB);
        dmdUpdate();
        break;
      case 'c':
        if (skorA > 0) {
          skorA--;
          Serial.print(F("Tim A: "));
          Serial.println(skorA);
        }
        dmdUpdate();
        break;
      case 'd':
        if (skorB > 0) {
          skorB--;
          Serial.print(F("Tim B: "));
          Serial.println(skorB);
        }
        dmdUpdate();
        break;
      case 'e':
        shootClockStart = 1;
        break;
      case 'f':
        shootClockStart = 0;
        break;
      case 'g':
        shootClockStart = 2;
        break;
      case 'h':
        skorA = 0;
        dmdUpdate();
        break;
      case 'i':
        skorB = 0;
        dmdUpdate();
        break;
      case 'j':
        timeSelect(true);
        break;
      case 'k':
        timeSelect(false);
        break;
      case 'l':
        startMatch = 1;
        break;
      case 'm':
        (startMatch != 1) ? startMatch = 0 : 0;
        break;
      case 'n':
        startMatch = 2;
        break;
    }
    inputString = "";
    stringEnd = false;
  }
}
