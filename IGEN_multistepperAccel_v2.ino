#include <AccelStepper.h>
#include <Math.h>

// INCHES
#define StepsPerCM  2424.24
#define SPEED 20000
#define ACCEL 5000

// This is how I saw steppers defined in the AccelStepper documentation, not sure if it works, NOTE step pin, direction pin
AccelStepper stepper1(AccelStepper::DRIVER, 3, 2);
AccelStepper stepper2(AccelStepper::DRIVER, 5, 4);
AccelStepper stepper3(AccelStepper::DRIVER, 7, 6);

/*PosVector = zeros(1,3);
  PulleyAVector = zeros(1,3);
  PulleyBVector = zeros(1,3);
  PulleyCVector = zeros(1,3);*/

// XYZ array
long PosVector[3];
long NewPosVector[3];

long PulleyAVector[3];
long PulleyBVector[3];
long PulleyCVector[3];

long VectorA[3];
long VectorB[3];
long VectorC[3];



// Motion system volume in centimeters
long XTravel = 101.5;
long YTravel = 85.5;
long ZTravel = 83.5;

long lengthA, lengthB, lengthC;

//  LengthAFinal = norm(VectorAFinal);
//  LengthBFinal = norm(VectorBFinal);
//  LengthCFinal = norm(VectorCFinal);

long VectorAFinal[3], VectorBFinal[3], VectorCFinal[3];

long lengthAFinal, lengthBFinal, lengthCFinal;

// 1cm per 100 steps
long DeltaA, DeltaB, DeltaC;


long command = 0;
long command0 = 0;
int count = 0;

//int SPEED = 10000; // shared for all servos
//int ACCEL = 2500; // also shared

long destination[] = {0, 0, 0}; // relative to current position

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

boolean newData = false;


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
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);

  // left motor
  PulleyAVector[0] = 0;
  PulleyAVector[1] = 0;
  PulleyAVector[2] = ZTravel;

  // middle motor
  PulleyBVector[0] = XTravel / 2;
  PulleyBVector[1] = YTravel;
  PulleyBVector[2] = ZTravel;

  // right motor
  PulleyCVector[0] = XTravel;
  PulleyCVector[1] = 0;
  PulleyCVector[2] = ZTravel;

  // enter origin coords
  // PosVector = [112, (YTravel / 3), 119]; % middle coord
  // origin point;
  PosVector[0] = XTravel / 2;
  PosVector[1] = (0 + YTravel + 0) / 3 ;
  PosVector[2] = ZTravel / 5;

  NewPosVector[0] = PosVector[0];
  NewPosVector[1] = PosVector[1];
  NewPosVector[2] = PosVector[2];

  // in matlab VectorA = PosVector - PulleyAVector;
  for (int x = 0; x < 3; x++)
  {
    VectorA[x] = PosVector[x] - PulleyAVector[x];
    VectorB[x] = PosVector[x] - PulleyBVector[x];
    VectorC[x] = PosVector[x] - PulleyCVector[x];
  }
}
void loop()
{
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();

    for (int x = 0; x < 3; x++)
    {
      VectorAFinal[x] = NewPosVector[x] - PulleyAVector[x];
      VectorBFinal[x] = NewPosVector[x] - PulleyBVector[x];
      VectorCFinal[x] = NewPosVector[x] - PulleyCVector[x];
    }

//    for (int x = 0; x < 3; x++)
//    {
//      lengthA[x] = lengthA[x] / sqrt(pow(VectorA[0], 2) + pow(VectorA[1], 2) + pow(VectorA[2], 2));
//      lengthB[x] = lengthB[x] / sqrt(pow(VectorB[0], 2) + pow(VectorB[1], 2) + pow(VectorB[2], 2));
//      lengthC[x] = lengthC[x] / sqrt(pow(VectorC[0], 2) + pow(VectorC[1], 2) + pow(VectorC[2], 2));
//    }
//
//
//    for (int x = 0; x < 3; x++)
//    {
//      lengthAFinal[x] = lengthAFinal[x] / sqrt(pow(VectorAFinal[0], 2) + pow(VectorAFinal[1], 2) + pow(VectorAFinal[2], 2));
//      lengthBFinal[x] = lengthBFinal[x] / sqrt(pow(VectorBFinal[0], 2) + pow(VectorBFinal[1], 2) + pow(VectorBFinal[2], 2));
//      lengthCFinal[x] = lengthCFinal[x] / sqrt(pow(VectorCFinal[0], 2) + pow(VectorCFinal[1], 2) + pow(VectorCFinal[2], 2));
//    }

    lengthA = sqrt(pow(VectorA[0], 2) + pow(VectorA[1], 2) + pow(VectorA[2], 2));
    lengthB = sqrt(pow(VectorB[0], 2) + pow(VectorB[1], 2) + pow(VectorB[2], 2));
    lengthC = sqrt(pow(VectorC[0], 2) + pow(VectorC[1], 2) + pow(VectorC[2], 2));

    lengthAFinal = sqrt(pow(VectorAFinal[0], 2) + pow(VectorAFinal[1], 2) + pow(VectorAFinal[2], 2));
    lengthBFinal = sqrt(pow(VectorBFinal[0], 2) + pow(VectorBFinal[1], 2) + pow(VectorBFinal[2], 2));
    lengthCFinal = sqrt(pow(VectorCFinal[0], 2) + pow(VectorCFinal[1], 2) + pow(VectorCFinal[2], 2));
    
    DeltaA = (lengthAFinal - lengthA) * (-StepsPerCM);
    DeltaB = (lengthBFinal - lengthB) * (-StepsPerCM);
    DeltaC = (lengthCFinal - lengthC) * (-StepsPerCM);

    showParsedData();

    stepper1.moveTo(-DeltaA);
    stepper2.moveTo(-DeltaB);
    stepper3.moveTo(-DeltaC);
    
    newData = false;

  }
  while (inMovement()) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
  }
}

bool inMovement() {
  if (stepper1.distanceToGo() != 0) {
    return true;
  }
  if (stepper2.distanceToGo() != 0) {
    return true;
  }
  if (stepper3.distanceToGo() != 0) {
    return true;
  }
  else {
    return false;
  }
}

void parseData() {      // split the data into its parts
  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ",");     // get x
  NewPosVector[0] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  NewPosVector[1] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  NewPosVector[2] = atoi(strtokIndx);
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void showParsedData() {
  Serial.println("########### ###########");
  Serial.println("ACCELSTEP DELTAS ");
  Serial.print("Stepper A: ");
  Serial.println(DeltaA);
  Serial.print("Stepper B: ");
  Serial.println(DeltaB);
  Serial.print("Stepper C: ");
  Serial.println(DeltaC);
  Serial.println();
  Serial.println("XYZ COORDS");
  Serial.print("Stepper A: ");
  Serial.println(NewPosVector[0]);
  Serial.print("Stepper B: ");
  Serial.println(NewPosVector[1]);
  Serial.print("Stepper C: ");
  Serial.println(NewPosVector[2]);
  Serial.print("########### ###########");
  Serial.println();
}
// Currently the inputs are are relative to current position and speed/accel are constants

// I could develop more sophisticated serial input commands to be able to switch between move and moveTo
// speed/accel of each servo could also be adjusted by serial inputs
