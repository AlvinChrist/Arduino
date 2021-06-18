#define CLK_PIN  9
#define DATA_PIN 8

////////////////////////////////////////////////////
void setup() {
   pinMode(CLK_PIN,INPUT_PULLUP);
   pinMode(DATA_PIN,INPUT_PULLUP);

   Serial.begin(9600);
   Serial.println("Rotary Encoder KY-040");
}

////////////////////////////////////////////////////
void loop() {
static uint16_t counter=0;
static uint8_t state=0;

    delayMicroseconds(100); // Simulate doing somehing else as well.

    state=(state<<1) | digitalRead(CLK_PIN) | 0xe0;

    if (state==0xf0){
       state=0x00;
       if(!digitalRead(DATA_PIN))
         counter++;
       else
         counter--;
       Serial.println(counter);
    }
}
