#include <SimpleTimer.h>
bool stringComplete;
bool stringStart;
String inputString;
String serialString;
SimpleTimer timer;

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

ISR(USART_RX_vect) {
  char data;
  data = UDR0;
  if (data == '#') {
    stringComplete = true;
    stringStart = false;
    return;
  }
  if (stringStart) {
    serialString += data;
  }
  if (data == '*') {
    stringStart = true;
  }
  if (data == 10) {
    inputString = serialString;
    serialString = "";
    stringStart = false ;
  }
}

void shootClock() {

}

void setup() {
  // put your setup code here, to run once:
  initSerial();
  timer.setInterval(5000, shootClock);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  timer.run();
  if (stringComplete) {
    if (inputString == "TAMBAH A") {
      
    }
    else if (inputString == "KURANG A") {

    }
    else if (inputString == "RESET A") {

    }
    else if (inputString == "TAMBAH B") {

    }
    else if (inputString == "KURANG B") {

    }
    else if (inputString == "RESET B") {

    }
    else if (inputString == "SHOOTCLOCK") {

    }
    stringComplete = false;
  }
}
