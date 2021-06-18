//pin 4-7 RELAY
#define R1 4
#define R2 5

#define R3 6
#define R4 7

#define RST 9
#define RTS 10

#define Limit_tangga_buka A4 //limit buka motor induksi
#define Limit_tangga_tutup A3 //limit tutup motor induksi

#define Limit_tangga_turun A2 //limit buka motor2
#define Limit_pintu_tutup A1 //limit tutup motor2

#define Finger A0

bool lastState = HIGH;
bool stringStart = false;
bool stringEnd = false;
bool toggle = false;
char inputString;
uint64_t prevMillis = 0;
uint64_t buttonMillis = 0;
byte stat = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 4; i < 11; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (int i = 11; i < 19; i++) {
    pinMode(i, INPUT);
  }
}

void buka_pintu() {
  if (digitalRead(Limit_pintu_tutup) == HIGH) {//pintu tutup
    digitalWrite(R2, HIGH);
    Serial.println("buka Pintu");
    prevMillis = millis();
    while (millis() - prevMillis <= 20000) {
      readUserInput();
      if (stringEnd) {
        if (inputString == 's') {
          break;
        }
      }
    }
    digitalWrite(R2, LOW);
  }
  else {
    digitalWrite(R1, HIGH);
    Serial.println("tutup pintu");
    while (digitalRead(Limit_pintu_tutup) == LOW) {//pintu buka
      readUserInput();
      if (stringEnd) {
        if (inputString == 's') {
          break;
        }
      }
    }
    digitalWrite(R1, LOW);
  }
}
void buka() {
  if (digitalRead(Limit_pintu_tutup) == HIGH || stat == 1) {
    stat = 1;
    digitalWrite(R2, HIGH);
    Serial.println("buka Pintu");
    prevMillis = millis();
    while (millis() - prevMillis <= 20000) {
      readUserInput();
      if (stringEnd) {
        if (inputString == 's') {
          break;
        }
      }
    }
    digitalWrite(R2, LOW);
    delay(1000);
    if (digitalRead(Limit_tangga_tutup) == HIGH || stat == 2) { //tangga tertutup ,
      stat = 2;
      digitalWrite(RTS, HIGH);
      Serial.println("membuka");
      while (digitalRead(Limit_tangga_buka) == LOW) {
        readUserInput();
        if (stringEnd) {
          if (inputString == 's') {
            break;
          }
        }
      }
      digitalWrite(RTS, LOW);
      delay(2000);
      if (digitalRead(Limit_tangga_turun) == HIGH || stat == 3) {
        stat = 3;
        digitalWrite(R4, HIGH);//turun
        Serial.println("turun");
        prevMillis = millis();
        while (millis() - prevMillis <= 45000) {
          readUserInput();
          if (stringEnd) {
            if (inputString == 's') {
              break;
            }
          }
        }
        digitalWrite(R4, LOW);
      }
    }
  }
}

void tangga_lipat(bool t) {
  if (t) {
    if (digitalRead(Limit_tangga_tutup) == HIGH) { //tangga tertutup ,
      digitalWrite(RTS, HIGH);
      Serial.println("membuka");
      while (digitalRead(Limit_tangga_buka) == LOW) {
        readUserInput();
        if (stringEnd) {
          if (inputString == 's') {
            break;
          }
        }
      }
      digitalWrite(RTS, LOW);
      // tangga terlipat
    }
  }
  else {
    if (digitalRead(Limit_tangga_buka) == HIGH) {//tangga terbuka ,RST = Naikkan tangga
      digitalWrite(RST, HIGH);
      Serial.println("menutup");
      while (digitalRead(Limit_tangga_tutup) == LOW) {
        readUserInput();
        if (stringEnd) {
          if (inputString == 's') {
            break;
          }
        }
      }
      digitalWrite(RST, LOW);
    }
  }
}
void tangga_turun() {
  if (digitalRead(Limit_tangga_turun) == HIGH) {
    digitalWrite(R4, HIGH);//turun
    Serial.println("turun");
    prevMillis = millis();
    while (millis() - prevMillis <= 45000) {
      readUserInput();
      if (stringEnd) {
        if (inputString == 's') {
          break;
        }
      }
    }
    digitalWrite(R4, LOW);
  }
  else {
    digitalWrite(R3, HIGH);//naik
    Serial.println("naik");
    while (digitalRead(Limit_tangga_turun) == LOW) {
      readUserInput();
      if (stringEnd) {
        if (inputString == 's') {
          break;
        }
      }
    }
    digitalWrite(R3, LOW);
  }
}

void tutup() {
  if (digitalRead(Limit_tangga_turun) == LOW || stat == 1) {
    stat = 1;
    digitalWrite(R3, HIGH);//naik
    Serial.println("naik");
    while (digitalRead(Limit_tangga_turun) == LOW) {
      readUserInput();
      if (stringEnd) {
        if (inputString == 's') {
          break;
        }
      }
    }
    delay(2000);
    digitalWrite(R3, LOW);
    if (digitalRead(Limit_tangga_buka) == HIGH || stat == 2) { //tangga terbuka ,RTS = Naikkan tangga
      stat = 2;
      digitalWrite(RST, HIGH);
      Serial.println("menutup");
      while (digitalRead(Limit_tangga_tutup) == LOW) {
        readUserInput();
        if (stringEnd) {
          if (inputString == 's') {
            break;
          }
        }
      }
      digitalWrite(RST, LOW);
    }
    delay(2000);
    if (digitalRead(Limit_pintu_tutup) == LOW || stat == 3) {
      stat = 3;
      digitalWrite(R1, HIGH);
      Serial.println("tutup pintu");
      while (digitalRead(Limit_pintu_tutup) == LOW) {//pintu buka
        readUserInput();
        if (stringEnd) {
          if (inputString == 's') {
            break;
          }
        }
      }
      digitalWrite(R1, LOW);
    }
  }
}
void readUserInput() {
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case ')':
        stringEnd = true;
        stringStart = false;
        break;
      case '(':
        inputString = '0';
        stringStart = true;
        break;
      default:
        if (stringStart)
          inputString = inChar;
    }
  }
}

void readTombol() {
  if (digitalRead(Finger) == LOW && lastState == HIGH) {
    buttonMillis = millis();
  }
  else if (digitalRead(Finger) == HIGH && lastState == LOW) {
    if (millis() - buttonMillis >= 1000) {
      if (digitalRead(Limit_tangga_turun) == HIGH) {
        buka();
        Serial.println("buka");
      }
      else {
        tutup();
        Serial.println("tutup");
      }
    }
  }
  lastState = digitalRead(Finger);
}
void loop() {
  readUserInput();
  readTombol();
  if (stringEnd) {
    if (inputString == 's') {
      Serial.println("stop");
      for (int i = 4; i < 11; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
      }
    }
    else if (inputString == 'p') {
      Serial.println("pintu");
      buka_pintu();
    }
    else if (inputString == 't') {
      Serial.println("tangga");
      tangga_turun();
    }
    else if (inputString == 'a') {
      tangga_lipat(true);
    }
    else if (inputString == 'b') {
      tangga_lipat(false);
    }
    else if (inputString == 'x') {
      buka();
    }
    else if (inputString == 'y') {
      tutup();
    }
    stringEnd = false;
  }
//  Serial.println(digitalRead(Limit_tangga_buka));
  //delay(100);
}
