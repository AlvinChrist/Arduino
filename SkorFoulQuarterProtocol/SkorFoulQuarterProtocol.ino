String serialString;
bool stringComplete;
bool stringStart;
int SkorFoulQuarter[5];
int Pos = 0;
/*void initSerial() {
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

  ISR(USART_RX_vect) {
  char data;
  data = UDR0;
  if (data == '#') {
    serialString = "";
    stringStart = false;
    stringComplete = true;
  }
  else if (data == ',') {
    Var1 = serialString;
    serialString = "";
  }
  else if (stringStart) {
    serialString += data;
  }
  else if (data == '*') {
    serialString = "";
    stringStart = true;
    stringComplete = false;
  }
  }
*/

void setup() {
  //initSerial();
  Serial.begin(9600);
  delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    char data = (char)Serial.read();
    if (data == '?') {
      if (Pos == 4) {
        SkorFoulQuarter[Pos] = serialString.toInt();
      }
      serialString = "";
      stringStart = false;
      stringComplete = true;
    }
    else if (data == ',') {
      if (Pos <= 5) {
        SkorFoulQuarter[Pos] = serialString.toInt();
      }
      Pos += 1;
      serialString = "";
    }
    else if (data == '!') {
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
  if (stringComplete) {
    Serial.println("Score 1 :" + String(SkorFoulQuarter[0]));
    Serial.println("Score 2 :" + String(SkorFoulQuarter[1]));
    Serial.println("Foul 1 :" + String(SkorFoulQuarter[2]));
    Serial.println("Foul 2 :" + String(SkorFoulQuarter[3]));
    Serial.println("Quarter :" + String(SkorFoulQuarter[4]));
    Serial.println();
    stringComplete = false;
  }
}
