//#include <SoftwareSerial.h>

#define Sensor 2   //Input Pin
#define LED 13     // Led pin for Indication
bool flag, stat;
void setup() {
  Serial.begin(9600);
  //mySerial.begin(9600);
  pinMode (Sensor, INPUT_PULLUP);  //Define Pin as input
  pinMode (LED, OUTPUT);    //Led as OUTPUT
  Serial.println("Waiting for motion");
  delay(5000);
}

void loop() {
  stat = !digitalRead(Sensor);
  (!stat && flag) ? Serial.println("movement") : (stat && !flag) ? Serial.println("") : 0 ;
  flag = stat;
}
