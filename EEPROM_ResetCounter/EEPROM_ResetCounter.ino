#include <EEPROM.h>
bool resetCounter;
bool lastState;
uint64_t resetMillis;
void setup() {
  Serial.begin(9600);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  Serial.println(EEPROM.read(0));
}

void loop() {
  resetCounter = digitalRead(17);
  if (lastState == HIGH && resetCounter == LOW) {
     resetMillis= millis();
  }
  else if (lastState == LOW && resetCounter == HIGH) {
    if (millis() - resetMillis >= 2000) {
      EEPROM.update(0, 0);
      delay(1000);
      Serial.println(EEPROM.read(0)); 
    }
  }
  lastState = resetCounter;

}
