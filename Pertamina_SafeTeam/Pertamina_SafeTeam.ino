#include "max6675.h"
#include <Wire.h>
#include "VL53L0X.h"

#define RPM 2
#define thermoDO 5
#define thermoCS 3
#define thermoCLK 7
#define LED_KUNING 34
#define LED_MERAH 35
#define LED_BIRU 36
#define LED_HIJAU 37
#define dir 42
#define pul 40

//#define RELAY1 A7
//#define RELAY2 A5

#define MACHINE_BATTERY A0
#define STARTER_BATTERY A2
//#define STARTER_BATTERY_2 A2
//#define PRESSURE A3   // select the input pin for Fuel Sensor
#define POMPA A5
//#define RELAY2 A5
//#define LIMIT1 A11
//#define LIMIT2 A9

#define pressPin A3
#define startPin 4
#define SYSTEM_BATTERY A1
#define sendPin 6
#define stepPin A11
//#define stepPin A10
#define onoffPin A7
//#define onoffPin A11


float enginetemp = 0, pressure=0;
int system_batt=0, machine_batt=0, starter_batt=0;
int counstep = 0;
//int fuelvalue = 0;  // variable to store the value coming from the sensor
int fuellevel = 0;
//int battvalue = 0;
//int battlevel = 0;
int mode = 1;
int timemode = 0;
int enginethrotle = 0;
int throtlepos = 0;
int target = 0;
int targetlalu = 0;
int eror = 0;
int pressvalue = 0;
int rpmmesin = 0;
int dataposisi = 0;
int baca = 0;
int data1[5];
int data2[5];

boolean mulaiser1 = false;
boolean mulaiser2 = false;
boolean siapser1 = false;
boolean siapser2 = false;
boolean reqstart1 = false;  

String inputweb[11];
String inputlora[11];
byte hitung2 = 0;
byte hitung1 = 0;

boolean reqstart = false;
boolean reqstop = false;
boolean remote = false;
boolean reqkirim = false;
boolean statmesin = false;
boolean ulang = false;

unsigned long timing=0;
unsigned long lasttiming = 0;
unsigned long currenttiming=0;

int konst=1;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
VL53L0X fuelsensor = VL53L0X();

void setup() {
  Serial.begin(9600);
  Serial.println(F("Pertamina Safe Team"));
  delay(500);
  pinMode(48,OUTPUT);
  digitalWrite(48,LOW);
  pinMode(49,INPUT_PULLUP);
  (digitalRead(49)==LOW)?konst=3:konst=1;
  pinMode(RPM, INPUT_PULLUP);
  pinMode(stepPin, INPUT_PULLUP);
  //pinMode(17, INPUT_PULLUP);//serial 2
  //pinMode(19, INPUT_PULLUP);//serial 2
  pinMode(dir, OUTPUT);
  pinMode(pul, OUTPUT);
  digitalWrite(pul,LOW);
  pinMode(startPin, OUTPUT);
  digitalWrite(startPin,LOW);
  pinMode(sendPin, OUTPUT);
  pinMode(onoffPin, OUTPUT);
  pinMode(POMPA,OUTPUT);
  digitalWrite(POMPA,LOW);
  attachInterrupt(digitalPinToInterrupt(RPM), rpm, FALLING);
  Serial1.begin(115200); // Wemos
  Serial2.begin(19200); // Lora
  baca = 0;
  digitalWrite(dir, LOW);

 while (digitalRead(stepPin)) {
    digitalWrite(pul, LOW);
    delayMicroseconds(500);
    digitalWrite(pul, HIGH);
    delayMicroseconds(500);
  }

  digitalWrite(sendPin, LOW);
  digitalWrite(onoffPin, LOW);
  Wire.begin();
  fuelsensor.setTimeout(500);
  if (!fuelsensor.init())
  {
    Serial.println(F("Failed to detect and initialize sensor!"));
  }
  fuelsensor.setMeasurementTimingBudget(200000);
  for (int i = 34; i < 38; i++) {
    pinMode(i, OUTPUT);
    delay(100);
    digitalWrite(i, HIGH);
  }
  Serial.println(F("DONE"));
}

void getBattery() {
  float starter, machine, system_;
  starter = ((analogRead(STARTER_BATTERY) / 1024.0) * 5) / 2.19 * 24; //pin A1
  machine = ((analogRead(MACHINE_BATTERY) / 1024.0) * 5) / 2.22 * 24; //pin A0
  system_ = ((analogRead(SYSTEM_BATTERY) / 1024.0) * 5) / 1.1 * 12; //pin A2
  starter_batt = starter / 24 * 100;
  machine_batt = machine / 24 * 100;
  system_batt = system_ / 12 * 100;
  (starter_batt>100)?starter_batt=100:starter_batt=starter_batt;
  (machine_batt>100)?machine_batt=100:machine_batt=machine_batt;
  (system_batt>100)?system_batt=100:system_batt=system_batt;
}

