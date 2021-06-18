#include <SoftwareSerial.h>
SoftwareSerial module_bluetooth(0, 1); // pin RX | TX

byte data = 0;
void setup()
{
  Serial.begin(38400);
  pinMode(LED_BUILTIN, OUTPUT);  //inisialisasi LED menjadi output
}
void loop()
{
  if (Serial.available() > 0)
  {
    data = Serial.read();
    Serial.println(data);    //Pembacaan dan ditampilkan data yang masuk
    //Data yang masuk
    if (data == 120) {
      digitalWrite(LED_BUILTIN, HIGH); 
    }
    else if (data == 0)
      digitalWrite(LED_BUILTIN, LOW);
  }

}
