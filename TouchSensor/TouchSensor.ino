#define SENSOR_PIN 2
int lastState = HIGH;
int currentState;
unsigned long previousMillis;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  currentState = digitalRead(SENSOR_PIN);
  if (lastState == HIGH && currentState == LOW) {
    previousMillis=currentMillis;
    Serial.println("The sensor is touched");
    digitalWrite(LED_BUILTIN, currentState);
  }
  else if (lastState == LOW && currentState == HIGH) {
    Serial.println("The sensor is is released");
    digitalWrite(LED_BUILTIN, currentState);
    if(currentMillis-previousMillis >= 2000){
      Serial.println("Long touch");
    }
    else{
      Serial.println("Short Touch");
    }
  }
  // save the the last state
  lastState = currentState;
}
