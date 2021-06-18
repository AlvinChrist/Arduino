#if 1
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define ORIENTATION 1 //0 PORTRAIT 1 LANDSCAPE
#define MINPRESSURE 200
#define MAXPRESSURE 1000
#define BUFFPIXEL 20

//for kbv
#define XP 6
#define XM A2
#define YP A1
#define YM 7

#define BMPIMAGEOFFSET 54
#define BUFFPIXEL      20
#define PALETTEDEPTH   0


#include <MCUFRIEND_kbv.h>
#include <SD.h>
#include <SPI.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;
byte i = 0;
bool idle = false;
byte arr[3] = {1, 2, 15};
uint64_t prevMillis = 0;
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
  uint8_t bmpDepth, sdbuffer[3 * BUFFPIXEL], bitmask, bitshift;        // Bit depth (currently must be 24
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
  bmpFile.close();
}
bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP,OUTPUT);
  pinMode(XM,OUTPUT);
  digitalWrite(YP,HIGH);
  digitalWrite(XM,HIGH);  
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    //pixel_x =  map(p.x, 94, 922, 0, 240);//PORTRAIT
    //pixel_y = map(p.y, 85, 897, 0, 320);

    pixel_x = map(p.y, 175, 909, 0, 320); //LANDSCAPE kbv
    pixel_y = map(p.x, 193,937,0, 240);
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


void nope() {
}

void tambahKurang(bool t) {
  //tft.fillRect((tft.width() - 15 * String(i).length()) / 2, tft.height()/ 4, String(i).length() * 14, 14, BLACK);
  tft.fillRect((320 - 11 * String(arr[i]).length()) / 2, 150, 40, 14, BLACK);
  t ? (i == 2) ? i = 0 : i += 1 : (i == 0) ? i = 2 : i -= 1;
  //tft.setCursor((tft.width() - 15 * String(i).length()) / 2, tft.height() / 4);
  tft.setCursor((320 - 11 * String(arr[i]).length()) / 2, 150);
  tft.print(arr[i]);
  delay(300);
}

void Home() {
  clr();
  showmsg(52, 10, 2, GREENYELLOW, "UV BOX By DOMOTECH");
  tft.drawFastHLine(0, 10 + 20, tft.width(), OLIVE);
  showBMP("/dm.bmp", 120, 40);
  on_btn.initButton(&tft, 220, 170 , 40, 40, BLUE, BLUE, WHITE, ">", 2);
  off_btn.initButton(&tft, 100, 170 , 40, 40, BLUE, BLUE, WHITE, "<", 2);
  start_btn.initButton(&tft, 260, 220, 90, 40, OLIVE, OLIVE, NAVY, "Mulai", 2);
  info_btn.initButton(&tft, 60, 220, 90, 40, DARKGREY, DARKGREY, BLUE, "Info", 2);
  on_btn.drawButton();
  off_btn.drawButton();
  start_btn.drawButton();
  info_btn.drawButton();
  showmsg(130, 170, 2, GREEN, "menit");
  tft.setCursor((320 - 10 * String(i).length()) / 2, 150);
  tft.print(arr[i]);
}
void Info() {
  clr();
  //back_btn.initButton(&tft, tft.width() * 1 / 8, tft.height() * 29 / 32, 60, 40, BLACK, DARKGREY, BLACK, "<-", 3);
  back_btn.initButton(&tft, 40, 218, 60, 40, BLACK, DARKGREY, BLACK, "<-", 3);
  back_btn.drawButton();
  //showmsg((tft.width() - 12 * 14) / 2, tft.height() * 2 / 5, 2, GREENYELLOW , "---DOMOTECH---");
  showmsg(33, 76, 3, GREENYELLOW , "---DOMOTECH---");
  //tft.drawFastHLine(0, tft.height() * 2 / 5 + 20, tft.width(), OLIVE);
  tft.drawFastHLine(0, 106, 320, OLIVE);
  //showmsg((tft.width() - 12 * 19) / 2 * ORIENTATION, tft.height() * 2 / 5 + 30 * 1, 2, WHITE, "Call Us 081807928888");
  showmsg(46, 126, 2, WHITE, "Call Us 081807928888");
  //tft.setCursor((tft.width() - 12 * 19) / 2 * ORIENTATION, tft.height() * 2 / 5 + 30 * 2);
  tft.setCursor(46, 156);
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

void setup(void)
{
  DDRD |= (1 << DDD0) ; // pin 0 -> SSR ->output
  DDRD &= ~(1 << DDD1); // pin 1 -> sensor -> input
  PORTD &= ~(1 << PORTD0); //pin SSR = LOW
  PORTD |= (1 << PORTD1); // pin sensor = PULLUP

  delay(2000);
  while(!SD.begin(10)) {
  }
  tft.begin(tft.readID());
  tft.setRotation(ORIENTATION);
  Home();
}
void loop(void)
{
  bool down = Touch_getXY();
  if (millis() - prevMillis >= 60000 && idle == false) {
    tft.fillScreen(BLACK);
    idle = true;
  }
  else if (down && idle) {
    idle = false;
    prevMillis = millis();
    Home();
  }
  else {
    start_btn.press(down && start_btn.contains(pixel_x, pixel_y));
    info_btn.press(down && info_btn.contains(pixel_x, pixel_y));
    off_btn.press(down && off_btn.contains(pixel_x, pixel_y));
    on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
    (on_btn.justPressed()) ? tambahKurang(1) : nope();
    (off_btn.justPressed()) ? tambahKurang(0) : nope();
    (info_btn.justPressed()) ? Info() : nope();
    if (start_btn.justPressed() && ((PIND & (1 << PIND1)) >> PIND1) == 0) {
      clr();
      //showmsg((tft.width() - 12 * 18) / 2, tft.height() * 1 / 2 - 21, 2, RED, "Proses Sterilisasi");
      showmsg(52, 99, 2, RED, "Proses Sterilisasi");
      //showmsg((tft.width() - 12 * 18) / 2, tft.height() * 1 / 2, 2, RED, "sedang berlansung..");
      showmsg(52, 120, 2, RED, "sedang berlansung..");
      PORTD |= (1 << PORTD0);
      prevMillis = millis();
      while (((PIND & (1 << PIND1)) >> PIND1) == 0) {
        //while (1) {
        if (millis() - prevMillis >= 60000 * arr[i]) {
          prevMillis = millis();
          break;
        }
      }
      PORTD &= ~(1 << PORTD0);
      clr();
      //showmsg((tft.width() - 12 * 15) / 2, tft.height() * 1 / 2 - (21 / 2), 2, GREEN, "Proses Selesai!");
      showmsg(70, 110, 2, GREEN, "Proses Selesai!");
      delay(3000);
      Home();
    }
  }
}
#endif
