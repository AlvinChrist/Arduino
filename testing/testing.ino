int x = 0b11111111;

void setup(){
  Serial.begin(9600);
  
}
void loop(){
  x*=2;
  Serial.println(x,BIN);
  delay(1000);
}
