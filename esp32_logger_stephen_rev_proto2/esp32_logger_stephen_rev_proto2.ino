#include <WiFi.h>
#include <HTTPClient.h>
#include "HardwareSerial.h"
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;//+7 jam
const int   daylightOffset_sec = 25200;

#define RXD2 16
#define TXD2 17
#define pin_trig 2
const int kontrol_pwm = 5;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;


HardwareSerial mySerial(1);
 
const char* ssid = "Pertamina";
const char* password =  "Pertamina88";

//const char* ssid = "HUAWEI-B315-3215";
//const char* password =  "123-abc-456-def";

String inputString,isi,inputString1,isi1;
boolean paket_sensor, paket_kontrol; 
char pisah[] = "";
int adc[20],control[20];
int count,count1;
int data1,data2,data3,data4;
int trig;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  mySerial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup() {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(kontrol_pwm, ledChannel);
  pinMode(pin_trig,INPUT);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  mySerial.begin(115200, SERIAL_8N1, RXD2, TXD2);
  WiFi.disconnect(true);
  delay(3000);
  paket_sensor= false;
  paket_kontrol= false;
  inputString ="";
  inputString1 ="";
  Serial.begin(115200);
  WiFi.disconnect(true); 
  konek_wifi();
  //baca_kontrol();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  delay(4000);
}

void konek_wifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void parsing(){
/*    byte index = 0;
    ptr = strtok(pisah, ";");  // takes a list of delimiters
    while(ptr != NULL)
    {
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, ";");  // takes a list of delimiters
    }
    //Serial.println(index);
// print the tokens
   for(int n = 0; n < index; n++)
   {
    Serial.println(strings[n]);
   }*/
}

void baca_serial(){
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();
    if (paket_sensor){
      inputString += inChar;
      if (inChar==';'){
        adc[count]=isi.toInt();
        Serial.println(isi);
        count++;
        isi="";
      }
      else{
        isi+=inChar;
      }
    }
    if (inChar == 'A'){
      paket_sensor = true;
      count=0;
      isi="";
    }
    if (inChar == 'B'){
      paket_sensor = false;
      Serial.println(inputString);
      inputString="";
      Serial.println("data hasil parse:");
      for(int n = 0; n < count; n++)
      {
        Serial.println(adc[n]);
      }
      kirim_sensor(adc);
    }  
    if (paket_kontrol){
      inputString1 += inChar;
      if (inChar==';'){
        control[count1]=isi1.toInt();
        Serial.println(isi1);
        count1++;
        isi1="";
      }
      else{
        isi1+=inChar;
      }
    }
    if (inChar == 'K'){
      paket_kontrol = true;
      count1=0;
      isi1="";
    }
    if (inChar == 'L'){
      paket_kontrol = false;
      Serial.println(inputString1);
      inputString1="";
      Serial.println("data hasil parse:");
      for(int n = 0; n < count1; n++)
      {
        Serial.println(control[n]);
      }
      kirim_kontrol(control[0],control[1],control[2],control[3]);
    } 
    if (inChar == 'T'){
      printLocalTime();
    } 
  }  
}

void baca_kontrol(){
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    String isi="http://kotabaru.pertaminasafeteams.com/data_1/kontrol.php?k1=10";
    http.begin(isi); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        //Serial.println(httpCode);
        int firstCommaIndex = payload.indexOf(';');
        int secondCommaIndex = payload.length();
        String param1 = payload.substring(0, firstCommaIndex);
        String param2 = payload.substring(firstCommaIndex+1, secondCommaIndex);
        firstCommaIndex = param2.indexOf(';');
        secondCommaIndex = param2.length();
        String param3 = param2.substring(0, firstCommaIndex);
        String param4 = param2.substring(firstCommaIndex+1, secondCommaIndex);
        firstCommaIndex = param4.indexOf(';');
        secondCommaIndex = param4.length();
        String param5 = param4.substring(0, firstCommaIndex);
        String param6 = param4.substring(firstCommaIndex+1, secondCommaIndex);
        
        data1 = param1.toInt(); //kontrol 1
        data2 = param3.toInt(); //kontrol 2
        data3 = param5.toInt(); //kontrol 3
        data4 = param6.toInt(); //mode
        mySerial.print("K");
        mySerial.print(data1);
        mySerial.print(";");
        mySerial.print(data2);
        mySerial.print(";");
        mySerial.print(data3);
        mySerial.print(";");
        mySerial.print(data4);
        mySerial.print(";");
        mySerial.println("L");
        ledcWrite(ledChannel, data1*2.55);
        Serial.println(data1*2.55);
        delay(15);
      }
      
    else {
      Serial.println("Error on HTTP request");
    }
    //String payload2 = http.getString();
    http.end(); //Free the resources
    //String payload3 = http.getString();
  }
}

void kirim_sensor(int a[]){
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    String isi = "http://kotabaru.pertkaminasafeteams.com/data_1/input.php?s1=";
    isi+=a[0];
    isi+="&s2=";
    isi+=a[1];
    isi+="&s3=";
    isi+=a[2];
    isi+="&s4=";
    isi+=a[3];
    isi+="&s5=";
    isi+=a[4];
    isi+="&s6=";
    isi+=a[5];
    isi+="&s7=";
    isi+=a[6];
    isi+="&s8=";
    isi+=a[7];
    isi+="&s9=";
    isi+=a[8];
    isi+="&s10=";
    isi+=a[9];
    //Serial.println(isi);
    http.begin(isi); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
    
    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      Serial.println(payload);  
    }
    else {
      Serial.print("Error on HTTP request ");
      Serial.println(httpCode);
    }
      http.end(); //Free the resources
  }
  else{
    konek_wifi();
  }
  
}

void kirim_kontrol(int a,int b, int c, int d){
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    String isi = "http://kotabaru.pertaminasafeteams.com/data_1/update_kontrol.php?k1=";
    isi+=a;
    isi+="&k2=";
    isi+=b;
    isi+="&k3=";
    isi+=c;
    isi+="&m=";
    isi+=d;
    http.begin(isi); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
  
    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      Serial.println(payload);  
    }
    else {
      Serial.print("Error on HTTP request ");
      Serial.println(httpCode);
    }
      http.end(); //Free the resources
  }
}
void loop() {
  int trig = digitalRead(pin_trig);
  if (trig == 1){
    
  }
  else{
    baca_kontrol();
  }
  baca_serial();
}
