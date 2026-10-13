#include "utils.hpp"
#include "essential_modules.hpp"
#include <Arduino.h>
#include <Servo.h>


Servo servoLeft;
Servo servoRight;

void setup() {
  Serial.begin(9600);
  servoLeft.attach(servoLeftPin); 
  servoRight.attach(servoRightPin); 

  pinMode(LeftirReceiverPin, INPUT);
  pinMode(FrontirReceiverPin, INPUT);
  pinMode(RightirReceiverPin, INPUT);

  pinMode(LeftredLedPin, OUTPUT);
  pinMode(FrontredLedPin, OUTPUT);
  pinMode(RightredLedPin, OUTPUT);

}

// get sensor data
// still need to create our own scenarios 

void loop() {
  int dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
  int dL = irDistanceCm(LeftirLedPin,  LeftirReceiverPin);
  int dR = irDistanceCm(RightirLedPin, RightirReceiverPin);
  Serial.print("F:");  Serial.print(dF);
  Serial.print(" L:"); Serial.print(dL);
  Serial.print(" R:"); Serial.println(dR);

  // is there a wall
  bool leftWall = dL < OPEN; 
  bool rightWall = dR < OPEN;
  bool frontClear = dF >= FRONT_CLEAR;
  bool frontTurn = dF >= FRONT_TURN_MIN && dF <= FRONT_TURN_MAX;


  if (frontTurn) {
    if (leftWall && rightWall) {
      deadEnd();
    } else if (leftWall) {  // right side open
      if (angledInto(LeftirLedPin, LeftirReceiverPin)) badLeft30Deg();
      else idealRightTurn();
    } else if (rightWall) { // left side open
      if (angledInto(RightirLedPin, RightirReceiverPin)) badRight30Deg();
      else  idealLeftTurn();
    } else {
      unknown();
    }
  }

  else if (frontClear) {
    if (leftWall && rightWall && abs(dL - dR) <= SIDE_DIFF) {
      middleLongCorridor();
    }
    // closer to one wall we make adjustements
    else if (leftWall && dL < dR && (rightWall || dL <= NEAR)) {
      if (angledInto(LeftirLedPin, LeftirReceiverPin)) badLeft30Deg();
      else  badLeftParallel();
    }
    else if (rightWall && dR < dL && (leftWall || dR <= NEAR)) {
      if (angledInto(RightirLedPin, RightirReceiverPin)) badRight30Deg();
      else badRightParallel();
    }
    else {
      unknown();
    }
  }

  else if (dF < FRONT_TURN_MIN) {
    if (dL < dR) badLeft30Deg();
    else if (dR < dL) badRight30Deg();
    else  unknown();
  }
 
  else {
    unknown();
  }
}
