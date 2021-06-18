//#include <Wire.h>
#include <DMD2.h>
#include <fonts/test.h>
//#define ADDR 3 //0 1 2

SPIDMD dmd(4, 4);
DMD_TextBox box(dmd);

byte c, d;//, dataArr[4];
bool mode;
uint16_t data[2], prevData[2];
char inData[2][6],p10_data[2][6];
bool startData, endData;
uint64_t prevMillis;

void setup() {
  //Wire.begin(ADDR);
  //Wire.setClock(100000);
  Serial.begin(115200);
  Serial.println(F("Slave"));
  delay(500);
  //Wire.onRequest(sendMeasurement);
  dmd.setBrightness(255);
  dmd.selectFont(test);
  dmd.begin();
  delay(200);
}

/*void sendMeasurement() {
  memset(dataArr, 0, sizeof(dataArr));
  dataArr[0] = (prevData[0] >> 8);
  dataArr[1] = prevData[0] & 0xFF;
  dataArr[2] = (prevData[1] >> 8);
  dataArr[3] = prevData[1] & 0xFF;
  Wire.write(dataArr, 4);
  //Serial.print(ratusan);
  //Serial.print('\t');
  //Serial.println(desimal);

  //Serial.println(prevData);
  }*/

void foo() {

}
void dmdUpdate() {
  if (mode) { //sensor arah angin
    dmd.clearScreen();
    //itoa(prevData[0], str, 10);
    dmd.drawString(128 - dmd.stringWidth(p10_data[0]) - 14, 9, p10_data[0]);
    dmd.drawCircle(121, 9, 5);
    //Serial.println(str);
    //Serial.println(F("mode1"));
  }
  else { // sensor
    dmd.clearScreen();
    //itoa(prevData[1], str, 10);
    dmd.drawString(128 - dmd.stringWidth(p10_data[1]) - 2, 9, p10_data[0]);
    //Serial.println(str);
    //Serial.println(F("mode2"));
  }
}

void loop() {
  if (endData) {
    memcpy(inData,p10_data,sizeof(p10_data));
    if (mode) {
      if (data[0] != prevData[0]) {
        prevData[0] = data[0];
        //memset(prevData[0], 0, sizeof(prevData[0]));
        //memcpy(&prevData[0], &data[0], sizeof(data[0]));
        //  Serial.print(prevData[1]);
        dmdUpdate();
      }
    }
    else if (!mode) {
      if (data[1] != prevData[1]) {
        prevData[1] = data[1];
        //memset(prevData[1], 0, sizeof(prevData[1]));
        //memcpy(&prevData[1], &data[1], sizeof(data[1]));
        //  Serial.print(prevData[1]);
        dmdUpdate();
      }
    }
    endData = false;
  }
  if (millis() - prevMillis >= 5000) {
    prevMillis = millis();
    mode = ! mode;
    //Serial.println(F("Change"));
    //Serial.println();
    dmd.clearScreen();
    dmdUpdate();
  }
}

void serialEvent() {
  while (Serial.available() > 0) {
    char inChar = (byte)Serial.read();
    switch (inChar) {
      case '#': //10
        startData = false;
        endData = true;
        break;
      case '*': //13
        c = 0;
        d = 0;
        memset(inData, 0, sizeof(inData));
        startData = true;
        endData = false;
        break;
      case ',':
        if (c < 2) {
          uint16_t tmp;
          tmp = atoi(inData[c]);
          if (tmp != data[c]) {
            data[c] = tmp;
          }
          c += 1;
          d = 0;
        }
        break;
      default:
        if (startData) {
          inData[c][d] = inChar;
          d += 1;
        }
        break;
    }
  }
}
