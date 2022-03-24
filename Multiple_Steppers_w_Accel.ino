#include <AccelStepper.h>

// This is how I saw steppers defined in the AccelStepper documentation, not sure if it works, NOTE step pin, direction pin
AccelStepper stepper1(AccelStepper::DRIVER, 3, 2);
AccelStepper stepper2(AccelStepper::DRIVER, 5, 4);
AccelStepper stepper3(AccelStepper::DRIVER, 7, 6);

long command = 0;
long command0 = 0;
int count = 0;

int SPEED = 60000; // shared for all servos
int ACCEL = 1000; // also shared

long destination[ ]= {0, 0, 0}; // relative to current position

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
}
void loop()
{
  recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        stepper1.moveTo(destination[0]);
        stepper2.moveTo(destination[1]);
        stepper3.moveTo(destination[2]);
        newData = false;
        
    }
  while(inMovement()){
    stepper1.run();
    stepper2.run();
    stepper3.run();
  }
 }

 bool inMovement(){
  if (stepper1.distanceToGo()!=0){return true;}
  if (stepper2.distanceToGo()!=0){return true;}
  if (stepper3.distanceToGo()!=0){return true;}
  else {return false;}
 }

void parseData() {      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get x
    destination[0] = atoi(strtokIndx);
    
    strtokIndx = strtok(NULL, ","); 
    destination[1] = atoi(strtokIndx);   

    strtokIndx = strtok(NULL, ",");
    destination[2] = atoi(strtokIndx);     
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
    Serial.print("Stepper A: ");
    Serial.println(destination[0]);
    Serial.print("Stepper B: ");
    Serial.println(destination[1]);
    Serial.print("Stepper C: ");
    Serial.println(destination[2]);
    Serial.println();
}
// Currently the inputs are are relative to current position and speed/accel are constants

// I could develop more sophisticated serial input commands to be able to switch between move and moveTo
// speed/accel of each servo could also be adjusted by serial inputs