void shut(int j) {
  for (int i = 34; i < 38; i++) {
    if (digitalRead(i) == LOW && i != j)
      digitalWrite(i, HIGH);
  }
}

void loop() {
  // basic readout test, just print the current temp
  //fuelvalue = analogRead(fuelPin);
  rangefuel();
  //Serial.println(fuellevel);
  enginetemp = thermocouple.readCelsius();
  //Serial.println();
  //Serial.print(F("SUHU :\t")));
  //Serial.println(enginetemp);
  //battvalue = analogRead(battPin);
  //battlevel = battvalue * 10 / 84;
  getBattery();
  pressure = analogRead(pressPin);
  pressure = map(pressure, 0, 1024, -150, 1350); //1 bar = 14.5 psi
  pressure = (pressure / 100) + 1.076;
  pressvalue = pressure;
  (pressvalue > 12) ? pressvalue = 0 : pressvalue = pressvalue;

  /*if (battlevel >= 100) {
    battlevel = 100;
    }
  */
  digitalWrite(sendPin, HIGH);
  delay(200);
  currenttiming = millis();
  timing =  currenttiming - lasttiming;
  if (timing >= 3000) {
    baca /= konst;
    rpmmesin = baca * 60000 / timing;
    baca = 0;
    timing = 0;
    lasttiming = currenttiming;
    if (timemode >= 4) {
      remote = false;
      mode = 1;
      timemode = 0;
      Serial.println(F("Kirim Ke ESP32"));
      Serial1.print(F("K"));  
      Serial1.print(enginethrotle / 60);
      Serial1.print(F(";"));
      Serial1.print(rpmmesin);
      Serial1.print(F(";"));
      Serial1.print(pressvalue);
      Serial1.print(F(";1;L"));
    }
    kirimdata();
  }
  delay(100);
  //wemos
  if (siapser1) {
    for (int k = 1; k <= 4; k++) {
      data1[k] = inputweb[k].toInt();
    }
    Serial.print(F("mode="));
    Serial.println(data1[4]);
    if (data1[4] == 1) {
      ulang = false;
      target = data1[1] * 60;
    }
    /*
      if (data1[4] == 2) {
      mode = 2;
      }
      if (data1[4] == 3) {
      mode = 3;
      }
    */
    if (data1[4] == 2 && ulang == false) {
      mode = 1;
      ulang = true;
      //Serial.println("OK"));
      if (statmesin) {
        //Serial.println("mati"));
        statmesin = false;
        digitalWrite(onoffPin, LOW);
        Serial1.print(F("K"));
        Serial1.print(30);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";1;L"));
        delay (1000);
        Serial1.print(F("K"));
        Serial1.print(30);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";1;L"));
        delay (1000);
        Serial1.print(F("K"));
        Serial1.print(30);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";1;L"));
        delay (1000);
      }
      else
      {
        mode = 1;
        Serial.println(F("Hidup"));
        statmesin = true;
        stepper(1800);
        digitalWrite(onoffPin, HIGH);
        digitalWrite(startPin, HIGH);
        Serial1.print(F("K"));
        Serial1.print(30);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";1;L"));
        delay (1000);
        Serial1.print(F("K"));
        Serial1.print(30);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";1;L"));
        delay (1000);
        Serial1.print(F("K"));
        Serial1.print(30);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";"));
        Serial1.print(0);
        Serial1.print(F(";1;L"));
        delay (1000);
        digitalWrite(startPin, LOW);
      }
    }
    siapser1 = false;
  }
  //lora
  if (siapser2) {
    for (int k = 1; k <= 4; k++) {
      data2[k] = inputlora[k].toInt();
    }
    if (data2[2] == 1) {
      target = data2[1] * 60;
      mode = 1;
      //Serial.println("xxx"));
    }
    if (data2[2] == 2) {
      mode = 2;
    }
    if (data2[2] == 3) {
      mode = 3;
    }
    siapser2 = false;
  }
  //Serial.println(mode);
  if (target > 6000) {
    target = 6000;
  }
  if (target < 0) {
    target = 0;
  }
  if (target != targetlalu) {
    stepper(target);
    targetlalu = target;
  }
  enginethrotle = target;
  if (reqstart) {
    reqstart = false;
    //Serial.print(F("hidup"));
    if (!statmesin) {
      digitalWrite(onoffPin, HIGH);
      digitalWrite(startPin, HIGH);
      delay(3000);
      digitalWrite(startPin, LOW);
      statmesin = true;
    }
  }
  if (reqstart1) {
    reqstart1 = false;
    //Serial.print(F("hidup"));
    if (!statmesin) {
      digitalWrite(onoffPin, HIGH);
      digitalWrite(POMPA, HIGH);
      delay(3000);
      digitalWrite(POMPA, LOW);
      statmesin = true;
    }
  }
  if (reqstop) {
    reqstop = false;
    statmesin = false;
    //Serial.print(F("mati"));
    digitalWrite(onoffPin, LOW);
  }
  /*
    Serial.print(F("*"));
    Serial.print(round(enginetemp));
    Serial.print(F(","));
    Serial.print(battlevel);
    Serial.print(F(","));
    Serial.print(fuellevel);
    Serial.print(F(","));
    Serial.print(rpmmesin);
    Serial.print(F(","));
    Serial.print(pressvalue);
    Serial.print(F(","));
  */
  //Serial.println(enginethrotle);
  //Serial.println("#"));
  if (remote) {
    timemode += 1;
    Serial1.print(F("K"));
    Serial1.print(enginethrotle / 60);
    Serial1.print(F(";"));
    Serial1.print(rpmmesin);
    Serial1.print(F(";"));
    Serial1.print(pressvalue);
    Serial1.print(F(";4;L"));
  }
  else
  {
    digitalWrite(sendPin, LOW);
  }
  delay(500);
}

