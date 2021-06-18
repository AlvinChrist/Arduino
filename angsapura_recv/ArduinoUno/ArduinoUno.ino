#include <SoftwareSerial.h>
#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);
SoftwareSerial chat(11, 12); // RX, TX

#define pir 9
#define led 5
#define sirene 2
#define lampu 3
int text;
String inputString;
bool stringStart;
bool stringEnd;
int pirState = LOW;
int val;
int SensorStartTime;
int SensorEndTime;
int LightStartTime;
int LightEndTime;
bool SensorOn = false;
bool LightOn = false;
bool SensorTimeConfig;
bool LightTimeConfig;
int interval = 10000;
unsigned long previousMillis = 0;
void setup()
{
  // open hardware serial, TX = 1, RX = 0
  Serial.begin(9600);
  Serial.println("Starting Chat Program...");

  // set the data rate for the SoftwareSerial port
  delay(1000); // delay 1s to stabilize serial ports
  chat.begin(300);
  pinMode(pir, INPUT);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(led, OUTPUT);
  pinMode(lampu, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (!Rtc.IsDateTimeValid())
  {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }
  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  delay(500);
  chat.write("getSensorAndLight");
}

void setSensorTime() {
  String Time;
  String StartHour;
  String StartMinute;
  String EndHour;
  String EndMinute;
  Time = chat.readString();
  for (int i = 0; i <= 1; i++) {
    StartHour += Time[i];
  }
  for (int i = 3; i <= 4; i++) {
    StartMinute += Time[i];
  }
  for (int i = 5; i <= 6; i++) {
    EndHour += Time[i];
  }
  for (int i = 8; i <= 9; i++) {
    EndMinute += Time[i];
  }
  SensorStartTime = StartHour.toInt() * 60 + StartMinute.toInt();
  SensorEndTime = EndHour.toInt() * 60 + EndMinute.toInt();
  Serial.println("Sensor :");
  Serial.println(StartHour + ":" + StartMinute + "-" + EndHour + ":" + EndMinute);
  if (SensorStartTime != NULL) {
    SensorTimeConfig = true;
  }
  return;
}

void setLightTime() {
  String Time;
  String StartHour;
  String StartMinute;
  String EndHour;
  String EndMinute;
  Time = chat.readString();
  for (int i = 0; i <= 1; i++) {
    StartHour += Time[i];
  }
  for (int i = 3; i <= 4; i++) {
    StartMinute += Time[i];
  }
  for (int i = 5; i <= 6; i++) {
    EndHour += Time[i];
  }
  for (int i = 8; i <= 9; i++) {
    EndMinute += Time[i];
  }
  LightStartTime = StartHour.toInt() * 60 + StartMinute.toInt();
  LightEndTime = EndHour.toInt() * 60 + EndMinute.toInt();
  Serial.println("Light :");
  Serial.println(StartHour + ":" + StartMinute + "-" + EndHour + ":" + EndMinute);
  if (LightStartTime != NULL) {
    LightTimeConfig = true;
  }
  return;
}

void chatEvent() {
  while (chat.available() > 0) {
    char inChar = (char)chat.read();
    if (inChar == '#') {
      stringEnd = true;
      stringStart = false;
      return;
    }
    if (stringStart) {
      inputString += inChar;
    }
    if (inChar == '*') {
      inputString = "";
      stringStart = true;
    }
    if (inChar == '\n') {
      inputString = "";
      stringStart = false;
    }
  }
}


void checkSensor() {
  is_sensor_on();
  unsigned long currentMillis = millis();
  if (LightOn) {
    digitalWrite(lampu, HIGH);
  }
  if (!LightOn) {
    digitalWrite(lampu, LOW);
  }
  if (SensorOn) {
    digitalWrite(led, HIGH);
    val = digitalRead(pir);
    if (val == HIGH) {
      if (pirState == LOW) {
        pirState = HIGH;
        return;
      }
      return;
    }
    if (val == LOW) {
      if (pirState == HIGH) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          pirState = LOW;
        }
        return;
      }
      return;
    }
  }
  if (!SensorOn) {
    digitalWrite(led, LOW);
    return;
  }
}

void start() {
  checkSensor();
  if (chat.available() > 0) {
    chatEvent();
    if (stringEnd) {
      Serial.println(inputString);
      if (inputString == "checkHuman") {
        chat.write(pirState);
        delay(7);
      }
      if (inputString == "setSensorTime") {
        setSensorTime();
      }
      if (inputString == "setLightTime") {
        setLightTime();
      }
      if ( inputString == "getLightStatus") {
        if (LightOn) {
          chat.write("on");
          delay(7);
        }
        else {
          chat.write("off");
          delay(7);
        }
      }
      if (inputString == "getSensorStatus") {
        if (SensorOn) {
          chat.write("on");
          delay(7);
        }
        else {
          chat.write("off");
          delay(7);
        }
      }
      if (inputString == "getDateTime") {
        printDateTime();
      }
      inputString = "";
      stringEnd = false;
    }
  }
  if (Serial.available())
  {
    while (Serial.available())
    {
      text = Serial.read();
      delay(7);
      chat.write(text);
      Serial.write(text);
    }
    chat.println();
    Serial.println();
  }
}

void printDateTime() {
  char dateTime[16];
  RtcDateTime now = Rtc.GetDateTime();
  sprintf(dateTime, "%02u/%02u/%02u %02u:%02u", now.Year() , now.Month() , now.Day() , now.Hour() , now.Minute());
  Serial.println(dateTime);
  chat.print(dateTime);
  delay(7);
  return;
}

void PIRSensor(int nowTime) {
  if (SensorStartTime > SensorEndTime) {
    if ((nowTime >= SensorStartTime) || (nowTime <= SensorEndTime)) {
      SensorOn = true;
      return;
    }
    else {
      SensorOn = false;
      return;
    }
  }
  else if (SensorStartTime < SensorEndTime) {
    if ((nowTime >= SensorStartTime) && (nowTime <= SensorEndTime)) {
      SensorOn = true;
      return;
    }
    else {
      SensorOn = false;
      return;
    }
  }
}


void Light(int nowTime) {
  if (LightStartTime > LightEndTime) {
    if ((nowTime >= LightStartTime) || (nowTime <= LightEndTime)) {
      LightOn = true;
      return;
    }
    else {
      LightOn = false;
      return;
    }
  }
  else if (LightStartTime < LightEndTime) {
    if ((nowTime >= LightStartTime) && (nowTime <= LightEndTime)) {
      LightOn = true;
      return;
    }
    else {
      LightOn = false;
      return;
    }
  }
}

void is_sensor_on() {
  int nowTime;
  RtcDateTime now = Rtc.GetDateTime();
  nowTime = now.Hour() * 60 + now.Minute();
  if (SensorTimeConfig) {
    PIRSensor(nowTime);
  }
  if (LightTimeConfig) {
    Light(nowTime);
  }

}
void loop() {
  start();
}
