#include <Wire.h>
uint32_t n=0;

void setup(){
  Wire.begin();        
  Serial.begin(9600);  

  Serial.println(F("-------------------------------------I am the Master"));
  delay(1000);
}


void loop()
{
  Wire.requestFrom(7,8);
  (n<11)?n++:n=0;
  delay(1000);
}
