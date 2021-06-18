#define CLOCK_PIN 3
#define DATA_PIN 2
#define BIT_COUNT 10
#define cs 4
#define RPM 5

uint64_t prevMillis;
uint16_t reading, prevReading;
uint16_t rotation, rpm, prevRpm;
char data[13], prevData[13];
/*const int CLOCK_PIN = 3;
  const int DATA_PIN = 2;
  const int BIT_COUNT = 10;
  const int cs = 4;
*/

void setup() {
  //setup our pins
  cli();
  PCICR |= 0b00000100;
  PCMSK2 |= 0b00100000;
  sei();
  Serial.begin(115200);

  pinMode(DATA_PIN, INPUT_PULLUP);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(cs, OUTPUT);
  //give some default values
  digitalWrite(CLOCK_PIN, HIGH);
  digitalWrite(cs, HIGH);
}

ISR(PCINT2_vect) {
  rotation += 1;
}

void loop() {
  if (millis() - prevMillis >= 200) {
    memset(data, 0, sizeof(data));
    reading = map(readPosition(), 0, 1023, 0, 359);
    prevMillis = millis();
    rotation=rotation/2;
    rpm = (rotation/(millis()-prevMillis)) * 60000;
    sprintf(data, "*%d,%d,#", reading, rpm);
    rotation = 0;
    if (memcmp(data, prevData, sizeof(data))) {
      memset(prevData, 0, sizeof(prevData));
      memcpy(&prevData, &data, sizeof(data));
      Serial.print(prevData);
    }
  }
}

//read the current angular position
int readPosition() {
  // Read the same position data twice to check for errors
  unsigned long sample1 = shiftIn(DATA_PIN, CLOCK_PIN, BIT_COUNT);
  delayMicroseconds(25);  // Clock mus be high for 20 microseconds before a new sample can be taken
  return sample1;
}

//read in a byte of data from the digital input of the board.
unsigned long shiftIn(const int data_pin, const int clock_pin, const int bit_count) {
  unsigned long data = 0;
  digitalWrite(cs, LOW);
  delayMicroseconds(1);
  for (int i = 0; i < bit_count; i++) {
    data <<= 1;
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(1);
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(1);

    data |= digitalRead(data_pin);
  }
  digitalWrite(cs, HIGH);
  delayMicroseconds(1);
  return data;
}
