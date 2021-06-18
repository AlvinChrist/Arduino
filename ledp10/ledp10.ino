#include <SPI.h>
#include <DMD2.h>
#include <SystemFont5x7.h>

#define DISPLAY_COLOUM_COUNT 2
#define DISPLAY_ROW_COUNT 1

char inputString;
bool stringEnd;
bool stringStart;
uint64_t previousMillis = 0;
char shootClockStart = 0;
byte counter = 24;
byte scoreA = 10;
byte scoreB = 12;

SoftDMD dmd(DISPLAY_COLOUM_COUNT, DISPLAY_ROW_COUNT);
DMDFrame box(dmd);

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
void setup() {
  Serial.begin(9600);
  dmd.setBrightness(50);
  dmd.selectFont(SystemFont5x7);
  dmd.begin();
  dmdUpdate();
  dmd.drawString((64 - dmd.stringWidth(String(counter))) / 2, 9 / 10, String(counter));
  dmd.drawLine(0, 8, 64, 8);
}

void dmdUpdate() {
  dmd.drawFilledBox(0, 9, 21, 16, GRAPHICS_OFF);
  dmd.drawString((64 - dmd.stringWidth(String(scoreA))) / 64, 9, String(scoreA));
  dmd.drawFilledBox(43, 9, 64, 16, GRAPHICS_OFF);
  dmd.drawString((64 - dmd.stringWidth(String(scoreB))), 9, String(scoreB));
  dmd.drawString((64 - dmd.stringWidth("A")) / 16, 9 / 10, "A");
  dmd.drawString((64 - dmd.stringWidth("B")) * 15 / 16, 9 / 10, "B");
}
// the loop routine runs over and over again forever:
void loop() {
  userInput();
  if (stringEnd) {
    Serial.println(inputString);
    switch (inputString) {
      case 'a':
        scoreA++;
        Serial.print("Tim A: ");
        Serial.println(scoreA);
        dmdUpdate();
        break;
      case 'b':
        scoreB++;
        Serial.print("Tim B: ");
        Serial.println(scoreB);
        dmdUpdate();
        break;
      case 'c':
        if (scoreA > 0) {
          scoreA--;
          Serial.print("Tim A: ");
          Serial.println(scoreA);
        }
        dmdUpdate();
        break;
      case 'd':
        if (scoreB > 0) {
          scoreB--;
          Serial.print("Tim B: ");
          Serial.println(scoreB);
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
        scoreA = 0;
        dmdUpdate();
        break;
      case 'i':
        scoreB = 0;
        dmdUpdate();
        break;
      case 'j':
        break;
    }
    inputString = "";
    stringEnd = false;
  }
  switch (shootClockStart) {
    case 2:
      //Serial.println("Reset shootclock");
      counter = 25;
      shootClockStart = 1;
      break;
    case 1:
      if (millis() - previousMillis >= 1000) {
        previousMillis = millis();
        counter--;
        dmd.drawFilledBox(27, 9 / 10, 40, 9 / 10 + 7, GRAPHICS_OFF);
        dmd.drawString((64 - dmd.stringWidth(String(counter))) / 2, 9 / 10, String(counter));
        if (counter == 0) {
          shootClockStart = false;
          counter = 25;
          break;
        }
        //Serial.println(counter);
      }
      break;
  }
}
