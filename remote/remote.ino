
/****************************************
   200 PROYEK ARDUINO
   Membaca Data 4 Ch remote Control Modul YK04
   Input : Remote Control
   Output : Serial Monitor
   22 Januari 2019
   Status : OK
   www.tokotronik.com
 ******************************/
bool prevState;
byte c;
void setup() {
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600); // setting baud rate
  Serial.println("Membaca Data 4 Ch Remote Control");
}
void flash(){
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
}
void loop() {
  if (digitalRead(2) == HIGH) { // Button B pressed
    if (prevState == LOW) {
      flash();
      Serial.println("Tombol yang anda tekan : B");
      c = 2;
    }
  }
  if (digitalRead(3) == HIGH && prevState == LOW ) { // Button D pressed
    if (prevState == LOW) {
      flash();
      Serial.println("Tombol yang anda tekan : D");
      c = 3;
    }
  }
  if (digitalRead(4) == HIGH && prevState == LOW) { // Button A pressed
    if (prevState == LOW) {
      flash();
      Serial.println("Tombol yang anda tekan : A");
      c = 4;
    }
  }
  if (digitalRead(5) == HIGH && prevState == LOW) { // Button C pressed
    if (prevState == LOW) {
      flash();
      Serial.println("Tombol yang anda tekan : C");
      c = 5;
    }
  }
  (c > 0) ? prevState = digitalRead(c) : 0;
}
