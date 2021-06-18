#include <SPI.h>

#define SS 10

byte stick_mode, prev_mode;
byte stick_status;
byte byte4, byte5, byte6, byte7, byte8, byte9;

void setup()
{
  Serial.begin(9600);
  delay(200);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(SS, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  delay(100);
  digitalWrite(SS, LOW);
  delay(1);
  SPI.transfer(0x01);
  delay(1);
  stick_mode = SPI.transfer(0x42);
  delay(1);
  stick_status = SPI.transfer(0);
  delay(1);
  digitalWrite(SS, HIGH);
  delay(1);
  (stick_status == 90) ? Serial.println("Controller Ready") : Serial.println("No controller detected");
  (stick_mode == 115) ? Serial.println("Mode Analog") : (stick_mode == 65) ? Serial.println("Mode Digital") : 0;
  prev_mode = stick_mode;
}


void loop() {
  digitalWrite(SS, LOW);
  delay(1);
  SPI.transfer(0x01);
  delay(1);
  stick_mode = SPI.transfer(0x42);
  delay(1);
  stick_status = SPI.transfer(0);
  delay(1);
  byte4 = SPI.transfer(0);
  delay(1);
  byte5 = SPI.transfer(0);
  delay(1);
  byte6 = SPI.transfer(0);
  delay(1);
  byte7 = SPI.transfer(0);
  delay(1);
  byte8 = SPI.transfer(0);
  delay(1);
  byte9 = SPI.transfer(0);
  delay(1);
  digitalWrite(SS, HIGH);
  delay(1);
  switch (byte4) {
    case 223:
      Serial.println("RIGHT");
      break;
    case 127:
      Serial.println("LEFT");
      break;
    case 239:
      Serial.println("UP");
      break;
    case 191:
      Serial.println("DOWN");
      break;
    case 253:
      Serial.println("L3");
      break;
    case 251:
      Serial.println("R3");
      break;
    case 247:
      Serial.println("START");
      break;
    case 254:
      Serial.println("SELECT");
      break;
  }
  switch (byte5) {
    case 191:
      Serial.println("X");
      break;
    case 127:
      Serial.println("PETAK");
      break;
    case 239:
      Serial.println("SEGITIGA");
      break;
    case 223:
      Serial.println("BULAT");
      break;
    case 247:
      Serial.println("R1");
      break;
    case 253:
      Serial.println("R2");
      break;
    case 251:
      Serial.println("L1");
      break;
    case 254:
      Serial.println("L2");
      break;
  }
  if (stick_mode != prev_mode) {
    (stick_mode == 115) ? Serial.println("Mode Analog") : (stick_mode == 65) ? Serial.println("Mode Digital") : 0;
    prev_mode=stick_mode;
  }
  delay(10);
}
