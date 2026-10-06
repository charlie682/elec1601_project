#include "essential_modules/utils.h"
#include "essential_modules/essential_modules.h"

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
    
    // gather distances 
    int dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
    int dL = irDistanceCm(LeftirLedPin, LeftirReceiverPin);
    int dR = irDistanceCm(RightirLedPin, RightirReceiverPin);

    // dead end
    if (dF < max_front_turn && (dL < 8) && (dR < 8)) {
      deadend(); 
    // bad parallel right 
    } else if ((dF > 7) && (dL > 6) && (dR < 4))) {
      badRightParallel();
    // bad parallel left
    } else if ((dF > 7) && (dL < 4) && (dR > 6))) {
      badLeftParallel();
    // bad 30deg right
    } else if ((dF < 5) && (dL > 6) && (dR < 4))) {
      badRight30Deg();
    // bad 30deg left
    } else if ((dF < 5) && (dL < 4) && (dR > 6))) {
      badLeft30Deg();
    // ideal right turn
    } else if ((dF < max_front_turn) && (dL < 8) && (dR > 10)) {
      idealRightTurn();
    // ideal left turn
    } else if ((dF < max_front_turn) && (dL > 10) && (dR < 8)) {
      idealLeftTurn();
    // middle long corridor
    } else if ((dF > 7) && (dL > 4 && dL < 7) && (dR > 4 && dR < 7)) {
      middleLongCorridor();
    //unknown
    } else {
      unknown(); 
    }
}