void stepper(int kali) {
  int dataposisi = throtlepos - kali;
  if (dataposisi >= 0) {
    digitalWrite(dir, LOW);
  }
  if (dataposisi < 0) {
    dataposisi = 0 - dataposisi;
    digitalWrite(dir, HIGH);
  }
  //Serial.print(F("Dataposisi = "));
  //Serial.println(dataposisi);
  if ( dataposisi != 0) {
    for (int i = 0; i <= dataposisi; i++) {
      digitalWrite(pul, LOW);
      delayMicroseconds(500);
      digitalWrite(pul, HIGH);
      delayMicroseconds(500);
    }
    //Serial.print(F("moving"));
    throtlepos = kali;
    //Serial.print(F("posisi"));
    //Serial.print(throtlepos);
  }
  digitalWrite(sendPin, LOW);
}
void kirimdata()
{
  digitalWrite(sendPin, HIGH);
  Serial.println(F("kirim ke wemos"));
  delay(100);
  //wemos
  Serial1.print(F("A"));
  Serial1.print(enginetemp);
  Serial1.print(F(";"));
  Serial1.print(fuellevel);
  Serial1.print(F(";"));
  Serial1.print(target / 60);
  Serial1.print(F(";"));
  Serial1.print(pressvalue);
  Serial1.print(F(";"));
  Serial1.print(starter_batt);
  Serial1.print(F(";"));
  Serial1.print(machine_batt);
  Serial1.print(F(";"));
  Serial1.print(system_batt);
  Serial1.print(F(";"));
  Serial1.print(rpmmesin);
  Serial1.print(F(";77;B"));
  /*
      Serial1.print(F("K"));
      Serial1.print(10);
      Serial1.print(F(";"));
      Serial1.print(11);
      Serial1.print(F(";"));
      Serial1.print(12);
      Serial1.print(F(";2;L"));
  */
  //lora
  if (reqkirim) {
    Serial.println(F("OK"));
    reqkirim = false;
    Serial2.print(F("*"));
    Serial2.print(round(enginetemp));
    Serial2.print(F(","));
    Serial2.print(system_batt);
    Serial2.print(F(","));
    Serial2.print(fuellevel);
    //Serial.print(fuellevel);
    Serial2.print(F(","));
    Serial2.print(rpmmesin);
    Serial2.print(F(","));
    Serial2.print(pressvalue);
    Serial2.print(F(","));
    Serial2.print(enginethrotle / 60);
    Serial2.print(F(","));
    Serial2.print(starter_batt);
    Serial2.print(F(","));
    Serial2.print(machine_batt);
    Serial2.println(F("#"));
  }
}

void rpm()
{
  baca += 1;
  //Serial2.println(baca);
}

void serialEvent1() {
  while (Serial1.available()) {
    char inChar1 = (char)Serial1.read();
    if (inChar1 == 'L') {
      siapser1 = true;
      mulaiser1 = false;
    }
    if (mulaiser1) {
      if (inChar1 == ';') {
        hitung1 += 1;
      }
      else
      {
        inputweb[hitung1] += inChar1;
      }
    }
    if (inChar1 == 'K' && siapser1 == false) {
      mulaiser1 = true;
      hitung1 = 1;
      for (int k = 1; k <= 4; k++) {
        inputweb[k] = "";
      }
    }
  }
}

