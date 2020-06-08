#include <Stepper.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

LiquidCrystal lcd(22,24,26,28,30,32);
const int stepsPerRevolution = 100; 
const char *wire_color[4] = { "Blue", "Yellow", "Green", "Red" }; 

Stepper myStepper(stepsPerRevolution, 4, 5,6, 7);
Stepper myStepper2(stepsPerRevolution, 8, 9,10, 11);


//measuring desired change in angle
unsigned long t = 0;
int potentiometer = A0;
float val = 0;  
int previous = 0;
int change = 0;
int counter = 0;

//test variables
int val1 = A1;
int val2 = A2;
int val3 = A3;
int val4 = A4;
float pin1= 0;
float pin2= 0;


void setup() {
  lcd.begin(16, 2);
  myStepper.setSpeed(200);
  myStepper2.setSpeed(200);
  Serial.begin(9600);
}

void step_1(int change) {
  
  if (abs(change) > 5)
  {
    lcd.setCursor(0, 2);
    lcd.print("turning");
   
    if (counter % 2 == 1){
      myStepper.step(change);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      }
    else {
      myStepper2.step(change);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      }
      
  }
  unsigned long dt = millis() - t;
  delay(1500-dt);
  t = round(millis());
  counter = counter + 1;
  }

void loop() {
  val = analogRead(potentiometer);    // read the value from the sensor
  lcd.setCursor(0, 1);
  lcd.clear();
  change = previous - round((val/1024)*520);

  lcd.print(round((val/1024.0)*386.0)-1);
  previous = round((val/1024.0)*520.0);
  step_1(change);
}
