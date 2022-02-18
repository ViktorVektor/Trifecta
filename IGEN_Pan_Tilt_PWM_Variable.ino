/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

byte PWM_PIN = 10;
 
int pwm_value;
int mapped;
float diff;



int lastVal = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

    pinMode(PWM_PIN, INPUT);

      Serial.begin(9600);
}
void loop() {

    pwm_value = pulseIn(PWM_PIN, HIGH);
  mapped = map(pwm_value, 1900, 1060, -3000, 3000);
  int speed = abs(mapped);

  Serial.print(pwm_value);
  Serial.print("  ");
  Serial.print(speed);



  int pulseWidth = 3000-speed;
  int steps = 20;

  Serial.print("  ");
  Serial.print(pulseWidth);
  Serial.println();


  

if(mapped > 200){
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < (steps); x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(pulseWidth);
  }

  }


  if (mapped < -200){
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < (steps); x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(pulseWidth); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(pulseWidth); 
  }
}
}
