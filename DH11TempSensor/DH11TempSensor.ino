#include <DHTesp.h>
#include <ThingSpeak.h>
#include "D:/Arduino/Thingspeak/Credentials.h"
#include <ESP8266WiFi.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;
char ssid[] = MYSSID1;
char pass[] = PASS1;
unsigned long startMillis;
unsigned long currentMillis;
float humidity;
float temperature;
float heatIndex;
WiFiClient client;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  startMillis = millis();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\tHeatIndex (C)");
  String thisBoard = ARDUINO_BOARD;
  Serial.println(thisBoard);
  dht.setup(0, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
}

void readTemp() {
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  Serial.print(humidity, 1);
  Serial.print("\t");
  Serial.print(temperature, 1);
  Serial.print("\t");
  Serial.println(heatIndex, 1);
}
void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to ");
    Serial.print(MYSSID1);
    while (WiFi.status() != WL_CONNECTED) {
      currentMillis = millis();
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }
  currentMillis = millis();
  if ((unsigned long)(currentMillis - startMillis) >= 600000) {
    startMillis = currentMillis;
    readTemp();
    ThingSpeak.setField(1, humidity);
    ThingSpeak.setField(2, temperature);
    ThingSpeak.setField(3, heatIndex);
    int headerCode = ThingSpeak.writeFields(CHANNEL, WRITE_API_KEY);
    if (headerCode = 200) {
      Serial.println("Data sent!");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(headerCode));
    }
  }
}
