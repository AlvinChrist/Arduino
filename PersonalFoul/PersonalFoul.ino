String serialString;
bool stringComplete;
bool stringStart;
int Pos = 0;
int PersonalFoul[24];

//HOME 0-11; AWAY 12-23
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
    if (data == '>') {
      if (Pos == 23) {
        PersonalFoul[Pos] = serialString.toInt();
      }
      serialString = "";
      stringStart = false;
      stringComplete = true;
    }
    else if (data == ',') {
      if ((Pos <= 24) && (serialString.toInt() <= 5)) {
        PersonalFoul[Pos] = serialString.toInt();
      }
      Pos += 1;
      serialString = "";
    }
    else if (data == '<') {
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
    for (int i = 0; i < 24; i++) {
      Serial.println("Player " + String(i + 1) + " : "  + PersonalFoul[i]);
    }
    Serial.println();
    stringComplete = false;
  }
}
