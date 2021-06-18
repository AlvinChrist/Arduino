#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_Black_16.h>
#define DISPLAYS_WIDE 3
#define DISPLAYS_HIGH 1
SoftDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH);
DMD_TextBox box(dmd);
/* TIP: If you want a longer string here than fits on your display, just define the display DISPLAYS_WIDE value to be wider than the
  number of displays you actually have.
*/
String Team2;
String serialString;
String Team1;
int Jarak;
bool startScroll;
bool stringComplete;
bool stringStart;

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
    if (serialString != NULL) {
      Team2 = serialString;
    }
    serialString = "";
    stringStart = false;
    stringComplete = true;
  }
  else if (data == ',') {
    Team1 = serialString;
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

void setup() {
  initSerial();
  delay(500);
  dmd.setBrightness(20);
  dmd.selectFont(Arial_Black_16);
  delay(500);
  dmd.begin();
}

void printText() {
  dmd.clearScreen();
  if (dmd.stringWidth(Team2) > 96) {
    delay(100);
    startScroll = true;
  }
  else
  {
    delay(100);
    startScroll = false;
    Jarak = (96 - dmd.stringWidth(Team2)) / 2;
    dmd.drawString(Jarak, 0, Team2);
  }
}

void loop() {
  if (startScroll) {
    for (int x = 0; x <= Team2.length(); x++) {
      box.print(Team2[x]);
      delay(100);
    }
    box.print(" ");
    delay(10);
    box.print(" ");
    delay(100);
  }
  if (stringComplete) {
    if (Team2 != NULL) {
      UDR0 = 'a';
    }
    stringComplete = false;
  }
}
