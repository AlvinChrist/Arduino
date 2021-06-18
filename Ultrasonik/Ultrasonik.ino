#include <eRCaGuy_Timer2_Counter.h>

#define TRIG_DEPAN 4
#define TRIG_BELAKANG 6
#define ECHO_1 2
#define ECHO_2 3
#define ECHO_3 5
#define ECHO_4 7

unsigned long timer[4];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  timer2.setup();
  pinMode(TRIG_DEPAN, OUTPUT);
  pinMode(TRIG_BELAKANG, OUTPUT);
  pinMode(ECHO_1, INPUT_PULLUP);
  pinMode(ECHO_2, INPUT_PULLUP);
  pinMode(ECHO_3, INPUT_PULLUP);
  pinMode(ECHO_4, INPUT_PULLUP);
  digitalWrite(TRIG_DEPAN, LOW);
  digitalWrite(TRIG_BELAKANG, LOW);
  delay(500);
}

void measure() {
  unsigned long start_time;
  bool prevStat1 = LOW, prevStat2 = LOW, prevStat3 = LOW, prevStat4 = LOW;
  bool sensor1, sensor2, sensor3, sensor4;
  digitalWrite(TRIG_DEPAN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG_DEPAN, LOW);
  start_time = millis();
  while ((unsigned long)(millis() - start_time) < 38) {
    sensor1 = digitalRead(ECHO_1);
    if (sensor1 && !prevStat1) {
      timer[0] = timer2.get_micros();
    }
    else if (!sensor1 && prevStat1) {
      timer[0] = timer2.get_micros() - timer[0];
    }
    prevStat1 = sensor1;
    sensor2 = digitalRead(ECHO_2);
    if (sensor2 && !prevStat2) {
      timer[1] = timer2.get_micros();
    }
    else if (!sensor2 && prevStat2) {
      timer[1] = timer2.get_micros() - timer[1];
    }
    prevStat2 = sensor2;
  }
  delayMicroseconds(20);
  digitalWrite(TRIG_BELAKANG, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG_BELAKANG, LOW);
  start_time = millis();
  while ((unsigned long)(millis() - start_time) < 38) {
    sensor3 = digitalRead(ECHO_3);
    if (sensor3 && !prevStat3) {
      timer[2] = timer2.get_micros();
    }
    else if (!sensor3 && prevStat3) {
      timer[2] = timer2.get_micros() - timer[2];
    }
    prevStat3 = sensor3;
    sensor4 = digitalRead(ECHO_4);
    if (sensor4 && !prevStat4) {
      timer[3] = timer2.get_micros();
    }
    else if (!sensor4 && prevStat4) {
      timer[3] = timer2.get_micros() - timer[3];
    }
    prevStat4 = sensor4;
  }
  //for (byte i = 0; i < 4; i++) {
    //(timer[i] > 38000) ? timer[i] = 0 : 0;
  //}
}

void loop() {
  // put your main code here, to run repeatedly
  measure();
  Serial.print("*");
  for (int i = 0; i < 4; i++) {
    unsigned int jarak;
    jarak = timer[i] * 0.343 / 2;
    Serial.print(jarak);
    (i < 3) ? Serial.print(",") : 0;
  }
  Serial.println("#");
  delay(330);
}
