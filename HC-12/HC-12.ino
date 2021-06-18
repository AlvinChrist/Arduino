/**
 * !        => Toggle AT Mode on/off
 * AT       => Check Status (return OK / ERROR)
 * AT+Bxxxx => set baudrate to xxxx (1200-115200)
 * AT+Cxxx  => set channel (000-100) (always have 5 channel gap for each communication)
 *            (1 channel += 0.4 MHz)
 * AT+FUx   => set UART Transmission Mode (x = 1,2,3,4)
 *             FU1  => Power saving mode (3.6 mA), air baud rate 250000bps
 *             FU2  => Full Power Saving Mode (86 µA), UART baud rate (1200,2400,4800)bps and 
 *                     air baud rate 250000bps, data transfer interval <=2 s
 *             FU3  => Full speed mode, air baud rate depends on UART baud rate (refer to 
 *                     https://statics3.seeedstudio.com/assets/file/bazaar/product/HC-12_english_datasheets.pdf)
 *             FU4  => Ultra long distance mode, UART baud rate 1200bps, air baud rate 5000bps
 *                     (distance ± 1800m)
 * 
 * AT+Px    => set transmit power level
 *              |x value             |  1 | 2 | 3 | 4 | 5  | 6  | 7  | 8  |
 *              |---------------------------------------------------------|
 *              |transmit power (dBm)| -1 | 2 | 5 | 8 | 11 | 14 | 17 | 20 |
 * 
 * 
 * AT+RX    => Display all parameters from module
 * AT+Uxxx  => Set data bits,parity(N-non,O-odd,E-even),and stop bits
 * AT+V     => Query module firmware version information, return the official website address and
               firmware version.
 * 
 * AT+SLEEP => After receiving the command, the module enters the sleep mode when exiting AT command
               mode, the operating current is about 22μA, and the module can not carry on the UART data
               transmission
               
 * AT+DEFAULT => The UART baud rate, UART communication channel, transmission mode restore factory
                 default (9600bps,C001,FU3,+20dBm) 
 */

void setup() {
  Serial.begin(115200);
  //Serial2.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  //if(Serial2.available())
    //Serial.write(Serial2.read());
  if(Serial.available())
    Serial.write(Serial.read());
  /*
  if (Serial2.available() > 0) {
    Serial.print((char)Serial2.read());
  }
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case '!':
        digitalWrite(2, !digitalRead(2));
        delay(80); //wait 80ms
        Serial.print("AT Mode ");
        if (digitalRead(2) == HIGH) {
          Serial.println("Off");
        }
        else {
          Serial.println("On");
        }
        break;
      default:
        Serial2.print(inChar);
    }
  }*/
}
