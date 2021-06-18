#define RELAY1 9
#define RELAY2 A2
#define RELAY3 A3
#define LIMIT_ATAS 4
#define LIMIT_BAWAH 5
#define SPEED 6

unsigned long prevMillis;
char stat;
char bufferstat = 'i';
bool prevLimitAtas = HIGH;
bool prevLimitBawah = HIGH;

//up&down,limit atas,bawah,speed,pagar_atas,pagar_bawah

void setup() {
  Serial.begin(9600);
  for (int i = 2; i < 8; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  delay(200);

  if (digitalRead(LIMIT_ATAS) || digitalRead(LIMIT_BAWAH)) {
    stat = 'i';
    Serial.println("STANDBY");
  }
  else {
    digitalWrite(RELAY3, HIGH);
    Serial.println("RESET");
    while (!digitalRead(LIMIT_BAWAH)) {
      //(digitalRead(6)) ? digitalWrite(RELAY1, LOW) : digitalWrite(RELAY1, HIGH);
    }
    digitalWrite(RELAY3, LOW);
    stat = 'i';
  }
}

void loop() {
  switch (stat) {
    case 'u':
      if (!digitalRead(LIMIT_ATAS) && prevLimitAtas == HIGH) {
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, LOW);
      }
      else if (digitalRead(LIMIT_ATAS) && prevLimitAtas == LOW) {
        digitalWrite(RELAY2, LOW);
        if (bufferstat == stat) {
          bufferstat = 'i';
          stat = 'i';
        }
        else if (bufferstat != stat) {
          stat = bufferstat;
        }
        delay(5000);
      }
      prevLimitAtas = !digitalRead(LIMIT_ATAS);
      break;
    case 'd':
      if (!digitalRead(LIMIT_BAWAH) && prevLimitBawah == HIGH) {
        digitalWrite(RELAY3, HIGH);
        digitalWrite(RELAY2, LOW);
      }
      else if (digitalRead(LIMIT_BAWAH) && prevLimitBawah == LOW) {
        digitalWrite(RELAY3, LOW);
        if (bufferstat == stat) {
          bufferstat = 'i';
          stat = 'i';
        }
        else if (bufferstat != stat) {
          stat = bufferstat;
        }
        delay(5000);
      }
      prevLimitBawah = !digitalRead(LIMIT_BAWAH);
      break;
  }
  if (!digitalRead(2)) {
    prevMillis = millis();
    Serial.println("up pressed");
    while (!digitalRead(2)) {
    }
    if (millis() - prevMillis > 150) {
      Serial.println("up");
      (stat == 'i') ? stat = 'u' : bufferstat = 'u';
    }
  }
  else if (digitalRead(7)) {
    prevMillis = millis();
    Serial.println("down pressed");
    while (digitalRead(7)) {
    }
    if (millis() - prevMillis > 150) {
      Serial.println("down");
      (stat == 'i') ? stat = 'd' : bufferstat = 'd';
    }
  }
}
