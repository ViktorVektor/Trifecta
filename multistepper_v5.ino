#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper1(AccelStepper::DRIVER, 3, 2);
AccelStepper stepper2(AccelStepper::DRIVER, 5, 4);
AccelStepper stepper3(AccelStepper::DRIVER, 7, 6);
AccelStepper panStepper(AccelStepper::DRIVER, 10, 11);
AccelStepper tiltStepper(AccelStepper::DRIVER, 8, 9);

long command[5] = {0, 0, 0, 0, 0};
long tempTiltCommand;

#define StepsPerCM 100
#define SPEED 1000

#define panTiltSpeed 50

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

boolean newData = false;

MultiStepper steppers;

// XYZ array
long Origin[3];
long NewPosVector[3];
long OldPosVector[3];
long AdjPosVector[3];
long NewPanTilt[2];
long OldPanTilt[2];

long PulleyAVector[3];
long PulleyBVector[3];
long PulleyCVector[3];

long VectorA[3];
long VectorB[3];
long VectorC[3];


long XTravel = 224;
long YTravel = 202;
long ZTravel = 238;

long lengthA, lengthB, lengthC;
long VectorAFinal[3], VectorBFinal[3], VectorCFinal[3];
long lengthAFinal, lengthBFinal, lengthCFinal;


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  delay(2000);

  stepper1.setMaxSpeed(SPEED);
  stepper2.setMaxSpeed(SPEED);
  stepper3.setMaxSpeed(SPEED);
  panStepper.setMaxSpeed(panTiltSpeed);
  tiltStepper.setMaxSpeed(panTiltSpeed);
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);
  panStepper.setCurrentPosition(0);
  tiltStepper.setCurrentPosition(0);
  
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(panStepper);
  steppers.addStepper(tiltStepper);

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
  Origin[0] = XTravel / 2;
  Origin[1] = (0 + YTravel + 0) / 3 ;
  Origin[2] = ZTravel / 5;

  NewPosVector[0] = Origin[0];
  NewPosVector[1] = Origin[1];
  NewPosVector[2] = Origin[2];

  NewPanTilt[0] = 0;
  NewPanTilt[1] = 0;

  OldPanTilt[0] = 0;
  OldPanTilt[1] = 0;

  // in matlab VectorA = PosVector - PulleyAVector;
  for (int x = 0; x < 3; x++)
  {
    VectorA[x] = Origin[x] - PulleyAVector[x];
    VectorB[x] = Origin[x] - PulleyBVector[x];
    VectorC[x] = Origin[x] - PulleyCVector[x];
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
        noGoZoneCheck();
        
        for (int i = 0; i <= 2; i++)
          NewPosVector[i] = AdjPosVector[i];
          
        vectorCalc();
        showParsedData();
        steppers.moveTo(command);
        newData = false;
    }
    
    steppers.runSpeedToPosition();
 }

void vectorCalc(){
  for (int x = 0; x < 3; x++)
          {
            VectorAFinal[x] = NewPosVector[x] - PulleyAVector[x];
            VectorBFinal[x] = NewPosVector[x] - PulleyBVector[x];
            VectorCFinal[x] = NewPosVector[x] - PulleyCVector[x];
          }
          
   lengthA = sqrt(pow(VectorA[0], 2) + pow(VectorA[1], 2) + pow(VectorA[2], 2));
   lengthB = sqrt(pow(VectorB[0], 2) + pow(VectorB[1], 2) + pow(VectorB[2], 2));
   lengthC = sqrt(pow(VectorC[0], 2) + pow(VectorC[1], 2) + pow(VectorC[2], 2));
    
   lengthAFinal = sqrt(pow(VectorAFinal[0], 2) + pow(VectorAFinal[1], 2) + pow(VectorAFinal[2], 2));
   lengthBFinal = sqrt(pow(VectorBFinal[0], 2) + pow(VectorBFinal[1], 2) + pow(VectorBFinal[2], 2));
   lengthCFinal = sqrt(pow(VectorCFinal[0], 2) + pow(VectorCFinal[1], 2) + pow(VectorCFinal[2], 2));
        
   command[2] = -(lengthAFinal - lengthA) * (-StepsPerCM);
   command[1] = -(lengthBFinal - lengthB) * (-StepsPerCM);
   command[0] = -(lengthCFinal - lengthC) * (-StepsPerCM);

   command[3] += (NewPanTilt[0] - OldPanTilt[0]);
   command[4] += (OldPanTilt[0] - NewPanTilt[0] + (NewPanTilt[1] - OldPanTilt[1]));

   OldPanTilt[0] = command[3];
   OldPanTilt[1] = NewPanTilt[1];
   
}

void parseData() {      // split the data into its parts
  char * strtokIndx; // this is used by strtok() as an index
  
  for(int i=0; i<3; i++)
    OldPosVector[i] = NewPosVector[i];
    
  strtokIndx = strtok(tempChars, ",");     // get x
  NewPosVector[0] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  NewPosVector[1] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  NewPosVector[2] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  NewPanTilt[0] = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  NewPanTilt[1] = atoi(strtokIndx);
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
  Serial.println(command[2]);
  Serial.print("Stepper B: ");
  Serial.println(command[1]);
  Serial.print("Stepper C: ");
  Serial.println(command[0]);
  Serial.println();
  Serial.println("XYZ COORDS");
  Serial.print("Stepper A: ");
  Serial.println(NewPosVector[0]);
  Serial.print("Stepper B: ");
  Serial.println(NewPosVector[1]);
  Serial.print("Stepper C: ");
  Serial.println(NewPosVector[2]);
  Serial.println();
  Serial.println("PAN TILT");
  Serial.print("Pan: ");
  Serial.println(NewPanTilt[0]);
  Serial.print("Tilt: ");
  Serial.println(NewPanTilt[1]);
  Serial.print("########### ###########");
  Serial.println();
}

void noGoZoneCheck(){
  long noGo[] = {50,0,50,0,30,0}; // upper then lower, x -> y -> z
  long desiredPath[3];
  bool inZone[] = {0,0,0};
  int numDivisions = 10; // determines how many checks we make for no go zones
  int bufferZone = 5; // how far away from boundary is our rerouted position
  
  for (int a = 0; a <= 2; a++)
     AdjPosVector[a] = NewPosVector[a];
  
  for (int b = 0; b <= 2; b++)
    desiredPath[b] = (NewPosVector[b] - OldPosVector[b]) / numDivisions;
  
  while (true){
    for(int i = 0; i <= 4; i += 2){ 
      for (int j = 0; j <= numDivisions; j++){
        if (OldPosVector[i/2] + j*desiredPath[i/2] < noGo[i] && OldPosVector[i/2] + j*desiredPath[i/2] > noGo[i+1]){
          if(OldPosVector[i/2] >= noGo[i])
            AdjPosVector[i/2] = noGo[i] + bufferZone;
          else if (OldPosVector[i/2] <= noGo[i+1])
            AdjPosVector[i/2] = noGo[i+1] - bufferZone;
          inZone[i/2] = true;
          break;
        }
      }
    }
    
    for(int c = 0; c <= 2; c++){
      if(inZone[c] == 0){
        for(int d = 0; d <= 2; d++)
          AdjPosVector[d] = NewPosVector[d];
        return;
      }    
    }
    Serial.println("Stopped: Obstacle in path!");
    return;  
  }
 }

// Currently the inputs are are relative to current position and speed/accel are constants

// I could develop more sophisticated serial input commands to be able to switch between move and moveTo
// speed/accel of each servo could also be adjusted by serial inputs
