#define RESET_PIN 8
#define RCK 10
#define off 0b111111111111111111111111
#define on  0b000000000000000000000000
#include <SPI.h>

void setup() {
  Serial.begin(9600);
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN,HIGH);
  pinMode(RCK, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  delay(100);
  res();
  Serial.println(F("reset"));
  delay(1000);
}

void res() {
  digitalWrite(RESET_PIN, LOW);
  digitalWrite(RESET_PIN, HIGH);
  delay(50);
  
}
void hidup(int c) {
  digitalWrite(RESET_PIN, LOW);
  digitalWrite(RESET_PIN, HIGH);
  delay(50);
  digitalWrite(RCK, LOW);
  if (c == 0) {
    SPI.transfer((0b111111111111111100000000 & 0x0000FF));
    Serial.println(F("1"));
  }
  else if (c == 1) {
    SPI.transfer((0b111111110000000011111111 & 0x00FF00) >> 8);
    Serial.println(F("2"));
  }
  else {
    SPI.transfer((0b000000001111111111111111 & 0xFF0000) >> 16);
    Serial.println(F("3"));
  }
  digitalWrite(RCK, HIGH);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    hidup(i);
    delay(1000);
  }
  res();
  delay(1000);
}
