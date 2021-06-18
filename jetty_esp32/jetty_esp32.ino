/*
   HTTP Client POST Request
   Copyright (c) 2018, circuits4you.com
   All rights reserved.
   https://circuits4you.com
   Connects to WiFi HotSpot. */

//#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266HTTPClient.h>
#include <HTTPClient.h>
//#include <PolledTimeout.h>

#define host "http://www.pertaminajetymonitoring.com/_php_/send.php"

const char* ssid = "Kui Home";
const char* password = "homesweethome";
/* Set these to your desired credentials. */
//const char* ssid = "Acer E-14-2.4G";
//const char* password =  "123456789012";
//const char* ssid = "Pertamina";
//const char* password =  "Pertamina88";
//const char* ssid =  "denny";
//const char* password =  "denny1234";

char postData[100], kecepatan[6], prevKecepatan[6];
float jarakTengah, prevJarakTengah;
uint16_t data16[3], prevData16[3];
byte ratusan[3], desimal[3], prevRatusan[3], prevDesimal[3];
int abc;
unsigned long prevMillis;
unsigned int data_[6] = {67, 70, 73, 300, 0, 0};
float kec;
//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  //Wire.begin(D2 , D1);
  //delay(500);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print(F("Connecting"));
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
int kirim_ke_web() {
  HTTPClient http;
  /*postData=F("key=cf40ae1644858daf0bf948f55b32a480&data1=");
    postData.concat((int)data[0][0]);
    postData.concat('.');
    postData.concat((int)data[0][1]);
    postData.concat(F("&data2="));
    postData.concat((int)data[1][0]);
    postData.concat('.');
    postData.concat((int)data[1][1]);
    postData.concat(F("&data3="));
    postData.concat((int)data[2][0]);
    postData.concat('.');
    postData.concat((int)data[2][1]);
  */
  //sprintf(postData, "key=cf40ae1644858daf0bf948f55b32a480&data1=%d.%d&data2=%d.%d&data3=%d.%d&data4=%d&data5=%d&data6=%s", prevRatusan[0], prevDesimal[0], prevRatusan[1], prevDesimal[1], prevRatusan[2], prevDesimal[2], prevData16[0], prevData16[1], prevKecepatan);
  sprintf(postData, "key=cf40ae1644858daf0bf948f55b32a480&data1=%d&data2=%d&data3=%d&data4=%d&data5=0&data6=%d", data_[0], data_[1], data_[2], data_[3], data_[5]);
  Serial.println(postData);
  http.begin(host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
  //Serial.println(httpCode);
  //Serial.println(payload);
  http.end();
  memset(postData, 0, sizeof(postData));
  return httpCode;
}

/*void get_i2c_data(byte &address) {
  byte pos = 0;
  if(address==3 || address==1){
    Wire.requestFrom(address,4);
  }
  else{
    Wire.requestFrom(address,2);
  }
  //(address != 3 || address!=1) ? Wire.requestFrom(address, 2) : Wire.requestFrom(address, 4);
  while (Wire.available() > 0) {
    if (address == 3) {
      data16[0] = Wire.read() << 8;
      data16[0] |= Wire.read();
      data16[1] = Wire.read() << 8;
      data16[1] |= Wire.read();
    }
    else if (address == 1) {
      data[1][0] = Wire.read();
      data[1][1] = Wire.read();
      data[3][0] = Wire.read();
      data[3][1] = Wire.read();
    }
    else {
      byte c = (byte) Wire.read();
      data[address][pos] = c;
      pos += 1;
    }
  }
  }*/

void loop() {
  //using periodic = esp8266::polledTimeout::periodic;
  //static periodic nextPing(1000);  
  if (millis() - prevMillis >= 5000) {
    prevMillis = millis();
    abc = kirim_ke_web();
    for(int i=0;i<3;i++){
      if(data_[i]==1){
        
      }
      else{
        data_[i]-=1;
      }
    }
    data_[3]=random(300,360);
    /*Wire.requestFrom(0, 10); //(address,byte)->ganti sesuai address
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
      jarakTengah = ratusan[1] + desimal[1] / 10.0;
      float kalkulasi = fabs(((jarakTengah - prevJarakTengah) * 1943.84) / (millis() - prevMillis));
      prevMillis = millis();
      memset(kecepatan, 0, sizeof(kecepatan));
      dtostrf(kalkulasi, 0, 1, kecepatan);
      kecepatan[5] = '\0'; //null termination
      (jarakTengah != prevJarakTengah) ? prevJarakTengah = jarakTengah : 0;
      if (abc != 200) {
      abc = kirim_ke_web(); //jika response sebelumnya bukan 200, maka kirim data lagi
      }
      if (abc < 0) {
      Serial.println(F("Error pada ESP!"));
      }
      if (abc > 200) {
      Serial.println(F("Error pada Web"));
      }
      else {
      for (int i = 0; i < 3; i++) {
        if (ratusan[i] != prevRatusan[i] || desimal[i] != prevDesimal[i] || data16[i] != prevData16[i]) {
          prevRatusan[i] = ratusan[i];
          prevDesimal[i] = desimal[i];
          prevData16[i] = data16[i];
          abc = kirim_ke_web();
        }
      }
      if (strcmp(kecepatan, prevKecepatan) != 0) {//0->kecepatan==prevKecepatan
        memset(prevKecepatan, 0, sizeof(prevKecepatan));
        strcpy(prevKecepatan, kecepatan); //copy isi kecepatan ke prevKecepatan
        kirim_ke_web();
      }
      }*/
  }

}
