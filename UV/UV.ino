#define SENSOR_PIN 9
#define BUZZER A6
#define UV 2

unsigned long previousMillis = 0;
bool sensor;
int lastState = HIGH;
int currentState;

void setup() {
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT); //buzzer
  digitalWrite(BUZZER, HIGH);
  pinMode(UV, OUTPUT); //UV
  delay(100);
}

void shortbuzzer() { //for buzzer
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, LOW);
    delay(100);
    digitalWrite(BUZZER, HIGH);
    delay(100);
  }
}

void buzzer2() { //for buzzer
  digitalWrite(BUZZER, LOW);
  delay(700);
  digitalWrite(BUZZER, HIGH);
  delay(300);
}

void buzzer1() {
  for (int i = 0; i < 2; i++) {
  digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(BUZZER, HIGH);
    delay(300);
  }
}

void UV1() {
  digitalWrite(UV, HIGH);
  delay(60000);
  digitalWrite(UV, LOW);
  shortbuzzer();
}

void UV2() {
  digitalWrite(UV, HIGH);
  delay(30000);
  digitalWrite(UV, LOW);
  shortbuzzer();
}

void loop() {
  unsigned long currentMillis = millis();
  currentState = digitalRead(SENSOR_PIN);
  if (lastState == HIGH && currentState == LOW) {
    previousMillis = currentMillis;
    Serial.println("The sensor is touched");
    digitalWrite(LED_BUILTIN, !currentState);
  }
  else if (lastState == LOW && currentState == HIGH) {
    Serial.println("The sensor is is released");
    digitalWrite(LED_BUILTIN, !currentState);
    if (currentMillis - previousMillis >= 2000) {
      previousMillis=currentMillis;
      Serial.println("Long touch");
      buzzer1();
      UV1();
    }
    else if(currentMillis-previousMillis >=100 && currentMillis-previousMillis <=2000){
      previousMillis=currentMillis;
      Serial.println("Short Touch");
      buzzer2();
      UV2();
    }
  }
  // save the the last state
  lastState = currentState;
}
