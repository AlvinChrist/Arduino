uint8_t arr[4][6]={{0xC2,0x00,0x15,0x3D,0x01,0x44},{0xC2,0x00,0x16,0x3D,0x02,0x44},{0xC2,0x00,0x17,0x3D,0x03,0x44},{0xC2,0x00,0x18,0x3D,0x04,0x44}};

void setup() {
  Serial.begin(115200);
}

void changeSettings(uint8_t options){
  Serial.flush();
  Serial.begin(9600); //start new baudrate for setting
  for(byte x=0;x<6;x++){
    Serial.write(arr[options][x]);
  }
  delay(2); 
  Serial.flush();
  Serial.begin(115200);
  while(Serial.available()){Serial.read();}
}

void loop() {
  for(int x=0;x<4;x++){
    changeSettings(x);
  }
}
