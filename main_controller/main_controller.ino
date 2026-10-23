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

  bool leftWall   = dL < OPEN;
  bool rightWall  = dR < OPEN;
  bool frontClear = dF >= FRONT_CLEAR;
  bool frontTurn  = dF >= FRONT_TURN_MIN && dF <= FRONT_TURN_MAX;

  bool nearLeft  = dL <= NEAR;        // right up against the left wall
  bool nearRight = dR <= NEAR;        // right up against the right wall
  bool seesLeft  = dL < FAR_DIST;     // left sensor sees ANYTHING (even a far wall)
  bool seesRight = dR < FAR_DIST;     // right sensor sees ANYTHING

  // ---------- 1) work out which scenario we're in ----------
  int scenario = 0;

  // own 9: corner - all three walls very close (check BEFORE dead end, which would also match)
  if (dF <= CORNER_CLOSE && leftWall && rightWall && (nearLeft || nearRight))
    scenario = 9;

  else if (frontTurn && leftWall && rightWall)
  scenario = 4;                                 // dead end

  // up against one wall: parallel still sees the far wall, 30 deg doesn't
  else if (nearLeft && !nearRight)
    scenario = seesRight ? 5 : 7;                   // 5 = parallel left, 7 = 30 deg left
  else if (nearRight && !nearLeft)
    scenario = seesLeft ? 6 : 8;                    // 6 = parallel right, 8 = 30 deg right

  // wall ahead at turning distance, one side open
  else if (frontTurn && leftWall && !rightWall)
    scenario = 2;                                   // ideal right turn
  else if (frontTurn && rightWall && !leftWall)
    scenario = 3;                                   // ideal left turn

  // nothing ahead
  else if (frontClear && leftWall && rightWall && abs(dL - dR) <= SIDE_DIFF)
    scenario = 1;                                   // centred corridor
  else if (frontClear && leftWall && rightWall)
    scenario = (dL < dR) ? 5 : 6;                   // off-centre but not touching a wall
  else if (frontClear && leftWall != rightWall)
    scenario = 1;                                   // one wall only -> go straight

  // own 10: front far, nothing on right, left wall only faintly seen (far away)
  else if (frontClear && !seesRight && seesLeft && !leftWall)
    scenario = 10;

  Serial.print("F:");  Serial.print(dF);
  Serial.print(" L:"); Serial.print(dL);
  Serial.print(" R:"); Serial.print(dR);
  Serial.print("  -> scenario "); Serial.println(scenario);

  // ---------- 2) run it ----------
  switch (scenario) {
    case 1:  middleLongCorridor(); break;
    case 2:  idealRightTurn();     break;
    case 3:  idealLeftTurn();      break;
    case 4:  deadEnd();            break;
    case 5:  badLeftParallel();    break;
    case 6:  badRightParallel();   break;
    case 7:  badLeft30Deg();       break;
    case 8:  badRight30Deg();      break;
    case 9:  ownChoice1();         break;
    case 10: ownChoice2();         break;
    default: unknown();            break;
  }
}

// void loop() {
//   int dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
//   int dL = irDistanceCm(LeftirLedPin,  LeftirReceiverPin);
//   int dR = irDistanceCm(RightirLedPin, RightirReceiverPin);
//   Serial.print("F:");  Serial.print(dF);
//   Serial.print(" L:"); Serial.print(dL);
//   Serial.print(" R:"); Serial.println(dR);

//   // is there a wall
//   bool leftWall = dL < OPEN; 
//   bool rightWall = dR < OPEN;
//   bool frontClear = dF >= FRONT_CLEAR;
//   bool frontTurn = dF >= FRONT_TURN_MIN && dF <= FRONT_TURN_MAX;


//   if (frontTurn) {
//     if (leftWall && rightWall) {
//       deadEnd();
//     } else if (leftWall && dL < 4 && frontClear) {
//       badLeftParallel();
//     } else if (rightWall && dR < 4 && frontClear) {
//       badRightParallel();
//     } else if (leftWall) { // right side open
//       if (angledInto(LeftirLedPin, LeftirReceiverPin)) badLeft30Deg();
//       else idealRightTurn();
//     } else if (rightWall) { // left side open
//       if (angledInto(RightirLedPin, RightirReceiverPin)) badRight30Deg();
//       else  idealLeftTurn();
//     } else {
//       unknown();
//     }
//   }

//   else if (frontClear) {
//     if (leftWall && rightWall && abs(dL - dR) <= SIDE_DIFF) {
//       middleLongCorridor();
//     }
//     // closer to one wall we make adjustements
//     else if (leftWall && dL < dR && (rightWall || dL <= NEAR)) {
//       if (angledInto(LeftirLedPin, LeftirReceiverPin)) badLeft30Deg();
//       else  badLeftParallel();
//     }
//     else if (rightWall && dR < dL && (leftWall || dR <= NEAR)) {
//       if (angledInto(RightirLedPin, RightirReceiverPin)) badRight30Deg();
//       else badRightParallel();
//     }
//     else {
//       unknown();
//     }
//   }

//   else if (dF < FRONT_TURN_MIN) {
//     if (dL < dR) badLeft30Deg();
//     else if (dR < dL) badRight30Deg();
//     else  unknown();
//   }

//   else if (leftWall != rightWall) {
//       middleLongCorridor();
//   }

//   else {
//     unknown();
//   }
// }
