#include <HX711.h>
#include <Math.h>
#include <DMD2.h>
#include <SPI.h>
#include <fonts/fixednums7x15.h>
#define DOUT  4
#define CLK  5

HX711 scale;
double oldWeight, newWeight,CurrWeight;
String Weight;

#define DISPLAYS_WIDTH 1
#define DISPLAYS_HEIGHT 1
SoftDMD dmd(DISPLAYS_WIDTH, DISPLAYS_HEIGHT);
DMD_TextBox box(dmd, 0, 0, 32, 16);

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = 120153.29; //-106600 worked for my 40Kg max scale setup

void calibrate() {
  scale.set_scale();
  Serial.println("You have 5 seconds to put 137 g");
  delay(5000);
  float CurrentWeight = scale.get_units(255);
  calibration_factor = (CurrentWeight / 0.137);
  scale.set_scale(calibration_factor);
  Serial.println(CurrentWeight);
  Serial.print("Done Calibrating : ");
  Serial.println(calibration_factor);
}

void setup() {
  Serial.begin(9600);
  dmd.setBrightness(100);
  dmd.selectFont(fixednums7x15);
  dmd.begin();
  Serial.println("HX711 Calibration");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println(" Type C to Calibrate and R to reset display");
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(50);
  scale.set_scale(calibration_factor);
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  Serial.println("HX711 ready");
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  newWeight = abs(round(scale.get_units()));
  if ((newWeight != oldWeight) || (abs(scale.get_units()) > 0.1 && abs(scale.get_units()) < 0.5)) {
    oldWeight = newWeight;
    CurrWeight =  scale.get_units();
    Weight = (String) CurrWeight+"Kg";
    Serial.println(Weight);
    Serial.print("Reading: ");
    Serial.print(scale.get_units(), 3);
    Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.println();
    dmd.drawString(0,0,Weight);
  }
  else {
    oldWeight = newWeight;
  }

  if (Serial.available()) {
    {
      char temp = Serial.read();
      if (temp == 'C' || temp == 'c') {
        calibrate();
      }
      if (temp == 'R' || temp == 'r') {
        dmd.clearScreen();
      }
    }
  }
}
