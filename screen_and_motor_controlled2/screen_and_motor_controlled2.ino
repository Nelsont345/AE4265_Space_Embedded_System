#include <Stepper.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(22,24,26,28,30,32);
const int stepsPerRevolution = 100; 
Stepper myStepper(stepsPerRevolution, 4, 5,6, 7);
int counter=0;
const char *direction[4] = { "North", "East", "South", "West" }; 
int potentiometer = A0;
float val = 0;  
int previous = 0;
int change = 0;



void setup() {
  lcd.begin(16, 2);
  myStepper.setSpeed(200);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(potentiometer);    // read the value from the sensor
  lcd.setCursor(0, 1);
  lcd.clear();
  change = previous - round((val/1024)*520);

  lcd.print(round((val/1024.0)*369.0)-1);
  previous = round((val/1024.0)*520.0);
  if (abs(change) > 5)
  {
    myStepper.step(change);
  }
  delay(300);
}
