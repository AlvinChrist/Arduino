#include <Wire.h>
#define ADDR 4 //0 1 2

uint16_t rpm=10000;
uint8_t myarr[2];
/*byte pos, count,ratusan,desimal;
char prevData[6], Data[6], tempEsp[4],EspData[4], EspData1[2];
bool startData, endData, floatSign;*/
void setup() {
  Wire.begin(ADDR);
  Serial.begin(9600);
  Serial.println(F("Slave"));
  delay(500);
  Wire.onRequest(sendMeasurement);
  delay(200);
}

void sendMeasurement() {
  myarr[0] =  rpm>>8;
  myarr[1] = rpm & 0xFF;
  Wire.write(myarr,2);
  //Serial.println(prevData);
}

void loop() {
  /*
  while (Serial.available()) {
    char inChar = (int)Serial.read();
    switch (inChar) {
      case '\r': //10
        startData = false;
        endData = true;
        floatSign = false;
        Data[pos] += '\0';
        EspData[pos] += '\0';
        EspData[1] += '\0';
        break;
      case '\n': //13
        memset(Data, 0, sizeof(Data));
        memset(tempEsp,0,sizeof(tempEsp));
        pos = 0;
        count = 0;
        startData = true;
        endData = false;
        break;
      case '.':
        if (startData) {
          floatSign = true;
          Data[pos] += inChar;
          pos += 1;
        }
        break;
      default:
        if (count == 1) {
          startData = false;
        }
        if (startData) {
          Data[pos] = inChar;
          if (floatSign) {
            EspData1[0] = inChar;
            count += 1;
          }
          else {
            tempEsp[pos] = inChar;
          }
          pos += 1;
        }
        break;
    }
    if (endData) {
      if (strcmp(prevData, Data)) {
        memset(prevData, 0, sizeof(prevData));
        memset(EspData,0,sizeof(EspData));
        //Serial.print(F("WeMos:"));
        //Serial.println(Data);
        strcpy(prevData, Data);
        strcpy(EspData,tempEsp);
      }
      endData = false;
    }
  }*/
}
