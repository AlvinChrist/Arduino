#include <ThingSpeak.h>
#include "Credentials.h" //file containing sensitive information
#include <ESP8266WiFi.h>
#include <SPI.h>

byte sensorPin       = 0;
float calibrationFactor = 4.5;
volatile byte pulseCount;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
String lasttotalMilliLitres;
unsigned long totalLitres;

int lastTotalMilliLitres;

char ssid[] = MYSSID;
char pass[] = PASS;
unsigned long previousMillis;
unsigned long currentMillis;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  previousMillis    = 0;
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, FALLING);
}

void readSensor() {
  detachInterrupt(sensorPin);
  flowRate = ((60000.0 / (millis() - previousMillis)) * pulseCount) / calibrationFactor;
  flowMilliLitres = (flowRate / 60) * 1000;
  totalMilliLitres += flowMilliLitres;
  totalLitres = totalMilliLitres/1000;
  pulseCount = 0;
  attachInterrupt(sensorPin, pulseCounter, FALLING);
}

void readLastInfo() {
  delay(2000);
  lasttotalMilliLitres = ThingSpeak.readStringField(CHANNEL, 1, READ_API_KEY); // reading counter from field 2
  
  Serial.println("Last Volume : " + lasttotalMilliLitres);
}

void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void SendData() {
  ThingSpeak.setField(1, int(totalMilliLitres));
  int headerCode = ThingSpeak.writeFields(CHANNEL, WRITE_API_KEY);
  if (headerCode == 200) {
    Serial.println("Data sent successfully");
  }
  else {
    Serial.println("Problem detected. HTTP error code " + String(headerCode));
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to ");
    Serial.print(MYSSID);
    while (WiFi.status() != WL_CONNECTED) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= 20000) {
        previousMillis = currentMillis;
        readSensor();
      }
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
    readLastInfo();
  }
  currentMillis = millis();
  if (currentMillis - previousMillis >= 20000) { // send data every 20s
    readSensor();
    previousMillis = currentMillis;

    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL");
    Serial.print("\t");       // Print tab space
    Serial.print(totalLitres);
    Serial.println("L");

    SendData();
  }
}
