#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper1(AccelStepper::DRIVER, 3, 2);
AccelStepper stepper2(AccelStepper::DRIVER, 5, 4);
AccelStepper stepper3(AccelStepper::DRIVER, 7, 6);

long positions[3] = {0, 0, 0};

int SPEED = 5000; // shared for all servos
int ACCEL = 1000; // also shared

long destination[ ]= {0, 0, 0}; // relative to current position

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

boolean newData = false;

MultiStepper steppers;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  delay(2000);

  stepper1.setMaxSpeed(SPEED);
  stepper2.setMaxSpeed(SPEED);
  stepper3.setMaxSpeed(SPEED);
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);
  
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
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
        steppers.moveTo(destination);
        newData = false;
    }
    steppers.runSpeedToPosition();

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
