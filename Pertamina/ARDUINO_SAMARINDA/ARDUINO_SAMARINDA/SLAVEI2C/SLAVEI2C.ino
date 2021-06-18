#include <Wire.h>
#include <DMD2.h>
#include <fonts/test.h>
#define ADDR 2 //0 1 2 (ganti sesuai board)

SPIDMD dmd(4, 4);
DMD_TextBox box(dmd);

byte pos, count, ratusan[2], desimal[2];
char prevData[6], Data[6];
bool startData, endData, floatSign;
void setup() {
  Wire.begin(ADDR);
  Wire.setClock(100000);
  Serial.begin(115200);
  Serial.println(F("Slave"));
  delay(500);
  Wire.onRequest(sendMeasurement);
  dmd.setBrightness(255);
  dmd.selectFont(test);
  dmd.begin();
  delay(200);
}

void sendMeasurement() {
  Wire.write(ratusan[1]);
  Wire.write(desimal[1]);
  //Serial.print(ratusan);
  //Serial.print('\t');
  //Serial.println(desimal);

  //Serial.println(prevData);
}

void dmdUpdate() {
  dmd.clearScreen();
  dmd.drawString(128 - dmd.stringWidth(prevData) - 2, 8, prevData);
  //Serial.print(F("DMD:"));
  //Serial.println(prevData);
}

void foo() {

}
void loop() {
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case '\r': //10
        startData = false;
        endData = true;
        desimal[0] = 0;
        desimal[0] = Data[count];
        (desimal[0] != desimal[1]) ? memcpy(&desimal[1], &desimal[0], 1) : 0;
        Data[pos] += '\0';
        break;
      case '\n': //13
        memset(Data, 0, sizeof(Data));
        pos = 0;
        count = 0;
        startData = true;
        endData = false;
        floatSign = false;
        break;
      case '.':
        if (startData) {
          ratusan[0] = 0;
          ratusan[0] = atoi(Data);
          Data[pos] += inChar;
          pos += 1;
          count = pos;
          floatSign = true;
        }
        break;
      default:
        if (startData) {
          memcpy(&Data[pos], &inChar, 1);
          pos += 1;
          if (floatSign) {
            startData = false;
          }
        }
        break;
    }
  }
  if (endData) {
    //Serial.println(Data);
    if (ratusan[0] != ratusan[1]) {
      memcpy(&ratusan[1], &ratusan[0], 1);
      memset(prevData, 0, sizeof(prevData));
      strcpy(prevData, Data);
      dmdUpdate();
      //Serial.print(F("WeMos:"));
      //Serial.println(prevData);
    }
    endData = false;
  }
}
