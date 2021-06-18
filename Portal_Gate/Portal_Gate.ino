int NTS = 7;      // Sensor Wave
int P1 = 5;   // Portal 1 Open
int P2 = 6; // Portal 2 Open
int counter = 0;
boolean stat_kirim = false;
boolean stat_dapat = false;
boolean stat_buka = false;
boolean stat_baca = true;

boolean sensor = false;
//boolean gate1 = false;
//boolean gate2 = false;
byte data = 0;


void setup() {
  Serial.begin(115200);
  pinMode(NTS, INPUT);  // sets the pin as output
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  
}

void loop() {
  sensor = digitalRead(NTS);
  //gate1 = digitalRead(P1);
  //gate2 = digitalRead(P2);
  if (stat_baca) {
    if (!sensor) {
      stat_kirim = true;
    }
  }
  if (stat_kirim) {
    stat_baca=false;
    stat_kirim = false;
    stat_buka = true;
    counter = 0;
    Serial.println ("(buka)");
    //delay(1000);
  }
  if (stat_dapat) {
    stat_dapat = false;
    //Serial.println(data);
    if (data == 49) {
      digitalWrite(P1, HIGH);
      delay (300);
      digitalWrite(P1, LOW);
      stat_buka = false;
      //stat_baca = true;
      counter=40000;
    }
    if (data == 50) {
      digitalWrite(P2, HIGH);
      delay (300);
      digitalWrite(P2, LOW);
      stat_buka = false;
    }
    data = 0;
  }
  if (stat_buka) {
    counter += 1;
  }
  if (counter >= 50000) {
    stat_baca = true;
    stat_buka=false;
  }
}

void serialEvent() {
  byte inbyte = 0;
  if (Serial.available())
  {
    inbyte = Serial.read();
    if (inbyte >= 49) {
      data = inbyte;
      stat_dapat = true;
    }
  }
}
