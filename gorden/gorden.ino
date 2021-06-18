const int Ena = 4;
const int Pul = 2;
const int Dir = 3;
const int Sen = 5;
const int Stat = 6;
const int del = 700;
const int delh = 700;
int Pos = 2000;
int Pos_lalu;
int delta;

void setup() {
  pinMode(Ena, OUTPUT);
  pinMode(Pul, OUTPUT);
  pinMode(Dir, OUTPUT);
  pinMode(Sen, INPUT_PULLUP);
  pinMode(Stat, INPUT);
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:

  digitalWrite(Pul, HIGH);
  digitalWrite(Dir, HIGH);
  digitalWrite(Ena, HIGH);
  while (!digitalRead(Sen)) {
    digitalWrite(Pul, LOW );
    delayMicroseconds(del);
    digitalWrite(Pul, HIGH);
    delayMicroseconds(delh);
  }
  Pos = 0;
  Pos_lalu = 0;
  digitalWrite(Ena, LOW);
}

void loop() {
  if (digitalRead(Stat)) {
    Pos = 0;

  }
  else {
    Pos = 2200;
  }
  if (Pos > Pos_lalu) {
    digitalWrite(Ena, HIGH);
    digitalWrite(Dir, LOW);
    delta = Pos - Pos_lalu;
    for (int i = 0; i <= delta; i++) {
      digitalWrite(Pul, LOW);
      delayMicroseconds(del);
      digitalWrite(Pul, HIGH);
      delayMicroseconds(delh);
    }
    digitalWrite(Ena, LOW);
    Pos_lalu = Pos;
    Serial.println("maju");
  }
  if (Pos < Pos_lalu) {
    digitalWrite(Ena, HIGH);
    digitalWrite(Dir, HIGH);
    delta = Pos_lalu - Pos;
    for (int i = 0; i <= delta; i++) {
      digitalWrite(Pul, LOW);       
      delayMicroseconds(del);
      digitalWrite(Pul, HIGH);
      delayMicroseconds(delh);
    }
    digitalWrite(Ena, LOW);
    Pos_lalu = Pos;
    Serial.println("mundur");
  }
  Serial.println(digitalRead(6));
}
