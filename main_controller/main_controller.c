#include "utils.h"
#include "essential_modules.h"
#include <Arduino.h>
#include <Servo.h>

Servo servoLeft; 
Servo servoRight; 

// constants 
const int min_front_turn = 3;
const int max_front_turn = 8;

void setup() {
  Serial.begin(9600);
  servoLeft.attach(9); 
  servoRight.attach(5); 
  stop();
}

// get sensor data
// still need to create our own scenarios 


void loop() {
  int dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
  int dL = irDistanceCm(LeftirLedPin,  LeftirReceiverPin);
  int dR = irDistanceCm(RightirLedPin, RightirReceiverPin);

  if (dF > 7 && dL > 6 && dR < 4)               badRightParallel();
  else if (dF > 7 && dL < 4 && dR > 6)          badLeftParallel();
  else if (dF < 5 && dL > 6 && dR < 4)          badRight30Deg();
  else if (dF < 5 && dL < 4 && dR > 6)          badLeft30Deg();
  else if (dF < max_front_turn && dL < 8 && dR < 8) deadEnd();
  else if (dF < max_front_turn && dL < 8 && dR > 10) idealRightTurn();
  else if (dF < max_front_turn && dL > 10 && dR < 8) idealLeftTurn();
  else if (dF > 7 && dL > 4 && dL < 7 && dR > 4 && dR < 7) middleLongCorridor();
  else unknown();
}


