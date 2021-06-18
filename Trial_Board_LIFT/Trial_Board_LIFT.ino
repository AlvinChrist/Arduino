#define RELAY1 9
#define RELAY2 A2
#define RELAY3 A3
#define LIMIT_ATAS 4
#define LIMIT_BAWAH 5
#define SPEED 6
byte data;
bool prevStatus = false;
unsigned long prevMillis;
int buttonArr[] = {2, 3, 4, 5, 6, 7, 8};// tombol down&up,limit atas,bawah,speed

void setup() {
  Serial.begin(9600);
  for (int i = 2; i < 9; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
}

void readInput() {
  for (int i = 0; i < 8; i++) {
    data = bitWrite(data, i, !digitalRead(buttonArr[i]));
  }
  /*if (!sensorBawah) {
    digitalWrite(RELAY3, LOW);
    }
    else if (!sensorAtas) {
    digitalWrite(RELAY2, LOW);
    }

    (!sensorSpeed) ? digitalWrite(RELAY1, HIGH) : digitalWrite(RELAY1, LOW);
  */
}

void loop() {
  //readInput();
  char inChar = (char)Serial.read();
  Serial.println(digitalRead(LIMIT_ATAS));
  if (!digitalRead(6)) {
  }
  switch (inChar) {
    case 'd':
      //if (sensorBawah == HIGH && sensorAtas == LOW) { //turun ke bawah
      Serial.println("down");
      digitalWrite(RELAY3, HIGH);
      digitalWrite(RELAY2, LOW);
      /*prevMillis = millis();
        while (digitalRead(2) == LOW) {
        //readInput();
        }
        if (millis() - prevMillis > 200) {
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, LOW);
        }*/
      break;
    case 'u':
      //if (sensorBawah == LOW && sensorAtas == HIGH) { //naik ke atas
      //prevMillis = millis();
      Serial.println("up");
      digitalWrite(RELAY3, LOW);
      digitalWrite(RELAY2, HIGH);
      /*while (digitalRead(3) == LOW) {
        //readInput();
        }
        if (millis() - prevMillis > 200) {
        digitalWrite(RELAY3, HIGH);
        digitalWrite(RELAY2, LOW);
        }

        }*/
      break;
    case 's':
      Serial.println("stop");
      digitalWrite(RELAY3, LOW);
      digitalWrite(RELAY2, LOW);
      break;

    case 'a':
      digitalWrite(RELAY1, HIGH);
      delay(2000);
      digitalWrite(RELAY1, LOW);
      /*case 4:
        prevMillis = millis();
        while (digitalRead(4) == LOW) {
          readInput();
        }
        if (millis() - prevMillis > 200) {
          Serial.println("B3");
        }
        break;
        case 8:
        prevMillis = millis();
        while (digitalRead(5) == LOW) {
          readInput();
        }
        if (millis() - prevMillis > 200) {
          Serial.println("B4");
        }
        break;
        case 16:
        prevMillis = millis();
        while (digitalRead(6) == LOW) {
          readInput();
        }
        if (millis() - prevMillis > 200) {
          Serial.println("B5");
        }
        break;
        case 32:
        prevMillis = millis();
        while (digitalRead(7) == LOW) {
          readInput();
        }
        if (millis() - prevMillis > 200) {
          Serial.println("B6");
        }
        break;
        case 64:
        prevMillis = millis();
        while (digitalRead(8) == LOW) {
          readInput();
        }
        if (millis() - prevMillis > 200) {
          Serial.println("B7");
        }
        break;
      */
  }
  //(digitalRead(6)) ? digitalWrite(RELAY1, LOW) : digitalWrite(RELAY1, HIGH);
}
