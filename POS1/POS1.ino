int posisi = 0;
int standby = 0;
bool prevStatus;
bool button;
void setup() {
  // put your setup code here, to run once:
  pinMode(12, INPUT_PULLUP); // Initialize sensor that uses digital pins 13 and 12.
  pinMode(2, OUTPUT);//UV DAN POMPA
  pinMode(3, OUTPUT);//LAMPU PENERANGAN
  pinMode(4, OUTPUT);//GREEN
  pinMode(5, OUTPUT);//RED
  pinMode(6, OUTPUT); //SIRENE
  Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}

void loop() {
  // Every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters.
  if (!digitalRead(12)) {
    digitalWrite(3, HIGH);
    if (posisi <= 10) {
      posisi += 1;
    }
  }
  else
  {
    digitalWrite(3, LOW);
    posisi = 0;
    if (standby <= 300) {
      standby += 1;
    }
  }
  if (posisi >= 5) {
    if (standby >= 200) {
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      mulai2();
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
    }
    else
    {
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      mulai1();
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
    }
    standby = 0;
    posisi = 0;
  }
  delay(200);
  // put your main code here, to run repeatedly:
}

void mulai1() {
  Serial.println("Semprot Mode 1");
  digitalWrite(6, HIGH); //SIRENE
  delay(500);
  digitalWrite(6, LOW); //SIRENE
  digitalWrite(2, HIGH);
  delay (5000);
  digitalWrite(2, LOW);
  digitalWrite(6, HIGH);
  delay(1000);
  while (!digitalRead(12)) {
  }
  digitalWrite(6, LOW);
}

void mulai2() {
  Serial.println("Semprot Mode 2");
  digitalWrite(2, HIGH);
  digitalWrite(6, HIGH); //SIRENE
  delay(400);
  digitalWrite(6, LOW); //SIRENE
  delay(400);
  digitalWrite(6, HIGH); //SIRENE
  delay(400);
  digitalWrite(6, LOW); //SIRENE
  delay(400);
  digitalWrite(6, HIGH); //SIRENE
  delay(400);
  digitalWrite(6, LOW); //SIRENE
  delay (6000);
  digitalWrite(2, LOW);
  digitalWrite(6, HIGH);
  delay(1000);
  while (!digitalRead(12)) {
  }
  digitalWrite(6, LOW); //SIRENE
}
