#include <Wire.h>
#include "VL53L0X.h"
#include "max6675.h"

#define RPM 2
#define THERMOCOUPLE_CS 3
#define BIG_RELAY 4
#define THERMOCOUPLE_MISO 5
#define THERMOCOUPLE_SCK 7
#define LORA_RX 16
#define LORA_TX 17
#define ESP_RX 18
#define ESP_TX 19
#define LED_KUNING 34
#define LED_MERAH 35
#define LED_BIRU 36
#define LED_HIJAU 37
#define MICROSTEPPER_ PULSE 40
#define MICROSTEPPER_ DIR 42
#define MICROSTEPPER_ ENABLE 44
#define MACHINE_BATTERY A0
#define STARTER_BATTERY_1 A1
#define STARTER_BATTERY_2 A2
#define PRESSURE_SENSOR A3
#define RELAY1 A7
#define RELAY2 A5
#define LIMIT1 A11
#define LIMIT2 A9

MAX6675 thermocouple(THERMOCOUPLE_SCK, THERMOCOUPLE_CS, THERMOCOUPLE_MISO);
VL53L0X fuelsensor = VL53L0X();
int fuelLevel;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(19200);

  Serial.println("TEST");
  Wire.begin();
  fuelsensor.setTimeout(500);
  if (!fuelsensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
    
  //pinMode(PRESSURE_SENSOR,INPUT);
  for (int i = 34; i < 38; i++) {
    pinMode(i, OUTPUT);
    delay(100);
    digitalWrite(i, HIGH);
  }
}

void shut(int j) {
  for (int i = 34; i < 38; i++) {
    if (digitalRead(i) == LOW && i != j)
      digitalWrite(i, HIGH);
  }
}

void loop() {
  int percentage;
  Serial.println("A");
  //float vol1, vol2, vol3;
  /*
    float sensorVal = analogRead(PRESSURE_SENSOR);
    Serial.println(sensorVal);
    sensorVal = map(sensorVal, 0, 1024, -150, 1350);
    sensorVal = (sensorVal / 100) + 1.076;
    //Serial.println(sensorVal);
    delay(100);
  */
  /*
    Serial.print("C = ");
    Serial.println(thermocouple.readCelsius());
    Serial.print("F = ");
    Serial.println(thermocouple.readFahrenheit());
    delay(1000);
    vol1 = ((analogRead(A1) / 1024.0) * 5) / 2.19 * 24; //pin A1
    vol2 = ((analogRead(A0) / 1024.0) * 5) / 2.22 * 24; //pin A0
    vol3 = ((analogRead(A2) / 1024.0) * 5) / 2.2 * 24; //pin A2
  */
  (fuelsensor.timeoutOccurred()) ? Serial.print("timeout") : fuelLevel = fuelsensor.readRangeSingleMillimeters();
  Serial.print(fuelLevel);
  Serial.print("\t");
  if (fuelLevel < 67) {
    digitalWrite(LED_BIRU, LOW);
    shut(LED_BIRU);
  }
  else if (fuelLevel > 99) {
    digitalWrite(LED_MERAH, LOW);
    shut(LED_MERAH);
  }
  else {
    switch (fuelLevel) {
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
  percentage = map(fuelLevel, 40, 115, 100, 0);
  Serial.println(percentage);
}
