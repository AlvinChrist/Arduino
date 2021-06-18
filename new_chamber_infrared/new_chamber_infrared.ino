#define RED 2
#define GREEN 3
#define BUZZER 4
#define lampuTL 5
#define POMPA 6
#define BUTTON A4
#define SENSOR 7

uint64_t prevMillis;
uint64_t oneMin;
uint64_t btnMillis;
bool sensor;
bool lastState = HIGH;
bool button;

void Sanitize(bool s = false) {
  prevMillis = millis();
  while (!sensor || s == true) {
    sensor = digitalRead(SENSOR);
    if (millis() - prevMillis >= 1500 || s == true) {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      if (millis() - oneMin >= 60000) {
        digitalWrite(lampuTL, HIGH);
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
        digitalWrite(lampuTL, HIGH);
        digitalWrite(POMPA, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
        delay(4500);
      }
      digitalWrite(POMPA, LOW);
      digitalWrite(lampuTL, LOW);
      digitalWrite(BUZZER, HIGH);
      oneMin = millis();
      prevMillis = millis();
      delay(1000);
      sensor = digitalRead(SENSOR);
      if (!s) {
        while (!sensor) {
          sensor = digitalRead(SENSOR);
        }
      }
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
      digitalWrite(BUZZER, LOW);
      s = false;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(lampuTL, OUTPUT);
  pinMode(POMPA, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);
  digitalWrite(BUZZER, LOW);
  digitalWrite(GREEN, HIGH);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor = digitalRead(SENSOR);
  if (!sensor) {
    Sanitize();
  }
  button = digitalRead(BUTTON);
  if (lastState == HIGH && button == LOW) {
    btnMillis = millis();
  }
  else if (lastState == LOW && button == HIGH) {
    if (millis() - btnMillis > 100) {
      Sanitize(true);
    }
    /*
      else if (millis() - btnMillis >= 1000) {
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(200);
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(200);
      digitalWrite(lampuTL, HIGH);
      delay(5000);
      digitalWrite(lampuTL, LOW);
      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);
      }
    */
  }
  lastState = button;
}
