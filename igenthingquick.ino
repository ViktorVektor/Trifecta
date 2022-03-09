/*     Simple Stepper Motor Control Exaple Code

    by Dejan Nedelkovski, www.HowToMechatronics.com

*/

#include <AccelStepper.h>
#include <MultiStepper.h>
#include <math.h>

#define FORWARD 0
#define BACK 1
#define STOP 2

// Stepper A
#define dirA  2
#define stepA 3

// Stepper B
#define dirB  4
#define stepB 5

// Stepper C
#define dirC  6
#define stepC 7

#define globalSpeed 1000000
#define globalAccel 1000000

byte PWM_A = 10;
byte PWM_B = 11;
byte PWM_C = 12;

int pwmValueA;
int mapA;
int speedA;

int pwmValueB;
int mapB;
int speedB;

int pwmValueC;
int mapC;
int speedC;

int pulseWidth = 200;
int steps = 200;
int pos;

int lastVal = 0;

AccelStepper stepperA = AccelStepper(1, stepA, dirA);
AccelStepper stepperB = AccelStepper(1, stepB, dirB);
AccelStepper stepperC = AccelStepper(1, stepC, dirC);

MultiStepper steppers;

// function for defining stepper movement direction
// moves the steppers for a certain number of steps
void moveStepper(int input, AccelStepper motor)
{
  const int increment = 10; // steps per click of the encoder
  pos = motor.currentPosition(); // pos is a global variable

  if (input == FORWARD)
  {
    pos += increment;
    motor.moveTo(pos);
  }
  else if (input == BACK)
  {
    pos -= increment;
    motor.moveTo(pos);
  }
  else
    motor.stop();
}

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepA, OUTPUT);
  pinMode(dirA, OUTPUT);

  pinMode(stepB, OUTPUT);
  pinMode(dirB, OUTPUT);

  pinMode(stepC, OUTPUT);
  pinMode(dirC, OUTPUT);

  pinMode(PWM_A, INPUT);
  pinMode(PWM_B, INPUT);
  pinMode(PWM_C, INPUT);

  Serial.begin(9600);

  stepperA.setMaxSpeed(globalSpeed);
  stepperA.setAcceleration(globalAccel);

  stepperB.setMaxSpeed(globalSpeed);
  stepperB.setAcceleration(globalAccel);

  stepperC.setMaxSpeed(globalSpeed);
  stepperC.setAcceleration(globalAccel);

  steppers.addStepper(stepperA);
  steppers.addStepper(stepperB);
  steppers.addStepper(stepperC);

}
void loop() {

  pwmValueA = pulseIn(PWM_A, HIGH);
  mapA = map(pwmValueA, 1900, 1060, -3000, 3000);
  speedA = abs(mapA);

  pwmValueB = pulseIn(PWM_B, HIGH);
  mapB = map(pwmValueB, 1700, 1150, -3000, 3000);
  speedB = abs(mapB);

  pwmValueC = pulseIn(PWM_C, HIGH);
  mapC = map(pwmValueC, 1700, 1060, -3000, 3000);
  speedC = abs(mapC);


  if (mapA > 200) {
    //    digitalWrite(dirPin, LOW); //Changes the rotations direction
    // Makes 400 pulses for making two full cycle rotation
    for(int x = 0; x < (steps); x++) {
        moveStepper(FORWARD, stepperA);
    }
  }
  else if (mapA < -200) {
    for(int x = 0; x < (steps); x++) {
        moveStepper(BACK, stepperA);
    }
  }
  else
    moveStepper(STOP, stepperA);

  if (mapB > 200) {
    for(int x = 0; x < (steps); x++) {
        moveStepper(FORWARD, stepperB);
    }
  }
  else if (mapB < -200) {
    for(int x = 0; x < (steps); x++) {
        moveStepper(BACK, stepperB);
    }
  }
  else
    moveStepper(STOP, stepperB);

  if (mapC > 200) {
    for(int x = 0; x < (steps); x++) {
        moveStepper(FORWARD, stepperC);
    }
  }
  else if (mapC < -200) {
    for(int x = 0; x < (steps); x++) {
        moveStepper(BACK, stepperC);
    }
  }
  else
    moveStepper(STOP, stepperC);
}
