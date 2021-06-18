#include <Wire.h>
word data16[2], prevData16[2];
byte ratusan[3], desimal[3], prevRatusan[3], prevDesimal[3];;
char postData[100], kecepatan[6],prevKecepatan[6];
float jarakTengah, prevJarakTengah;
unsigned long prevMillis;

void setup() {
  Serial.begin(115200);
  Wire.begin();
}
void kirim_ke_web() {
  sprintf(postData, "key=cf40ae1644858daf0bf948f55b32a480&data1=%d.%d&data2=%d.%d&data3=%d.%d&data4=%d&data5=%d&data6=%s", prevRatusan[0], prevDesimal[0], prevRatusan[1], prevDesimal[1], prevRatusan[2], prevDesimal[2], prevData16[0], prevData16[1], prevKecepatan);
  Serial.println(postData);
}

void loop() {
  delay(1);
  Wire.requestFrom(0, 10);
  ratusan[0] = Wire.read();
  ratusan[1] = Wire.read();
  ratusan[2] = Wire.read();
  desimal[0] = Wire.read();
  desimal[1] = Wire.read();
  desimal[2] = Wire.read();
  data16[0] = Wire.read() << 8;
  data16[0] |= Wire.read();
  data16[1] = Wire.read() << 8;
  data16[1] |= Wire.read();
  /*Serial.print(F("Data16[0]:"));
    Serial.println(data16[0]);
    Serial.print(F("Data16[1]:"));
    Serial.println(data16[1]);
    Serial.print(F("ratusan[0]:"));
    Serial.println(ratusan[0]);
    Serial.print(F("ratusan[1]:"));
    Serial.println(ratusan[1]);
    Serial.print(F("ratusan[2]:"));
    Serial.println(ratusan[2]);
    Serial.print(F("desimal[0]:"));
    Serial.println(desimal[0]);
    Serial.print(F("desimal[1]:"));
    Serial.println(desimal[1]);
    Serial.print(F("desimal[1]:"));
    Serial.println(desimal[2]);
    Serial.println();*/
  jarakTengah = ratusan[1] + desimal[1] / 10.0;
  float kalkulasi = fabs(((jarakTengah - prevJarakTengah) * 1943.84) / (millis() - prevMillis));
  prevMillis = millis();
  dtostrf(kalkulasi, 0, 1, kecepatan);
  kecepatan[5]="\0"; //null termination
  (jarakTengah != prevJarakTengah) ? prevJarakTengah = jarakTengah : 0;
  for (int i = 0; i < 3; i++) {
    if (ratusan[i] != prevRatusan[i] || desimal[i] != prevDesimal[i] || data16[i] != prevData16[i]) {
      prevRatusan[i] = ratusan[i];
      prevDesimal[i] = desimal[i];
      prevData16[i] = data16[i];
      kirim_ke_web();
    }
    delay(1000);
  }
  if(strcmp(kecepatan,prevKecepatan)!=0){
    strcpy(prevKecepatan,kecepatan); //copy isi kecepatan ke prevKecepatan
    kirim_ke_web();
  }
}
