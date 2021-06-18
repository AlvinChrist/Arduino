//#include <Wire.h>
#include <DMD2.h>
#include <fonts/test.h>
//#define ADDR 1 //0 1 2 (ganti sesuai board)

SPIDMD dmd(4, 4);
DMD_TextBox box(dmd);


float prevKalkulasi, kalkulasi;
byte pos, ratusan[2], desimal[2]; //, r_kecepatan, d_kecepatan;
char prevData[6], Data[6], prevDataOneSec[6], kecepatan[5];
bool startData, endData, floatSign;
uint64_t prevMillis;

void setup() {
  //Wire.begin(ADDR);
  //Wire.setClock(100000);
  Serial.begin(115200);
  Serial.println(F("Slave"));
  delay(200);
  // Wire.onRequest(sendMeasurement);
  dmd.setBrightness(255);
  dmd.selectFont(test);
  dmd.begin();
  delay(200);
}

/*void sendMeasurement() {
  Wire.write(ratusan[1]);   // kirim ratusan jarak
  Wire.write(desimal);      // kirim desimal jarak
  Wire.write(r_kecepatan);  // kirim ratusan/puluhan kecepatan
  Wire.write(d_kecepatan);  // kirim desimal kecepatan
  //Serial.print(ratusan);
  //Serial.print('\t');
  //Serial.println(desimal);

  //Serial.println(prevData);
  }*/

void dmdUpdate() {
  dmd.clearScreen();
  dmd.drawString(128 - dmd.stringWidth(kecepatan) - 2, 8, kecepatan);
  //Serial.print(F("DMD:"));
  //Serial.println(prevData);
}

void calculateSpeed() {
  float jarak = (float) atof(prevData);
  float prevJarak = (float)atof(prevDataOneSec);
  kalkulasi = fabs(((jarak - prevJarak) * 1943.84) / (millis() - prevMillis));
  //char tmp[3];
  //bool f_sign;
  dtostrf(kalkulasi, 5, 1, kecepatan); // masukkan jarak ke dalam char array dengan pembatasan 1 desimal
  /*for (byte x = 0; x < 5; x++) { // memisahkan ratusan/puluhan dari desimal
    if (kecepatan[x] == '.') {
      r_kecepatan = atoi(tmp);
      memset(tmp, 0, sizeof(tmp));
      f_sign = true;
    }
    else {
      if (f_sign) {
        d_kecepatan = kecepatan[x] - '0';
        f_sign = false;
      }
      else {
        tmp[x] = kecepatan[x];
      }
    }
    }*/
  //Serial.println(r_kecepatan);
  //Serial.println(d_kecepatan);
  //Serial.println(kecepatan);
  memset(prevDataOneSec, 0, sizeof(prevDataOneSec));
  strcpy(prevDataOneSec, prevData);
  prevMillis = millis();
}

void foo() {

}
void loop() {
  if (endData) {
    //Serial.println(Data);
    if (ratusan[0] != ratusan[1] || desimal[0] != desimal[1]) {
      ratusan[1] = ratusan[0];
      desimal[1] = desimal[0];
      //memcpy(&ratusan[1], &ratusan[0], 1);
      memset(prevData, 0, sizeof(prevData)); //hapus isi prevData[]
      strcpy(prevData, Data); // copy isi Data[] ke prevData[]
      endData = false;
      //Serial.print(F("WeMos:"));
      //Serial.println(prevData);
    }
  }
  if (millis() - prevMillis >= 1000) {
    calculateSpeed();
    if (kalkulasi != prevKalkulasi) {
      dmdUpdate();
      Serial.println(kalkulasi);
      prevKalkulasi = kalkulasi;
    }
  }
}

void serialEvent() {
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case '\r': //10
        floatSign = false;
        endData = true;
        Data[pos] += '\0';
        break;
      case '\n': //13
        memset(Data, 0, sizeof(Data)); //Data[6] = 3 ratusan, 1 tanda titik, 1 desimal, 1 null character (\0)
        pos = 0;
        startData = true;
        endData = false;
        break;
      case '.':
        if (startData) {
          ratusan[0] = atoi(Data);
          Data[pos] += inChar;
          pos += 1;
          floatSign = true;
        }
        break;
      default:
        if (startData) {
          Data[pos] = inChar;
          pos += 1;
          if (floatSign) {
            desimal[0] = inChar - '0';
            startData = false; // lompati desimal ke 2 dan ke 3
          }
        }
        break;
    }
  }
}