void serialEvent2() {
  while (Serial2.available()) {
    char inChar = (char)Serial2.read();
    if (inChar == '?') {
      reqkirim = true;
    }
    if (inChar == '-') {
      reqstart1 = true;
    }
    if (inChar == '!') {
      reqstop = true;
    }
    if (inChar == '=') {
      reqstart = true;
    }
    if (inChar == '#') {
      siapser2 = true;
      mulaiser2 = false;
    }
    if (mulaiser2) {
      if (inChar == ',') {
        hitung2 += 1;
      }
      else
      {
        inputlora[hitung2] = inputlora[hitung2] + inChar;
      }
    }
    if (inChar == '*') {
      mulaiser2 = true;
      hitung2 = 1;
      for (int j = 1; j <= 4; j++) {
        inputlora[j] = "";
      }
    }
  }
  remote = true;
  timemode = 0;
}

void rangefuel() {
  //Serial.println(fuelvalue.RangeMilliMeter);
  (fuelsensor.timeoutOccurred()) ? Serial.print(F("timeout")) : fuellevel = fuelsensor.readRangeSingleMillimeters();
  if (fuellevel < 67) {
    digitalWrite(LED_BIRU, LOW);
    shut(LED_BIRU);
  }
  else if (fuellevel > 99) {
    digitalWrite(LED_MERAH, LOW);
    shut(LED_MERAH);
  }
  else {
    switch (fuellevel) {
      case 67 ... 83:
        digitalWrite(LED_HIJAU, LOW);
        shut(LED_HIJAU);
        break;
      case 84 ... 99:
        digitalWrite(LED_KUNING, LOW);
        shut(LED_KUNING);
        break;
    }
  }
  fuellevel = map(fuellevel, 40, 115, 100, 0);
  (fuellevel > 100) ? fuellevel = 100 : (fuellevel < 0) ? fuellevel = 0 : fuellevel = fuellevel;
}

/*
  void rangefuel()
  {
  switch (fuelvalue) {
    case 295 ... 312:
      fuellevel = 100;
      break;
    case 325 ... 350:
      fuellevel = 97;
      break;
    case 354 ... 368:
      fuellevel = 94;
      break;
    case 380 ... 398:
      fuellevel = 91;
      break;
    case 403 ... 423:
      fuellevel = 88;
      break;
    case 425 ... 445:
      fuellevel = 85;
      break;
    case 446 ... 456:
      fuellevel = 82;
      break;
    case 465 ... 475:
      fuellevel = 80;
      break;
    case 483 ... 493:
      fuellevel = 77;
      break;
    case 500 ... 510:
      fuellevel = 74;
      break;
    case 516 ... 526:
      fuellevel = 71;
      break;
    case 531 ... 541:
      fuellevel = 68;
      break;
    case 545 ... 555:
      fuellevel = 65;
      break;
    case 559 ... 569:
      fuellevel = 62;
      break;
    case 571 ... 581:
      fuellevel = 60;
      break;
    case 583 ... 593:
      fuellevel = 57;
      break;
    case 594 ... 604:
      fuellevel = 54;
      break;
    case 605 ... 615:
      fuellevel = 51;
      break;
    case 618 ... 628:
      fuellevel = 48;
      break;
    case 630 ... 640:
      fuellevel = 45;
      break;
    case 642 ... 652:
      fuellevel = 42;
      break;
    case 653 ... 663:
      fuellevel = 40;
      break;
    case 666 ... 672:
      fuellevel = 37;
      break;
    case 675 ... 681:
      fuellevel = 34;
      break;
    case 685 ... 691:
      fuellevel = 31;
      break;
    case 694 ... 700:
      fuellevel = 28;
      break;
    case 702 ... 708:
      fuellevel = 25;
      break;
    case 710 ... 716:
      fuellevel = 22;
      break;
    case 718 ... 724:
      fuellevel = 20;
      break;
    case 725 ... 731:
      fuellevel = 17;
      break;
    case 732 ... 736:
      fuellevel = 14;
      break;
    case 738 ... 744:
      fuellevel = 11;
      break;
    case 745 ... 749:
      fuellevel = 8;
      break;
    case 750 ... 756:
      fuellevel = 5;
      break;
    case 757 ... 761:
      fuellevel = 2;
      break;
    case 762 ... 766:
      fuellevel = 0;
      break;
  }
  }*/
