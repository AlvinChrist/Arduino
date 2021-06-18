#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_Black_16.h>
#define DISPLAY_WIDTH 2
#define DISPLAY_HEIGHT 1
SoftDMD dmd(DISPLAY_WIDTH, DISPLAY_HEIGHT, 10, 11, 12, 13, 14, 15);
DMD_TextBox box(dmd);
/* TIP: If you want a longer string here than fits on your display, just define the display DISPLAYS_WIDE value to be wider than the
  number of displays you actually have.
*/
uint32_t msglength;
String pesan = "Silahkan Masuk";
bool startScroll;
void setup() {
  delay(500);
  dmd.setBrightness(100);
  dmd.selectFont(Arial_Black_16);
  delay(100);
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  dmd.begin();
}

void printText(String msg) {
  dmd.clearScreen();
  uint32_t Jarak;
  msglength = dmd.stringWidth(msg);
  if (msglength > 64) {
    startScroll = true;
  }
  else
  {
    delay(100);
    startScroll = false;
    Jarak = (64 - msglength) / 2;
    dmd.drawString(Jarak, 0, msg);
  }
}

void loop() {
  if (startScroll) {
    for (int x = 0; x <= pesan.length() * 2; x++) {
      box.print(pesan[x]);
      delay(10);
    }
  }
  printText(pesan);
}
