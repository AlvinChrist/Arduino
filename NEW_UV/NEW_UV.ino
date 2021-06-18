#if 1
#define BUZZER 0
#define ORIENTATION 0 //0 PORTRAIT 1 LANDSCAPE
#define UV 1
#define SENSOR A6
#define MINPRESSURE 200
#define MAXPRESSURE 1000
#define BUFFPIXEL 20
#define XP 8
#define XM A2
#define YP A3
#define YM 9
#define BMPIMAGEOFFSET 54
#define BUFFPIXEL      20
#define PALETTEDEPTH   0

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SD.h>
#include <SPI.h>
#include <TouchScreen.h>
MCUFRIEND_kbv tft;


byte i = 1;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_GFX_Button on_btn, off_btn, start_btn, back_btn, info_btn;
int pixel_x, pixel_y;     //Touch_getXY() updates global vars

uint16_t read16(File& f) {
  uint16_t result;         // read little-endian
  f.read((uint8_t*)&result, sizeof(result));
  return result;
}

uint32_t read32(File& f) {
  uint32_t result;
  f.read((uint8_t*)&result, sizeof(result));
  return result;
}

uint8_t showBMP(char *nm, int x, int y)
{
  File bmpFile;
  int bmpWidth, bmpHeight, w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;    // W+H in pixels
  uint8_t bmpDepth, sdbuffer[3 * BUFFPIXEL], bitmask, bitshift, ret;        // Bit depth (currently must be 24
  uint32_t bmpImageoffset, rowSize, pos;  // Start of image data in file
  uint16_t bmpID, n, lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
  boolean flip = true;        // BMP is stored bottom-to-top
  boolean is565 = false;      //

  if ((x >= tft.width()) || (y >= tft.height()))
    return 1;               // off screen

  bmpFile = SD.open(nm);      // Parse BMP header
  bmpID = read16(bmpFile);    // BMP signature
  (void) read32(bmpFile);     // Read & ignore file size
  (void) read32(bmpFile);     // Read & ignore creator bytes
  bmpImageoffset = read32(bmpFile);       // Start of image data
  (void) read32(bmpFile);     // Read & ignore DIB header size
  bmpWidth = read32(bmpFile);
  bmpHeight = read32(bmpFile);
  n = read16(bmpFile);        // # planes -- must be '1'
  bmpDepth = read16(bmpFile); // bits per pixel
  pos = read32(bmpFile);      // format
  if (bmpID != 0x4D42) ret = 2; // bad ID
  else if (n != 1) ret = 3;   // too many planes
  else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
  else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette
  else {
    bool first = true;
    is565 = (pos == 3);               // ?already in 16-bit format
    // BMP rows are padded (if needed) to 4-byte boundary
    rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
    if (bmpHeight < 0) {              // If negative, image is in top-down order.
      bmpHeight = -bmpHeight;
      flip = false;
    }
    w = bmpWidth;
    h = bmpHeight;
    if ((x + w) >= tft.width())       // Crop area to be loaded
      w = tft.width() - x;
    if ((y + h) >= tft.height())      //
      h = tft.height() - y;

    if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
      bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
      bitmask = 0xFF;
      if (bmpDepth < 8)
        bitmask >>= bmpDepth;
      bitshift = 8 - bmpDepth;
      n = 1 << bmpDepth;
      lcdbufsiz -= n;
      palette = lcdbuffer + lcdbufsiz;
      for (col = 0; col < n; col++) {
        pos = read32(bmpFile);    //map palette to 5-6-5
        palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
      }
    }

    // Set TFT address window to clipped image bounds
    tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
    for (row = 0; row < h; row++) { // For each scanline...
      // Seek to start of scan line.  It might seem labor-
      // intensive to be doing this on every line, but this
      // method covers a lot of gritty details like cropping
      // and scanline padding.  Also, the seek only takes
      // place if the file position actually needs to change
      // (avoids a lot of cluster math in SD library).
      uint8_t r, g, b, *sdptr;
      int lcdidx, lcdleft;
      if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
      else        // Bitmap is stored top-to-bottom
        pos = bmpImageoffset + row * rowSize;
      if (bmpFile.position() != pos) { // Need seek?
        bmpFile.seek(pos);
        buffidx = sizeof(sdbuffer); // Force buffer reload
      }

      for (col = 0; col < w; ) {  //pixels in row
        lcdleft = w - col;
        if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
        for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
          uint16_t color;
          // Time to read more pixel data?
          if (buffidx >= sizeof(sdbuffer)) { // Indeed
            bmpFile.read(sdbuffer, sizeof(sdbuffer));
            buffidx = 0; // Set index to beginning
            r = 0;
          }
          b = sdbuffer[buffidx++];
          g = sdbuffer[buffidx++];
          r = sdbuffer[buffidx++];
          color = tft.color565(r, g, b);
          lcdbuffer[lcdidx] = color;
        }
        tft.pushColors(lcdbuffer, lcdidx, first);
        first = false;
        col += lcdidx;
      }           // end cols
    }               // end rows
    tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
    ret = 0;        // good render
  }
  bmpFile.close();
  return (ret);
}

bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  DDRC = DDRC | B00001100;
  PORTC = PORTC | B0001100;
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x =  map(p.x, 94, 922, 0, 240);//PORTRAIT
    pixel_y = map(p.y, 85, 897, 0, 320);

    //pixel_x = map(p.y, 85, 897, 0, 320); //LANDSCAPE
    //pixel_y = map(p.x, 922, 94, 0, 240);
  }
  return pressed;
}

