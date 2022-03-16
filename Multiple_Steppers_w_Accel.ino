#include <AccelStepper.h>

// This is how I saw steppers defined in the AccelStepper documentation, not sure if it works, NOTE step pin, direction pin
AccelStepper stepper1(AccelStepper::DRIVER, 3, 2);
AccelStepper stepper2(AccelStepper::DRIVER, 5, 4);
AccelStepper stepper3(AccelStepper::DRIVER, 7, 6);

long command = 0;
long command0 = 0;
int count = 0;

int SPEED = 6000; // shared for all servos
int ACCEL = 2000; // also shared

long destination[ ]= {0, 0, 0}; // relative to current position

void setup()
{  
   Serial.begin(9600);
   Serial.setTimeout(10);
     
   stepper1.setMaxSpeed(SPEED);
   stepper2.setMaxSpeed(SPEED);
   stepper3.setMaxSpeed(SPEED);
   stepper1.setAcceleration(ACCEL);
   stepper2.setAcceleration(ACCEL);
   stepper3.setAcceleration(ACCEL);
}
void loop()
{
  while (Serial.available() != 0){
    
    command = Serial.parseInt(); // converts ascii to integers
    Serial.print("command = ");
    Serial.println(command);

    if (command != command0){
      destination[count] = command;
      command0 = command;
      if (count == 2){
        count = 0;
      }
      else{
        count++;
      }
    }
    Serial.print("count = ");
    Serial.println(count);
    if (count == 0){
        stepper1.move(destination[0]);
        stepper2.move(destination[1]);
        stepper3.move(destination[2]);
      }
   
    Serial.print(destination[0]);
    Serial.print(" ");
    Serial.print(destination[1]);
    Serial.print(" ");
    Serial.print(destination[2]);
    Serial.println("");
  }
  while(stepper1.distanceToGo()!=0 && stepper2.distanceToGo()!=0 && stepper3.distanceToGo()!=0){
    stepper1.run();
    stepper2.run();
    stepper3.run();
    Serial.println(".");
  }
 }


// Currently the inputs are are relative to current position and speed/accel are constants

// I could develop more sophisticated serial input commands to be able to switch between move and moveTo
// speed/accel of each servo could also be adjusted by serial inputs
