/*
   Basic control of a Pan-tilt mechanism control using two encoders

  UNTESTED as of Feb 16 2022


*/
#include <AccelStepper.h>
#include <math.h>

#define FORWARD 0
#define BACK    1
#define STOP    2

// Stepper pin motor definitions
#define dirLeft       3
#define stepLeft      4
#define enLeft        6

#define dirRight      5
#define stepRight     5
#define enRight       7

#define interfaceType       1


// Radio input definitions
// unknown if the the left or right side joysticks will be used
// would be better if the joysticks can snap back to the center
#define deadzone 10

#define base        A0
#define baseMin     200   // minimum PWM radio input
#define baseMax     1500  // maximum PWM radio input
const int baseMid = (baseMax + baseMin) / 2;

#define pitch       A1
#define pitchMin    200
#define pitchMax    1500
const int pitchMid = (pitchMax + pitchMin) / 2;


// current position from PWM input
int current = 0;


// Stepper motor variables for positioning
int pos = 0;

// Emergency stop condition
// MAKE SURE TO ATTACH THE INTERRUPT
#define ESTOP         2
boolean STOP_THE_MACHINE = false;

void EMERGENCY_STOP()
{
  // dictate any emergency stop operations here
  STOP_THE_MACHINE = true;
  detachInterrupt(digitalPinToInterrupt(ESTOP)); 
}


/* functioning handling stepper movements
  future function will include saving to EEPROM when a button is pressed

  Input: from readEncode() FORWARD or BACK
  Motor: Stepper motor class being moved

*/
void moveStepper(int input, AccelStepper motor)
{
  const int increment = 10; // steps per click of the encoder
  pos = motor.currentPosition(); // pos is a global variable

  if (input == FORWARD)
  {
    pos += increment;
    motor.runToNewPosition(pos);
  }
  else if (input == BACK)
  {
    pos -= increment;
    motor.runToNewPosition(pos);
  }
  else
    motor.stop();
}


AccelStepper stepperLeft = AccelStepper(interfaceType, stepLeft, dirLeft);
AccelStepper stepperRight = AccelStepper(interfaceType, stepRight, dirRight);

void setup() {

  /* attach an interrupt on the ESTOP pin, then when the button state changes 
  * run the emergency stop function
  */
  attachInterrupt(digitalPinToInterrupt(ESTOP), EMERGENCY_STOP, CHANGE);

  stepperLeft.setMaxSpeed(10000);
  stepperRight.setMaxSpeed(10000);
  stepperLeft.setAcceleration(1000);
  stepperRight.setAcceleration(1000);

  // 
  pinMode(ESTOP, INPUT_PULLUP);

  // enable the left and right stepper drivers
  // pulled LOW to turn ON
  digitalWrite(enLeft, LOW);
  digitalWrite(enRight, LOW);
}

void loop() {
  if(!STOP_THE_MACHINE)
  {
    if (millis() % 200 == 0)
    {
      current = analogRead(base);

      // currently just adding/subtracking position based on joystick being forward or back
      if(current > (baseMid + deadzone))
        moveStepper(FORWARD, stepperLeft);
      if(current < (baseMid - deadzone))
        moveStepper(BACK, stepperLeft);

      current = analogRead(pitch);

      if(current > (pitchMid + deadzone))
        moveStepper(FORWARD, stepperRight);
      if(current < (pitchMid - deadzone))
        moveStepper(BACK, stepperRight);
    }
  }
  else
  {
    // switch off the motor drivers
    digitalWrite(enLeft, HIGH);
    digitalWrite(enRight, HIGH);
    while(true)
    {
      // park here until the arduino or system is power cycled
    }
  }
  
  
}