void showmsg(int x , int y, byte sz, uint16_t c, const char *msg) {
  tft.setCursor(x, y);
  tft.setTextSize(sz);
  tft.setTextColor(c);
  tft.print(msg);
  delay(50);
}

void clr() {
  tft.fillScreen(BLACK);
}

void Info() {
  clr();
  //back_btn.initButton(&tft, tft.width() * 1 / 8, tft.height() * 29 / 32, 60, 40, BLACK, DARKGREY, BLACK, "<-", 3);
  back_btn.initButton(&tft, 30, 290, 60, 40, BLACK, DARKGREY, BLACK, "<-", 3);
  back_btn.drawButton();
  //showmsg((tft.width() - 12 * 14) / 2, tft.height() * 2 / 5, 2, GREENYELLOW , "---DOMOTECH---");
  showmsg(12, 128, 3, GREENYELLOW , "--DOMOTECH--");
  //tft.drawFastHLine(0, tft.height() * 2 / 5 + 20, tft.width(), OLIVE);
  tft.drawFastHLine(0, 158, 250, OLIVE);
  //showmsg((tft.width() - 12 * 19) / 2 * ORIENTATION, tft.height() * 2 / 5 + 30 * 1, 2, DARKGREY, "Call Us 081807928888");
  showmsg(0, 168, 2, DARKGREY, "Call Us 081807928888");
  //tft.setCursor((tft.width() - 12 * 19) / 2 * ORIENTATION, tft.height() * 2 / 5 + 30 * 2);
  tft.setCursor(0, 198);
  tft.print("IG @domotechbymeraki");
  while (true) {
    bool down = Touch_getXY();
    back_btn.press(down && back_btn.contains(pixel_x, pixel_y));
    if (back_btn.justPressed()) {
      Home();
      break;
    }
  }
}
void nope() {
}

void tambahKurang(bool t) {
  //tft.fillRect((tft.width() - 15 * String(i).length()) / 2, tft.height()/ 4, String(i).length() * 17, 21, BLACK);
  tft.fillRect((240 - 15 * String(i).length()) / 2, 160, 60, 21, BLACK);
  t ? i += 1 : i -= 1;
  //tft.setCursor((tft.width() - 15 * String(i).length()) / 2, tft.height() / 4);
  tft.setCursor((240 - 15 * String(i).length()) / 2, 160);
  tft.print(i);
  delay(100);
}

void Home() {
  clr();
  showmsg(12, 10, 2, GREENYELLOW, "UV BOX By DOMOTECH");
  tft.drawFastHLine(0, 10 + 20, tft.width(), OLIVE);
  showBMP("/dm.bmp", 80, 50);
  on_btn.initButton(&tft, 180, 230 , 40, 40, BLUE, BLUE, WHITE, "+", 2);
  off_btn.initButton(&tft, 60, 230 , 40, 40, BLUE, BLUE, WHITE, "-", 2);
  start_btn.initButton(&tft, 180, 280, 90, 40, OLIVE, OLIVE, NAVY, "Mulai", 2);
  info_btn.initButton(&tft, 60, 280, 90, 40, DARKGREY, DARKGREY, BLUE, "Info", 2);
  on_btn.drawButton();
  off_btn.drawButton();
  start_btn.drawButton();
  info_btn.drawButton();
  showmsg(90, 180, 2, GREEN, "menit");
  tft.setCursor((120 - 6 * String(i).length()), 160);
  tft.print(i);
}

void setup(void)
{
  DDRD = DDRD | B00000011; //pin 0 dan 1 as output
  PORTD = B00000001; // pin 0 HIGH and pin 1 LOW
  tft.begin(tft.readID());
  tft.setRotation(ORIENTATION);
  SD.begin(10);
  Home();
}

void loop(void)
{
  bool down = Touch_getXY();
  start_btn.press(down && start_btn.contains(pixel_x, pixel_y));
  info_btn.press(down && info_btn.contains(pixel_x, pixel_y));
  off_btn.press(down && off_btn.contains(pixel_x, pixel_y));
  on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
  (on_btn.justPressed()) ? tambahKurang(1) : nope();
  (off_btn.justPressed() && i > 1) ? tambahKurang(0) : nope();
  (info_btn.justPressed()) ? Info() : nope();
  if (start_btn.justPressed()) {
    //if (start_btn.justPressed() && analogRead(SENSOR) < 512) {
    clr();
    //showmsg((tft.width() - 12 * 18) / 2, tft.height() * 1 / 2 - 21, 2, RED, "Proses Sterilisasi");
    showmsg(12, 139, 2, RED, "Proses Sterilisasi");
    //showmsg((tft.width() - 12 * 18) / 2, tft.height() * 1 / 2, 2, RED, "sedang berlansung..");
    showmsg(12, 160, 2, RED, "sedang berlansung..");
    PORTD = B00000011;
    uint64_t uvMillis = millis();
    // while (analogRead(SENSOR) < 512) {
    while (1) {
      if (millis() - uvMillis >= 60000 * i) {
        break;
      }
    }
    PORTD = B00000000;
    clr();
    //showmsg((tft.width() - 12 * 15) / 2, tft.height() * 1 / 2 - (21 / 2), 2, GREEN, "Proses Selesai!");
    showmsg(30, 150, 2, GREEN, "Proses Selesai!");
    delay(2500);
    PORTD = PORTD ^ B00000001;
    Home();
  }
}
#endif
