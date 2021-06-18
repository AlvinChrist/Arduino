  #define POMPA 5
#define BUTTON 2
bool lastState;
bool btn;
uint64_t prevMillis;
uint64_t oneMin;

void setup() {
  pinMode(POMPA,OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);        
}

void loop() {
 btn=digitalRead(BUTTON);
 if(btn==LOW && lastState==HIGH){
   prevMillis=millis();
 }
 else if(btn==HIGH && lastState==LOW){
  if(millis()-prevMillis>100){
    delay(1000);
    digitalWrite(POMPA,HIGH);
    if(millis()-oneMin>60000){
      delay(8000);
    }
    else{
      delay(5000);
    }
    digitalWrite(POMPA,LOW);
  }
 }
 lastState=btn;
 oneMin=millis();
}
