  #include <SoftwareSerial.h>
SoftwareSerial chat(5, 4); // RX, TX
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SD.h>
#include <SPI.h>
const char* ssid = "ArifinOffice";
const char* password = "sekrapdeksel";
String Lusername;
String rootjs1;
String rootjs2;
String onrootcss;
String offrootcss;
String sens1;
String sens2;
String lampu1;
String lampu2;
String loginjs;
String logincss;
String webPage = "";
String loginPage = "";
String cpwd;
String cpwdcss;
String username;
String pass;
String mpass;
String cfgsensor;
String cfgsensorcss;
String SensorStartTime;
String SensorEndTime;
String LightStartTime;
String LightEndTime;
int text;
String inputString;
bool startsensor = false;
bool stringStart;
bool stringEnd;
File myFile;
int lvalue;
int val = false;
#define inLed 2 // GPIO2 of ESP8266

ESP8266WebServer server(80);
void handlePage() {
  myFile = SD.open("login.txt");
  if (myFile) {
    Serial.print("Loading login page..");
    while (myFile.available()) {
      loginPage += myFile.readStringUntil('\n');
    }
    myFile.close();
    Serial.println(" Done");
  }
  else {
    Serial.println("Error on loading login page");
    myFile.close();
  }
  myFile = SD.open("loginjs.txt");
  if (myFile) {
    Serial.print("Loading Login javascript..");
    while (myFile.available()) {
      loginjs += myFile.readStringUntil('\n');
    }
    myFile.close();
    Serial.println(" Done");
  }
  else {
    Serial.println("Error on loading loginjs.txt");
    myFile.close();
  }
  myFile = SD.open("logincss.txt");
  if (myFile) {
    Serial.print("Loading login css..");
    while (myFile.available()) {
      logincss += myFile.readStringUntil('\n');
    }
    myFile.close();
    Serial.println(" Done");
  }
  else {
    Serial.println("Error on loading logincss.txt");
    myFile.close();
  }
  myFile = SD.open("if.txt");
  if (myFile) {
    Serial.print("Loading webpage..");
    while (myFile.available()) {
      webPage += myFile.readStringUntil('\n');
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading if.txt");
    myFile.close();
  }
  myFile = SD.open("rootjs1.txt");
  if (myFile) {
    Serial.print("Loading Javascript file..");
    while (myFile.available()) {
      rootjs1 += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("Error on loading js.txt");
    myFile.close();
  }
  myFile = SD.open("rootjs2.txt");
  if (myFile) {
    while (myFile.available()) {
      rootjs2 += myFile.readStringUntil('\n');
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading js.txt");
    myFile.close();
  }
  myFile = SD.open("onrcss.txt");
  if (myFile) {
    Serial.print("Loading CSS file..");
    while (myFile.available()) {
      onrootcss += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("Error on loading css.txt");
    myFile.close();
  }
  myFile = SD.open("offrcss.txt");
  if (myFile) {
    while (myFile.available()) {
      offrootcss += myFile.readStringUntil('\n');
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading rootcss.txt");
    myFile.close();
  }
  myFile = SD.open("change.txt");
  if (myFile) {
    Serial.print("Loading Changepwd file..");
    while (myFile.available()) {
      cpwd += myFile.readStringUntil('\n');
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading change.txt");
    myFile.close();
  }
  myFile = SD.open("changecs.txt");
  if (myFile) {
    Serial.print("Loading Changepwd CSS file.. ");
    while (myFile.available()) {
      cpwdcss += myFile.readStringUntil('\n');
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading changecs.txt");
    myFile.close();
  }
  myFile = SD.open("cfgsens.txt");
  if (myFile) {
    Serial.print("Loading cfgsens file.. ");
    while (myFile.available()) {
      cfgsensor += myFile.readStringUntil('\n');
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading cfgsens");
    myFile.close();
  }
  myFile = SD.open("cfgcss.txt");
  if (myFile) {
    Serial.print("Loading cfgcss file.. ");
    while (myFile.available()) {
      cfgsensorcss += myFile.readStringUntil('\n');
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading cfgcss");
    myFile.close();
  }
  myFile = SD.open("username.txt");
  if (myFile) {
    while (myFile.available()) {
      username += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("Error on loading Credentials");
    myFile.close();
  }
  myFile = SD.open("password.txt");
  if (myFile) {
    while (myFile.available()) {
      pass += myFile.readStringUntil('\n');
      pass.trim();
    }
    myFile.close();
  }
  else {
    Serial.println("Error on loading Credentials (2)");
    myFile.close();
  }
  myFile = SD.open("sens1.txt");
  if (myFile) {
    while (myFile.available()) {
      sens1 += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("Sens1 file cannot be loaded");
    myFile.close();
  }
  myFile = SD.open("sens2.txt");
  if (myFile) {
    while (myFile.available()) {
      sens2 += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("Sens2 file cannot be loaded");
    myFile.close();
  }
  myFile = SD.open("lampu1.txt");
  if (myFile) {
    while (myFile.available()) {
      lampu1 += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("lampu1 file cannot be loaded");
    myFile.close();
  }
  myFile = SD.open("lampu1.txt");
  if (myFile) {
    while (myFile.available()) {
      lampu1 += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("lampu1 file cannot be loaded");
    myFile.close();
  }
  myFile = SD.open("lampu2.txt");
  if (myFile) {
    while (myFile.available()) {
      lampu2 += myFile.readStringUntil('\n');
    }
    myFile.close();
  }
  else {
    Serial.println("lampu2 file cannot be loaded");
    myFile.close();
  }

}

void setSensorTime() {
  chat.write("*setSensorTime#");
  delay(7);
  chat.print(SensorStartTime);
  delay(7);
  chat.print(SensorEndTime);
  delay(7);
  return;
}

void setLightTime() {
  chat.write("*setLightTime#");
  delay(7);
  chat.print(LightStartTime);
  delay(7);
  chat.print(LightEndTime);
  delay(7);
  return;
}
void start()
{
  server.handleClient();
  if (chat.available() > 0) {
    inputString = chat.readString();
    if (inputString == "getSensorAndLight") {
      setSensorTime();
      delay(1500);
      setLightTime();
    }
    else {
      Serial.println(inputString);
    }
  }
  if (Serial.available())
  {
    while (Serial.available())
    {
      text = Serial.read();
      chat.write(text);
      Serial.write(text);
      delay(7);
    }
    chat.println();
    Serial.println();
    return;
  }
}

void handleCSS() {
  server.send(200, "text/css", logincss);
  return;
}
void handleJS() {
  server.send(200, "text/javascript", loginjs);
  return;
}
void handleRootCSS() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  if (lvalue == 0) {
    server.send(200, "text/css", onrootcss);
    return;
  }
  if (lvalue == 1) {
    server.send(200, "text/css", offrootcss);
    return;
  }
}
void handleRootJS() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  chat.write("*checkHuman#");
  delay(7);
  val = chat.read();
  if (val == HIGH) {
    server.send(200, "text/javascript", rootjs1);
    return;
  }
  if (val == LOW) {
    server.send(200, "text/javascript", rootjs2);
    return;
  }
}
void handleChangepwdCSS() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  server.send(200, "text/css", cpwdcss);
  return;
}
//Check if header is present and correct
bool is_authentified() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    if (cookie.indexOf("ESPSESSIONID=d56b699830e77ba53855679cb1d252da") != -1) {
      return true;
    }
  }
  return false;
}

//login page, also called for disconnect
void handleLogin() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
  }
  if (server.hasArg("LOGOUT")) {
    Serial.println("Disconnection");
    if (server.hasArg("HIDDENLOG")) {
      myFile = SD.open("LOG.txt", FILE_WRITE);
      if (myFile) {
        myFile.println(Lusername + " logout at " + server.arg("HIDDENLOG") + '\n');
        myFile.close();
      }
    }
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == username &&  server.arg("PASSWORD") == pass ) {
      Lusername = server.arg("USERNAME");
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=d56b699830e77ba53855679cb1d252da");
      server.send(301);
      Serial.println("Log in Successful");
      if (server.hasArg("SUBMIT")) {
        myFile = SD.open("LOG.txt", FILE_WRITE);
        if (myFile) {
          myFile.println(server.arg("USERNAME") + " login at  " + server.arg("SUBMIT"));
          myFile.close();
        }
        return;
      }
      else {
        Serial.println("Cannot log login attempt!");
        return;
      }
      return;
    }
    server.sendHeader("Location", "/login");
    server.send(200, "text/html", "<script>alert(\"username/password incorrect\");window.location.replace(\"/login\");</script>");
  }
  server.send(200, "text/html", loginPage);
}
void handleCfgsensor() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  if (server.method() == HTTP_POST) {
    if ((server.hasArg("SENSORSTARTTIME") && server.hasArg("SENSORENDTIME")) || (server.hasArg("LIGHTSTARTTIME") && server.hasArg("LIGHTENDTIME"))) {
      if (server.hasArg("SENSORSTARTTIME") && server.hasArg("SENSORENDTIME")) {
        if (server.arg("SENSORSTARTTIME").length() > 0) {
          SensorStartTime = server.arg("SENSORSTARTTIME");
          SensorEndTime = server.arg("SENSORENDTIME");
          if (SD.exists("var2.txt")) {
            SD.remove("var2.txt");
          }
          myFile = SD.open("var2.txt", FILE_WRITE);
          if (myFile) {
            myFile.println(SensorStartTime + SensorEndTime);
            myFile.close();
          }
          setSensorTime();
          delay(7);
          Serial.println("Sensor :\n" + SensorStartTime + " - " + SensorEndTime);
        }
      }
      if (server.hasArg("LIGHTSTARTTIME") && server.hasArg("LIGHTENDTIME")) {
        if (server.arg("LIGHTSTARTTIME").length() > 0) {
          LightStartTime = server.arg("LIGHTSTARTTIME");
          LightEndTime = server.arg("LIGHTENDTIME");
          if (SD.exists("var3.txt")) {
            SD.remove("var3.txt");
          }
          myFile = SD.open("var3.txt", FILE_WRITE);
          if (myFile) {
            myFile.println(LightStartTime + LightEndTime);
            myFile.close();
          }
          delay(1500);
          setLightTime();
          delay(7);
          Serial.println("Light :\n" + LightStartTime + " - " + LightEndTime);
        }
      }
      server.sendHeader("Location", "/configuresensor");
      server.send(200, "text/html", "<script>alert(\"Configurations Saved !\");window.location.replace(\"/\");</script>");
      return;
    }
    else {
      server.sendHeader("Location", "/configuresensor");
      server.send(200, "text/html", "<script>alert(\"Please fill at least one form ! \");window.location.replace(\"/configuresensor\");</script>");
      return;
    }
  }
  server.send(200, "text/html", cfgsensor);
  return;
}
void handleCfgCSS() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  server.send(200, "text/css", cfgsensorcss);
}
void handleChangepwd() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  if (server.hasArg("OLDPWD")) {
    if (server.arg("OLDPWD") != pass) {
      server.sendHeader("Location", "/changepwd");
      server.send(200, "text/html", "<script>alert(\"Current Password Incorrect\");window.location.replace(\"/changepwd\");</script>");
      return;
    }
    if (server.hasArg("NEWPWD") && server.hasArg("REPEATPWD")) {
      if (server.arg("NEWPWD") == server.arg("REPEATPWD")) {
        SD.remove("password.txt");
        myFile = SD.open("password.txt", FILE_WRITE);
        if (myFile) {
          Serial.println("Password Changed");
          myFile.println(server.arg("NEWPWD"));
        }
        myFile.close();
        myFile = SD.open("password.txt");
        if (myFile) {
          while (myFile.available()) {
            pass = myFile.readStringUntil('\n');
            pass.trim();
            Serial.print(": " + pass);
          }
          myFile.close();
        }
        server.send(200, "text/html", "<script>alert(\"Password Changed\");window.location.replace(\"/\");</script>");
        return;
      }
      else {
        server.sendHeader("Location", "/changepwd");
        server.send(200, "text/html", "<script>alert(\"New password does not match\");window.location.replace(\"/changepwd\");</script>");
        return;
      }
    }
  }
  server.send(200, "text/html", cpwd);
}
void handleLog() {
  String logfile;
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  else {
    myFile = SD.open("LOG.txt", FILE_READ);
    if (myFile) {
      while (myFile.available()) {
        logfile += myFile.readStringUntil('\n');
      }
      myFile.close();
    }
    server.send(200, "text/html", "<!DOCTYPEhtml><html><head></head><body><pre>" + logfile + "</pre><br /><br /><input type=\"button\" value=\"back\" onclick=\"window.location.replace(\'/\');\" /></body></html>");
    return;
  }
}
void handleErase() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  else {
    SD.remove("LOG.txt");
    myFile = SD.open("LOG.txt", FILE_WRITE);
    myFile.close();
    server.send(200, "text/html", "<script>alert(\"Log erased! \");window.location.replace(\"/\");</script>");
    return;
  }
}
void handleSensor() {
  String stats;
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  delay(7);
  chat.write("*getSensorStatus#");
  delay(7);
  stats = chat.readString();
  if (stats == "on") {
    server.send(200, "text/javascript", sens1);
  }
  else {
    server.send(200, "text/javascript", sens2);
  }
}

void handleLampu() {
  String stats;
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  delay(7);
  chat.write("*getLightStatus#");
  delay(7);
  stats = chat.readString();
  if (stats == "on") {
    server.send(200, "text/javascript", lampu1);
  }
  else {
    server.send(200, "text/javascript", lampu2);
  }
}
//root page can be accessed only if authentification is ok
void handleRoot() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  else {
    server.send(200, "text/html", webPage);
    return;
  }
}
//no need authentification
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
void handleSensorTime() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  else {
    server.send(200, "text/html", "<script>alert(\"Sensor :\\n" + SensorStartTime + "-" + SensorEndTime + "\\n\\nLampu :\\n" + LightStartTime + "-" + LightEndTime + "\");window.location.replace(\"/\"); </script>");
    return;
  }
}
void setup(void)
{
  String Time;
  String svalue;
  // open hardware serial, TX = 1, RX = 0
  Serial.begin(9600);
  Serial.println("Starting Chat Program...");
  // set the data rate for the SoftwareSerial port
  delay(1000); // delay 1s to stabilize serial ports
  chat.begin(300);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(7);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Initializating SDcard...");
  if (!SD.begin(0)) {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println(" Done");
  myFile = SD.open("var.txt");
  if (myFile) {
    Serial.print("Loading last LED Config.. ");
    while (myFile.available()) {
      svalue = myFile.readStringUntil('\n');
      svalue.trim();
      lvalue = svalue.toInt();
    }
    Serial.println(" Done");
    myFile.close();
  }
  else {
    Serial.println("Error on loading last LED Config");
    myFile.close();
  }
  if (SD.exists("var2.txt")) {
    myFile = SD.open("var2.txt");
    if (myFile) {
      Serial.print("Loading Sensor last config..");
      while (myFile.available()) {
        Time = myFile.readStringUntil('\n');
        Time.trim();
      }
      Serial.println(" Done");
      for (int i = 0; i <= 4; i++) {
        SensorStartTime += Time[i];
      }
      for (int i = 5; i <= 9; i++) {
        SensorEndTime += Time[i];
      }
      myFile.close();
    }
    else {
      Serial.println("Error on loading last Sensor Config");
      myFile.close();
    }
  }
  if (SD.exists("var3.txt")) {
    myFile = SD.open("var3.txt");
    if (myFile) {
      Serial.print("Loading Light last config..");
      while (myFile.available()) {
        Time = myFile.readStringUntil('\n');
        Time.trim();
      }
      Serial.println(" Done");
      for (int i = 0; i <= 4; i++) {
        LightStartTime += Time[i];
      }
      for (int i = 5; i <= 9; i++) {
        LightEndTime += Time[i];
      }
      myFile.close();
    }
    else {
      Serial.println("Error on loading last Light Config");
      myFile.close();
    }
  }
  handlePage();
  pinMode(inLed, OUTPUT);;
  digitalWrite(inLed, lvalue);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  server.on("/", handleRoot);
  server.on("/rootjs.txt", handleRootJS);
  server.on("/rootcss.txt", handleRootCSS);
  server.on("/logincss.txt", handleCSS);
  server.on("/loginjs.txt", handleJS);
  server.on("/login", handleLogin);
  server.onNotFound(handleNotFound);
  server.on("/changepwd", handleChangepwd);
  server.on("/lampu.txt", handleLampu);
  server.on("/changecs.txt", handleChangepwdCSS);
  server.on("/configuresensor", handleCfgsensor);
  server.on("/cfgcss.txt", handleCfgCSS);
  server.on("/LOG", handleLog);
  server.on("/eraselog", handleErase);
  server.on("/sens.txt", handleSensor);
  server.on("/sensortime", handleSensorTime);
  server.on("/LED1=OFF", []() {
    if (!is_authentified()) {
      server.sendHeader("Location", "/login");
      server.sendHeader("Cache-Control", "no-cache");
      server.send(301);
      return;
    }
    lvalue = 1;
    if (SD.exists("var.txt")) {
      SD.remove("var.txt");
    }
    myFile = SD.open("var.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(lvalue);
      myFile.close();
    }
    server.sendHeader("Location", "/");
    server.send(200, "text/html", webPage);
    digitalWrite(inLed, lvalue);

  });
  server.on("/LED1=ON", []() {
    if (!is_authentified()) {
      server.sendHeader("Location", "/login");
      server.sendHeader("Cache-Control", "no-cache");
      server.send(301);
      return;
    }
    lvalue = 0;
    if (SD.exists("var.txt")) {
      SD.remove("var.txt");
    }
    myFile = SD.open("var.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(lvalue);
      myFile.close();
    }
    server.sendHeader("Location", "/");
    server.send(200, "text/html", webPage);
    digitalWrite(inLed, lvalue);
  });
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
  Serial.println("HTTP server started");
  setSensorTime();
  delay(1500);
  setLightTime();
}
void loop() {
  start();
}
