String serialString;
int Jarak;
bool startScroll;
bool stringComplete;
bool stringStart;
String Var1;
String Var2;

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
    if (data == '#') {
      if (serialString != NULL) {
        Var2 = serialString;
      }
      serialString = "";
      stringStart = false;
      stringComplete = true;
    }
    else if (data == ',') {
      if (serialString != NULL) {
        Var1 = serialString;
      }
      serialString = "";
    }
    else if (data == '*') {
      serialString = "";
      stringStart = true;
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
    if (Var1 != NULL) {
      //UDR0 = 'a';
      Serial.println("Var 1: " + Var1);
      Serial.println("Var 2: " + Var2);
    }
    stringComplete = false;
  }
}
