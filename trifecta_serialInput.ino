#include <AccelStepper.h>

// This is how I saw steppers defined in the AccelStepper documentation, not sure if it works, NOTE step pin, direction pin
AccelStepper stepper1(AccelStepper::DRIVER, 3, 2);
AccelStepper stepper2(AccelStepper::DRIVER, 5, 4);
AccelStepper stepper3(AccelStepper::DRIVER, 7, 6);

AccelStepper* steppers[3]={
  &stepper1,
  &stepper2,
  &stepper3,
};

long command = 0;
long command0 = 0;


int Speed = 300; // shared for all servos
int Acceleration = 100; // also shared

long destination[ ]= {0, 0, 0}; // relative to current position

void setup()
{  
   Serial.begin(9600);
   Serial.setTimeout(10);
   delay(2000);
     
   for(int i = 0; i < 3; i++){
    steppers[3]-> setMaxSpeed(Speed);
    steppers[3]-> setAcceleration(Acceleration);
   }
}
void loop()
{
  while (Serial.available() == 0){
    command = Serial.parseInt(); // converts ascii to integers
  }
  int count = 0;
  if (command != command0){
    destination[count] = command;
    command0 = command;
    if (count >= 2){
      count = 0;
    }
    else{
      count++;
    }
  }

  if (count == 0){
    for(int i = 0; i < 3; i++){
    steppers[3]-> move(destination[i]);
   }
  }
    stepper1.run();
    stepper2.run();
    stepper3.run();
}

// Currently the inputs are are relative to current position and speed/accel are constants

// I could develop more sophisticated serial input commands to be able to switch between move and moveTo
// speed/accel of each servo could also be adjusted by serial inputs
