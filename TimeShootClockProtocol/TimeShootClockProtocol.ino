#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#define DISPLAYS_WIDE 1
#define DISPLAYS_HEIGHT 2
SoftDMD dmd(DISPLAYS_WIDE, DISPLAYS_HEIGHT);
DMD_TextBox box(dmd);

String serialString;
bool stringComplete;
bool stringStart;
int VarArray[3];
int Pos;
int Jarak;

void initSerial() {
  // USART initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART Receiver: On
  // USART Transmitter: On
  // USART0 Mode: Asynchronous
  // USART Baud Rate: 9600 (Double Speed Mode)
  UCSR0A = 0x02;
  UCSR0B = 0x98;
  UCSR0C = 0x06;
  UBRR0H = 0x00;
  UBRR0L = 0xCF;
}
/*
  ISR(USART_RX_vect) {
  char data;
  data = UDR0;
  if (data == ')') {
    if (serialString != NULL) {
      ShootClock = serialString;
    }
    serialString = "";
    stringStart = false;
    stringComplete = true;
  }
  else if (data == ',') {
    if (serialString != NULL) {
      Time = serialString;
    }
    serialString = "";
  }
  else if (data == '(') {
    serialString = "";
    stringStart = true;
    stringComplete = false;
  }
  else if (stringStart) {
    serialString += data;
  }
  }
*/

void setup() {
  //initSerial();
  Serial.begin(9600);
  delay(100);
  dmd.setBrightness(150);
  dmd.selectFont(SystemFont5x7);
  delay(100);
  dmd.begin();
}

void serialEvent() {
  while (Serial.available()) {
    char data = (char)Serial.read();
    if (data == ')') {
      if (Pos == 2) {
        VarArray[Pos] = serialString.toInt();
      }
      serialString = "";
      stringStart = false;
      stringComplete = true;
    }
    else if (data == ',') {
      if (Pos <= 2) {
        VarArray[Pos] = serialString.toInt();
      }
      Pos += 1;
      serialString = "";
    }
    else if (data == '(') {
      serialString = "";
      stringStart = true;
      Pos = 0;
      stringComplete = false;
    }
    else if (stringStart) {
      serialString += data;
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  if (stringComplete) {
    char dmdBuff[6];
    dmd.clearScreen();
    sprintf(dmdBuff,"%02u:%02u",VarArray[0],VarArray[1]);
    dmd.drawString(0,0,dmdBuff);
    Jarak = (32 - dmd.stringWidth(String(VarArray[2]))) / 2;
    dmd.drawString(Jarak, 16, String(VarArray[2]));
    Serial.println("Time " + String(dmdBuff));
    Serial.println("ShootClock " + String(VarArray[2]));
    Serial.println();
    stringComplete = false;
  }
}
