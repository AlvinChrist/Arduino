#define TRIG_DEPAN 4
#define TRIG_BELAKANG 6
#define ECHO_1 2
#define ECHO_2 3
#define ECHO_3 5
#define ECHO_4 7

long duration[4];
float distance[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TRIG_DEPAN, OUTPUT);
  pinMode(TRIG_BELAKANG, OUTPUT);
  pinMode(ECHO_1, INPUT_PULLUP);
  pinMode(ECHO_2, INPUT_PULLUP);
  pinMode(ECHO_3, INPUT_PULLUP);
  pinMode(ECHO_4, INPUT_PULLUP);
  digitalWrite(TRIG_DEPAN, LOW);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG_DEPAN, HIGH );
  delayMicroseconds( 20 );
  digitalWrite(TRIG_DEPAN, LOW );
  duration[0] = pulseIn(ECHO_1, HIGH, 38000);
  distance[0] = duration[0] * 0.0343 / 2;
  Serial.print("SENSOR 1 = ");
  Serial.print(distance[0]);
  Serial.println(" cm\t");
  delay(500);
}
