#define latchPin 7
#define clockPin 5
#define dataPin 4
#define resetPin 6

char inputString = '0';
bool stringEnd = false;
bool stringStart = false;

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  off();
}

void on() {
  digitalWrite(latchPin, LOW);
  long int led = 0b000000000000000000000000;
  shiftOut(dataPin, clockPin, MSBFIRST, led >> 16);
  shiftOut(dataPin, clockPin, MSBFIRST, led >> 8);
  shiftOut(dataPin, clockPin, MSBFIRST, led);
  digitalWrite(latchPin, HIGH);
}

void off() {
  digitalWrite(latchPin, LOW);
  long int led = 0b111111111111111111111111;
  shiftOut(dataPin, clockPin, MSBFIRST, led >> 16);
  shiftOut(dataPin, clockPin, MSBFIRST, led >> 8);
  shiftOut(dataPin, clockPin, MSBFIRST, led);
  digitalWrite(latchPin, HIGH);
}

void mode1() {
  off();
  Serial.println("Mode 1");
  long int led = 0b111111111111111111111111;
  for (int i = 0; i < 24; i++) {
    led = led * 2;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, led >> 16);
    shiftOut(dataPin, clockPin, MSBFIRST, led >> 8);
    shiftOut(dataPin, clockPin, MSBFIRST, led);
    digitalWrite(latchPin, HIGH);
    delay(200);
  }
}

void mode2() {
  off();
  Serial.println("Mode 2");
  long int led = 0b111111111111111111111111;
  for (int i = 0; i < 24; i++) {
    led = led / 2;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, led >> 16);
    shiftOut(dataPin, clockPin, MSBFIRST, led >> 8);
    shiftOut(dataPin, clockPin, MSBFIRST, led);
    digitalWrite(latchPin, HIGH);
    delay(200);
  }
}

void mode3() {
  off();
  Serial.println("Mode 3");
  long int led1 = 0b111111111111111111111111;
  long int led2 = 0b111111111111111111111111;
  for (int i = 0; i < 13; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (led1 & led2) >> 16);
    shiftOut(dataPin, clockPin, MSBFIRST, (led1 & led2) >> 8);
    shiftOut(dataPin, clockPin, MSBFIRST, (led1 & led2));
    digitalWrite(latchPin, HIGH);
    led1 = (led1 << 1);
    led2 = (led2 >> 1);
    delay(200);
  }
}

void mode4() {
  off();
  Serial.println("Mode 4");
  unsigned long int led1=0b111111111110111111111111;
  unsigned long int led2=0b111111111111011111111111;
  for (int i = 1; i < 14; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (led1 & led2) >> 16);
    shiftOut(dataPin, clockPin, MSBFIRST, (led1 & led2) >> 8);
    shiftOut(dataPin, clockPin, MSBFIRST, (led1 & led2));
    digitalWrite(latchPin, HIGH);
    Serial.println(led1&led2,BIN);
    led1 = led1>>1;
    led2 = led2<<1;;
    delay(200);
  }
}
void userInput() {
  if (Serial.available() > 0) {
    char inChar = char(Serial.read());
    if (inChar == '#') {
      stringEnd = true;
      stringStart = false;
    }
    if (stringStart) {
      inputString = inChar;
    }
    if (inChar == '*') {
      inputString = '0';
      stringStart = true;
    }
  }
}
void loop() {
  userInput();
  if (stringEnd) {
    switch (inputString) {
      case '1':
        mode1();
        break;
      case '2':
        mode2();
        break;
      case '3':
        mode3();
        break;
      case '4':
        mode4();
        break;
      case 'o':
        on();
        break;
      case 'c':
        off();
        break;
    }
    inputString = '0';
    stringEnd = false;
  }
}
