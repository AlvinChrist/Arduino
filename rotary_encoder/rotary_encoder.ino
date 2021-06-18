#define CLK 2
#define DT 3

int last_clk;
int clk_val;
int encoderPosCount = 0;
bool clock_wise;
bool button;
bool lastButton=HIGH;

void setup() {
  pinMode(7,INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  last_clk = digitalRead(CLK);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  clk_val = digitalRead(CLK);
  if (clk_val != last_clk) {
    if (digitalRead(DT) != clk_val) {
      encoderPosCount++;
      clock_wise = true;
    }
    else {
      clock_wise = false;
      encoderPosCount--;
    }
    Serial.print("Rotated: ");
    (clock_wise) ? Serial.println("clockwise") : Serial.println("Counter Clockwise");
    Serial.print("Encoder Position: ");
    Serial.println(encoderPosCount);
  }
  last_clk = clk_val;
  
  button = digitalRead(7);
  if (button == LOW && lastButton == HIGH) {
    Serial.print("Press");
  }
  else if (button == HIGH && lastButton == LOW) {
    Serial.println("  Release");
  }
  lastButton = button;
}
