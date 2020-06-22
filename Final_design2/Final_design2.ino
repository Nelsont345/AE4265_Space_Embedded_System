
#include <Stepper.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(22,24,26,28,30,32);
const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 4, 5,6, 7);
Stepper myStepper2(stepsPerRevolution, 8, 9,10, 11);

int potentiometer = A0;
int counter = 0;
unsigned long t = 0;
float val = 0;  
float val2=0;
int previous = 0;
int change = 0;
int current_driver = 1;

//flags for switching
int driver_flag = 1;

struct analog_output{
  byte analog_pin;
  byte digital_pin;
  const char* color;
  int flag;
};

analog_output wires1[] = {
  {A1, 6, "green", 0},
  {A2, 7, "red", 0},
  {A3, 4, "blue", 0},
  {A4, 5, "yellow", 0}  
};

analog_output wires2[] = {
  {A1, 10, "green", 0},
  {A2, 11, "red", 0},
  {A3, 8, "blue", 0},
  {A4, 9, "yellow", 0}  
};


void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(200);
  myStepper2.setSpeed(200);
  // initialize the serial port:
  lcd.begin(16, 2);
  Serial.begin(9600);
  reset_pins(2);
}

void reset_pins(int reset_this_driver){ //only call for switch
  if (reset_this_driver ==1){
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  }
  else{
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  }
}

void fault_detection(){
  auto error_detection = [](analog_output wire[]){
    delay(100); //needed to prevent incorrect measurement
    float pin1 = 0;
    float pin2 = 0;
    for (int i =0; i<4;i++){
      int val1 = wire[i].analog_pin;
      digitalWrite(wire[i].digital_pin, LOW);
      pin1 = analogRead(val1);
      delay(50);
      digitalWrite(wire[i].digital_pin, HIGH);
      pin2 = analogRead(val1);
      delay(50);
      Serial.println(pin2-pin1);
      if (pin2-pin1 > 300){
      }
      else{
        //Serial.print(wire[i].flag);
        wire[i].flag ++ ;
      }
    }
    
  }; 
  for (int j=0; j<4; j++){ //testing 4 times the readings of the wires to determine connection
    reset_pins(2);
    error_detection(wires1);
    reset_pins(1);
    error_detection(wires2);
  }
  
  int wires1_errors = 0; //adding up errors for each driver 
  int wires2_errors = 0; 
    
    for (int j=0; j<4; j++){
      wires1_errors =  wires1_errors + wires1[j].flag;
      wires2_errors = wires2_errors +  wires2[j].flag;
      wires1[j].flag =0; //resets
      wires2[j].flag =0; //resets
    }

  lcd.setCursor(0, 2);
  if(wires1_errors > 1){
    if (wires2_errors > 1){
      Serial.println(wires2_errors);
      Serial.println("Fatal error, fault on both drivers");
      lcd.print("Both Faulty");
      driver_flag = 3;     
    }
    else {
    Serial.println("Error on driver 1 only, Using driver 2");
    lcd.print("Driver 1 Faulty");
    driver_flag = 2;
    reset_pins(2);
    }
  }
  else if(wires2_errors > 1){
    Serial.println("Error on driver 2 only, using driver 1");
    lcd.print("Driver 2 Faulty");
    driver_flag = 1;
    reset_pins(1);
  }
  else{
    Serial.print("No errors detected");
    lcd.print("No Faults detected");
    driver_flag = 1;
  }    
  counter =0;
  delay(1000);
  //step_1(change);
}



void step_1(int change) {
  if (driver_flag == 1){
    reset_pins(2);
    myStepper.step(change);
    }
  else if (driver_flag == 2){
    reset_pins(1);
    myStepper2.step(change);
    }
  else{
  }
  if (t<1500){
  unsigned long dt = millis() - t;
  delay(1500-dt);
  t = round(millis());
  }
  else{
    delay(1500);  
  }
  }


void loop() {
  //error detection, runs every 4 turns
  if (counter > 4) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Testing drivers");
    fault_detection();
  }
  else{
  }

  //checking if the position needs to be updated
  if (driver_flag<3){
    val = analogRead(potentiometer);    // read the value from the sensor
    delay(100);
    val2 = analogRead(potentiometer);    // read the value from the sensor
    if (abs(val2-val)<20){
    change = previous - round((val/1024)*520);
    previous = round((val/1024.0)*520.0);
    if (abs(change) > 8){
      lcd.setCursor(0, 1);
      lcd.clear();
      lcd.print(round((val/1024.0)*386.0)-1);
      lcd.setCursor(0, 2);
      lcd.print("turning (");
      lcd.print(counter);
      lcd.print(")");
      step_1(change);
      counter = counter + 1;
    }
    else{      
    }
    }
  }
    
  else {
    Serial.println("Awaiting repair");
    delay(1500);
    counter ++;
  }
}
