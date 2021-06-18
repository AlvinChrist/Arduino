#define SERIAL_BUFFER_SIZE 128

#include <Wire.h>
#include <DMD2.h>
#include <fonts/test.h>
#define ADDR 0 //0 1 2 (ganti sesuai board)
#define trigger 3

SPIDMD dmd(4, 4);
DMD_TextBox box(dmd);

String inData2, inData;
char kecepatan[5];
word data16[2];
bool startData, endData, startData2, endData2, floatSign;
unsigned long prevMillis;
byte ratusan[3], desimal[3];
uint8_t cmd[4][6] = {{0xC2, 0x00, 0x15, 0x3D, 0x01, 0x44}, {0xC2, 0x00, 0x16, 0x3D, 0x02, 0x44},
  {0xC2, 0x00, 0x17, 0x3D, 0x03, 0x44}, {0xC2, 0x00, 0x18, 0x3D, 0x04, 0x44}
};

void setup() {
  Wire.begin(ADDR);
  Wire.setClock(100000);
  Serial.begin(115200);
  Serial.println(F("Slave"));
  pinMode(trigger, OUTPUT);
  //delay(500);
  Wire.onRequest(sendDataToMaster);
  dmd.setBrightness(255);
  dmd.selectFont(test);
  dmd.begin();
  delay(200);
  inData2.reserve(5);
  inData.reserve(5);
}

void changeSettings(uint8_t options) {
  digitalWrite(trigger, HIGH);
  Serial.flush(); // wait for last transmitted data to be sent
  Serial.begin(9600);
  while (Serial.available()) Serial.read();
  delay(150);
  for (byte x = 0; x < 6; x++) {
    Serial.write(cmd[options][x]);
  }
  Serial.flush(); // wait for last transmitted data to be sent
  Serial.begin(115200);
  while (Serial.available()) Serial.read();
  delay(150);
  digitalWrite(trigger, LOW);
  startData = false;
  startData2 = false;
  endData = false;
  endData2 = false;
}

void sendDataToMaster() {
  Serial.println(F("I2c"));
  Wire.write(ratusan[0]);
  Wire.write(ratusan[1]);
  Wire.write(ratusan[2]);
  Wire.write(desimal[0]);
  Wire.write(desimal[1]);
  Wire.write(desimal[2]);
  Wire.write((data16[0] >> 8) & 0xFF);
  Wire.write(data16[0] & 0xFF);
  Wire.write((data16[1] >> 8) & 0xFF);
  Wire.write(data16[1] & 0xFF);
  //Serial.print(ratusan[1]);
  //Serial.print('\t');
  //Serial.println(desimal);

  //Serial.println(prevData);
}

/*void dmdUpdate() {
  dmd.clearScreen();
  dmd.drawString(128 - dmd.stringWidth(prevData) - 2, 8, prevData);
  }*/

void loop() {
  for  (byte i = 0; i < 4; i++) {
    changeSettings(i);
    byte c;
    Serial.println(Serial.available());
    delay(200);
    if (i == 0) {
      while (Serial.available() > 0) {
        char inChar = (byte)Serial.read();
        switch (inChar) {
          case '#': //10
            startData = false;
            endData = true;
            c = 0;
            break;
          case '*': //13
            c = 0;
            inData = "";
            startData = true;
            endData = false;
            break;
          case ',':
            if (c < 2) {
              data16[c] = inData.toInt();
              inData = "";
              c += 1;
            }
            break;
          default:
            if (startData) {
              inData += inChar;
            }
            break;
        }
      }
    }
    else {
      while (Serial.available() > 0) {
        char inChar = (byte)Serial.read();
        switch (inChar) {
          case '\r': //10
            endData2 = true;
            startData2 = false;
            break;
          case '\n': //13
            inData2 = "";
            startData2 = true;
            endData2 = false;
            break;
          case '.':
            if (startData2) {
              ratusan[i - 1] = inData2.toInt();
              inData2 = "";
              floatSign = true;
            }
            break;
          default:
            if (startData2) {
              inData2 += inChar;
              if (floatSign) {
                desimal[i - 1] = inChar - '0';
                startData2 = false;
                floatSign = false;
              }
            }
            break;
        }
      }
    }
  }
  Serial.print(F("ANGIN:"));
  Serial.println(data16[0]);
  Serial.print(F("ARUS:"));
  Serial.println(data16[1]);
  Serial.println(F("\n"));
  Serial.print(F("Ratusan[0]"));
  Serial.println(ratusan[0]);
  Serial.print(F("Ratusan[1]"));
  Serial.println(ratusan[1]);
  Serial.print(F("Ratusan[2]"));
  Serial.println(ratusan[2]);
  Serial.print(F("Desimal[0]"));
  Serial.println(desimal[0]);
  Serial.print(F("Desimal[1]"));
  Serial.println(desimal[1]);
  Serial.print(F("Desimal[2]"));
  Serial.println(desimal[2]);
  Serial.println();
}
/*void serialEvent() {

  }*/
