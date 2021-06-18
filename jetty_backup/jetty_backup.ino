/*
   HTTP Client POST Request
   Copyright (c) 2018, circuits4you.com
   All rights reserved.
   https://circuits4you.com
   Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <PolledTimeout.h>
#define host "http://www.pertaminajetymonitoring.com/_php_/send.php"
//#define ssid "Kui Home"
//#define password "homesweethome"

//String postData;

char postData[100];
/* Set these to your desired credentials. */
//const char* ssid = "Acer E-14-2.4G";
//const char* password =  "123456789012";
const char* ssid = "Pertamina";
const char* password =  "Pertamina88";
//const char* ssid =  "denny";
//const char* password =  "denny1234";
byte data[3][2], prevData[3][2];
uint16_t data16[2], prevData16[2];
bool startData, endData;
int stat;
//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  Wire.begin(D2 , D1);
  delay(500);
  Serial.begin(9600);
  //WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
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
  sprintf(postData, "key=cf40ae1644858daf0bf948f55b32a480&data1=%d.%d&data2=%d.%d&data3=%d.%d&data4=%d&data5=%d", (int)data[0][0], (int)data[0][1], (int)data[1][0], (int)data[1][1], (int)data[2][0], (int)data[2][1], prevData16[0], prevData16[1]);
  Serial.println(postData);
  http.begin(host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
  Serial.println(httpCode);
  //Serial.println(payload);
  http.end();
  memset(postData, 0, sizeof(postData));
  return httpCode;
}

void get_i2c_data(byte &address) {
  byte pos = 0;
  (address != 3) ? Wire.requestFrom(address, 2) : Wire.requestFrom(address, 4);
  while (Wire.available() > 0) {
    if (address == 3) {
      data16[0] = Wire.read() << 8;
      data16[0] |= Wire.read();
      data16[1] = Wire.read() << 8;
      data16[1] |= Wire.read();
    }
    else {
      byte c = (byte) Wire.read();
      data[address][pos] = c;
      pos += 1;
    }
  }
}

void loop() {
  using periodic = esp8266::polledTimeout::periodicMs;
  static periodic nextPing(1000); //timer setiap 1000ms
  if (nextPing) {
    for (byte i = 0; i < 4; i++) {
      get_i2c_data(i); // ambil data I2C dr address 0-3
    }
    if (stat != 200) {
      stat = kirim_ke_web(); //jika response sebelumnya bukan 200, maka kirim data lagi
    }
    else if (stat == 403 || stat == 404 || stat == 500) {
      Serial.println(F("Error pada website!"));
      while (true) {
      }
    }
    else {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
          if (data[i][j] != prevData[i][j] || data16[j] != prevData16[j]) {
            memcpy(prevData, data, sizeof(data));
            memcpy(prevData16, data16, sizeof(data));
            stat = kirim_ke_web();
          }
        }
      }
    }
  }
}
