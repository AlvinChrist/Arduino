//int solValveA[]={4,5,6};//Kran yang terhubung dengan pompa A
//int solValveB[]={7,8};//Kran yang terhubung dengan pompa B
//int pompaA=2;//Pompa A
//int pompaB=3;//Pompa B
bool berhenti = false;
//bool stringEnd = false;
//bool stringStart = false;
//char inputString='0';
uint64_t prevMillis = 0;

void setup()
{
  //Serial.begin(9600);
  for (int i = 4; i < 9; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}

/*void userInput() {
  if (Serial.available() > 0) {
    char inChar = char(Serial.read());
    if (inChar == '#') {
      stringEnd = true;
      stringStart = false;
    }
    if (stringStart) {
      inputString = inChar;
    }
    if (inChar == '*') {
      inputString = '0';
      stringStart = true;
    }
  }
  }*/

void loop()
{
  /*userInput();
    if (stringEnd) {
    switch (inputString) {
      case '4':
        digitalWrite(4, LOW);
        delay(5000);
        digitalWrite(4, HIGH);
        break;
      case '5':
        digitalWrite(5, LOW);
        delay(5000);
        digitalWrite(5, HIGH);
        break;
      case '6':
        digitalWrite(6, LOW);
        delay(5000);
        digitalWrite(6, HIGH);
        break;
      case '7':
        digitalWrite(7, LOW);
        delay(5000);
        digitalWrite(7, HIGH);
        break;
      case '8':
        digitalWrite(8, LOW);
        delay(5000);
        digitalWrite(8, HIGH);
        break;
    }
    inputString = '0';
    stringEnd = false;
    }*/
  while (!berhenti) {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    prevMillis = millis();
    while (true) {
      if (millis() - prevMillis >= 600000) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        break;
      }
    }
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    prevMillis = millis();
    while (true) {
      if (millis() - prevMillis >= 600000) {
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        break;
      }
    }
    berhenti = true;
  }
}
