#include <avr/wdt.h>
#define SENSOR 4

const unsigned long i = 1800L * 1000;
unsigned long prevMillis = 0;
bool tutup = false;

void wdtOn() {
  MCUSR &= ~(1 << WDRF);
  MCUSR = MCUSR & B11110111;
  WDTCSR = WDTCSR | B00011000;
  WDTCSR = (1 << WDP3);
  WDTCSR = WDTCSR | B01000000;
  MCUSR = MCUSR & B11110111;
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(SENSOR, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  MCUSR &= ~(1 << WDRF);
  wdtOn();
  //Serial.print("reset");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(4));
  wdt_reset();
  if (tutup == false) {
    if (digitalRead(SENSOR)) {
      //Serial.println("baca");
      prevMillis = millis();
      while (millis() - prevMillis <= 2000) {
        wdt_reset();
      }
      if (digitalRead(SENSOR)) {
        wdt_reset();
        tutup = true;
        digitalWrite(5, HIGH);  
        //Serial.println("tutup");
        prevMillis = millis();
      }
    }
  }
  else {
    if (millis() - prevMillis >= i) {
      prevMillis = millis();
      //Serial.println("buka"); 
      digitalWrite(5, LOW);
      tutup = false;
      delay(1000);
    }
  }
}
