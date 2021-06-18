#define Mic 2
#define Endless 3
#define Loop 4

byte a=0;
bool stringStart;
bool stringEnd;
char inputString;
unsigned long previousMillis;

void setup() {
  Serial.begin(9600);
  pinMode(Mic, OUTPUT);
  pinMode(Loop, OUTPUT);
  pinMode(Endless, OUTPUT);
}

void userInput() {
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case '#':
        stringEnd = true;
        stringStart = false;
        break;
      case '*':
        inputString = "";
        stringStart = true;
        break;
    }
    if (stringStart) {
      inputString = inChar;
    }
  }
}

void record(){
  bool stat=true;
  previousMillis = millis();
  while (stat){
    unsigned long currentMillis = millis();
    if(currentMillis-previousMillis>=10000){
      previousMillis=currentMillis;
      digitalWrite(Mic,LOW);
      Serial.println("Done");
      stat=false;
    }
    else{
      digitalWrite(Mic,HIGH);
    }
  }
}

void playLoop(bool stat){
   if(stat){
    digitalWrite(Loop,HIGH);
   }
   else{
    digitalWrite(Loop,LOW);
   }
}

void playEndless(){
  bool stat=true;
  digitalWrite(Endless,HIGH);
  delay(5);
  digitalWrite(Endless,LOW);
}

void loop() {
  userInput();
  if (stringEnd) {
    Serial.println(inputString);
    switch (inputString) {
      case 'a':
        Serial.print("Recording... ");
        record();
        break;
      case 'b':
        Serial.println("Playing recording in Endless mode");
        playEndless();
        break;
      case 'c':
        switch(a){
          case 0:
            Serial.print("Playing in loop mode... ");
            playLoop(true);
            a=1;
            break;
          case 1:
            Serial.println("Stop");
            playLoop(false);
            a=0;
            break;
        }
        break;
    }
    inputString = "";
    stringEnd = false;
  }

}
