#include "essential_modules/utils.h"

Servo servoLeft; 
Servo servoRight; 

void setup() {
  Serial.begin(9600);
  servoLeft.attach(9); 
  servoRight.attach(5); 
  stop();
}

void loop() {
    
    // gather distances 
    int dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
    int dL = irDistanceCm(LeftirLedPin, LeftirReceiverPin);
    int dR = irDistanceCm(RightirLedPin, RightirReceiverPin);
}

