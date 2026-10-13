#include "utils.hpp"
#include "essential_modules.hpp"
#include <Arduino.h>
#include <Servo.h>

const int max_front_turn = 8;

Servo servoLeft;
Servo servoRight;

void setup() {
  Serial.begin(9600);
  servoLeft.attach(SERVO_LEFT_PIN);
  servoRight.attach(SERVO_RIGHT_PIN);
  stop();

  pinMode(LeftirReceiverPin, INPUT);
  pinMode(FrontirReceiverPin, INPUT);
  pinMode(RightirReceiverPin, INPUT);

  pinMode(LeftredLedPin, OUTPUT);
  pinMode(FrontredLedPin, OUTPUT);
  pinMode(RightredLedPin, OUTPUT);
  setLeds(false, false, false);

  delay(3000);   // time to put the robot down
}

// SENSE -> CLASSIFY -> ACT -> (stopped) -> repeat
void loop() {
  stop();
  int dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
  int dL = irDistanceCm(LeftirLedPin,  LeftirReceiverPin);
  int dR = irDistanceCm(RightirLedPin, RightirReceiverPin);

  Serial.print("F="); Serial.print(dF);
  Serial.print(" L="); Serial.print(dL);
  Serial.print(" R="); Serial.print(dR);
  Serial.print("  -> ");

  if      (dF > 7 && dL > 6 && dR < 4)                      { Serial.println("badRightParallel"); badRightParallel(); }
  else if (dF > 7 && dL < 4 && dR > 6)                      { Serial.println("badLeftParallel");  badLeftParallel(); }
  else if (dF < 5 && dL > 6 && dR < 4)                      { Serial.println("badRight30Deg");    badRight30Deg(); }
  else if (dF < 5 && dL < 4 && dR > 6)                      { Serial.println("badLeft30Deg");     badLeft30Deg(); }
  else if (dF < max_front_turn && dL < 8 && dR < 8)         { Serial.println("deadEnd");          deadEnd(); }
  else if (dF < max_front_turn && dL < 8 && dR > 10)        { Serial.println("idealRightTurn");   idealRightTurn(); }
  else if (dF < max_front_turn && dL > 10 && dR < 8)        { Serial.println("idealLeftTurn");    idealLeftTurn(); }
  else if (dF > 7 && dL > 4 && dL < 7 && dR > 4 && dR < 7) { Serial.println("middleLongCorridor"); middleLongCorridor(); }
  else                                                      { Serial.println("unknown");          unknown(); }

  delay(200);
}
