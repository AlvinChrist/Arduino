int inputPin = 5;
int ledPin = 4;
int pirState = LOW;
int val = 0;
void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(ledPin,OUTPUT);
  pinMode(inputPin,INPUT);

}

void loop() {
  val = digitalRead(inputPin);
  if(val==HIGH){
    digitalWrite(ledPin,HIGH);
    if(pirState==LOW){
      Serial.println("Motion Detected!");
      pirState = HIGH;
    }
  }
  if(val==LOW){
    digitalWrite(ledPin,LOW);
    if(pirState==HIGH){
      Serial.println("Motion Ended!");
      pirState = LOW;
    }
  }
}
