#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SENSOR_PIN 3 //3 dan 13
#define BUZZER 4
#define UV_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
uint64_t  previousMillis = 0;
int sensor;
int lastState = LOW;
int currentState;
int options = 0;

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Hello, world!");
  display.display();
  delay(1000);

  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT); //buzzer
  delay(100);
  digitalWrite(BUZZER, HIGH);
  pinMode(UV_PIN, OUTPUT); //UV
  delay(100);
}

void UV(){
  digitalWrite(UV_PIN, HIGH);
  delay(10000);
  digitalWrite(UV_PIN, LOW);
}

void loop() {
  currentState = digitalRead(SENSOR_PIN);
  switch (currentState) {
    case 1:
      switch (lastState) {
        case 0:
          Serial.println("Sensor Touched");
          lastState = currentState;
          break;
      }
      break;
    case 0:
      switch (lastState) {
        case 1:
          Serial.println("Sensor Released");
          lastState = currentState;
          options += 1;
          break;
      }
      break;
  }
  switch (options) {
    case 0:
      display.clearDisplay();
      display.println("Option 1");
      display.display();
      break;
    case 1:
      display.clearDisplay();
      display.println("Option 2");
      display.display();
      break;
    case 2:
      display.clearDisplay();
      display.println("Option 3");
      display.display();
      break;
    case 3:
      options = 0;
      break;
  }
}
