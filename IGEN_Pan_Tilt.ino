/* Tester code for the Tilt Pan System


   Uses AccelStepper library
   Feb 16 2022
*/

#include <AccelStepper.h>
#include <math.h>

#define DEBUG

#define dirPin 2
#define stepPin 3
#define dirPinTwo 7
#define stepPinTwo 8
#define motorInterfaceType 1

#define leftStepper A0
#define rightStepper A1

int leftPos = 0;
int leftRead = 0;

int rightPos = 0;
int rightRead = 0;

int curr = 0;

// Define a stepper and the pins it will use
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5);
AccelStepper stepperTwo = AccelStepper(motorInterfaceType, stepPinTwo, dirPinTwo); // second stepper motor

void setup()
{
  Serial.begin(9600);
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(40000);
  stepperTwo.setMaxSpeed(1000);
  stepper.setAcceleration(5000);
  stepperTwo.setAcceleration(500); //degrees per second
}

void loop()
{
  if (millis() % 250 == 0) // position update every 250ms
  {
    leftRead = analogRead(leftStepper);
    rightRead = analogRead(rightStepper);
    curr = map(leftRead, 0, 1023, 0, 1600); //1600 steps per rev setting on the driver
    if (abs(curr - leftPos) > 20) // only update position if it's substantially different
    {
      leftPos = curr;
    }

    curr = map(rightRead, 0, 1023, 0, 1600);
    if (abs(curr - rightPos) > 20)
    {
      rightPos = curr;
    }
    Serial.print(" AAAAAA ");
    Serial.print(leftPos);
    Serial.print(" BBBBBB ");
    Serial.println(rightPos);
  }

  stepper.runToNewPosition(leftPos);

  stepperTwo.runToNewPosition(rightPos);

}
