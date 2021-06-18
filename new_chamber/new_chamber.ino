#include <Wire.h>
#include <VL53L0X.h>

#define lampuTL 5
#define RED 2
#define GREEN 3
#define BUZZER 4
#define POMPA 6
#define BUTTON 10

uint64_t prevMillis;
uint64_t oneMin;
uint64_t btnMillis;
bool lastState = HIGH;
bool button;
int range_thres;
byte count;
VL53L0X sensor;

uint32_t jarak;

void Sanitize(bool s = false) {
  prevMillis = millis();
  while (jarak < 380 && jarak > 290 || s == true) {
    jarak = sensor.readRangeSingleMillimeters();
    (jarak < 380 && jarak > 290 || s == true) ? digitalWrite(lampuTL, HIGH) : digitalWrite(lampuTL, LOW);
    if (millis() - prevMillis >= 1500 || s == true) {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      if (millis() - oneMin >= 60000) {
        digitalWrite(POMPA, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(400);
        digitalWrite(BUZZER, LOW);
        delay(400);
        digitalWrite(BUZZER, HIGH);
        delay(400);
        digitalWrite(BUZZER, LOW);
        delay(400);
        digitalWrite(BUZZER, HIGH);
        delay(400);
        digitalWrite(BUZZER, LOW);
        delay(6000);
      }
      else {
        digitalWrite(POMPA, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
        delay(4500);
      }
      digitalWrite(POMPA, LOW);
      delay(1000);
      digitalWrite(BUZZER, HIGH);
      jarak = sensor.readRangeSingleMillimeters();
      oneMin = millis();
      prevMillis = millis();
      jarak = sensor.readRangeSingleMillimeters();
      if (!s) {
        while (jarak < 380 && jarak > 290) {
          Serial.println(jarak);
          jarak = sensor.readRangeSingleMillimeters();
        }
      }
      digitalWrite(GREEN, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
      digitalWrite(BUZZER, LOW);
      digitalWrite(lampuTL, LOW);
      s = false;
    }
  }
}

void (* resetFunc)(void) = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  sensor.setTimeout(500);
  sensor.setSignalRateLimit(0.5);
  sensor.setMeasurementTimingBudget(20000);
  if (!sensor.init()) {
    //Serial.println("ERROR SENSOR");
    digitalWrite(lampuTL,HIGH);
  }
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(lampuTL, OUTPUT);
  pinMode(POMPA, OUTPUT);
  digitalWrite(GREEN, HIGH);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  jarak = sensor.readRangeSingleMillimeters();
  Serial.println(jarak);
  if (jarak < 380 && jarak > 320) {
    Serial.println(jarak);
    Sanitize();
  }
  else if (jarak == 65535) {
    resetFunc();
  }
  button = digitalRead(BUTTON);
  if (lastState == HIGH && button == LOW) {
    btnMillis = millis();
  }
  else if (lastState == LOW && button == HIGH) {
    if (millis() - btnMillis > 100) {
      Sanitize(true);
    }
  }
  lastState = button;
}
