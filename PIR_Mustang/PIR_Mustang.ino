int inputPin = 2;
int pirState = LOW;
int val = 0;
void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(inputPin,INPUT_PULLUP);
}

void loop() {
  digitalWrite(LED_BUILTIN,digitalRead(inputPin));
  /*
  val = digitalRead(inputPin);
  if(val==HIGH){
    digitalWrite(LED_BUILTIN,HIGH);
    if(pirState==LOW){
      Serial.println("Motion Detected!");
      pirState = HIGH;
    }
  }
  if(val==LOW){
    digitalWrite(LED_BUILTIN,LOW);
    if(pirState==HIGH){
      Serial.println("Motion Ended!");
      pirState = LOW;
    }
  }*/
}
