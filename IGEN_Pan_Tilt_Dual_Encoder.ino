/*
   Basic control of a Pan-tilt mechanism control using two encoders

  UNTESTED as of Feb 16 2022


*/
#include <AccelStepper.h>
#include <Wire.h> // for I2C bus
#include <math.h>

#define FORWARD 0
#define BACK    1
#define STOP    2

// Stepper pin motor definitions
#define dirLeft       3
#define stepLeft      4
#define dirRight      5
#define stepRight     5
#define enLeft        6
#define enRight       7

// encoder pin definitions
#define leftCLK       8
#define rightCLK      9
#define leftDT        10
#define rightDT       11
#define leftSW        12
#define rightSW       13

#define interfaceType       1

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

/*
   class for a rotary encoder

*/
class Encoder
{
    // conditional variables
    int currentStateCLK;
    int lastStateCLK;
    int btnState;
    unsigned long lastButtonPress = 0;
    // encoder pins
    int CLK;
    int DT;
    int SW;

  public:
    Encoder(int encoderCLK, int encoderDT, int encoderSW)
    {
      CLK = encoderCLK;
      DT = encoderDT;
      SW = encoderSW;

      currentStateCLK = digitalRead(CLK);
      btnState - digitalRead(SW);
    }

    // handles forward or backward encoding
    int readEncode()
    {
      currentStateCLK = digitalRead(CLK);

      if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
      {
        if (digitalRead(DT) != currentStateCLK)
          return BACK;
        else
          return FORWARD;
      }
      lastStateCLK = currentStateCLK;

    }

    // handles switch response
    boolean readSwitch()
    {
      btnState = digitalRead(SW);

      if (btnState == LOW)
      {
        //debouncing
        if (millis() - lastButtonPress > 50)
        {
          return true;
        }

        lastButtonPress = millis();
      }

      delay(1); //debouncing
    }

    // end of class Encoder
};

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

Encoder leftEncoder = Encoder(leftCLK, leftDT, leftSW);
Encoder rightEncoder = Encoder(rightCLK, rightDT, rightSW);

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
      moveStepper(leftEncoder.readEncode(), stepperLeft);
      moveStepper(rightEncoder.readEncode(), stepperRight);
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
