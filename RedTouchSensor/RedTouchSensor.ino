int lastState = LOW;
int sensor;
void setup() {
  pinMode(3, INPUT);
  Serial.begin(9600);

}

void loop() {
  sensor = digitalRead(3);
  switch (lastState) {
    case 0:
      switch (sensor) {
        case 1:
          Serial.println("TOUCHED");
          lastState = sensor;
          break;
      }
      break;
    case 1:
      switch (sensor) {
        case 0:
          Serial.println("RELEASED");
          lastState = sensor;
          break;
      }
      break;
  }

}
