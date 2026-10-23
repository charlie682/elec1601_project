#include "utils.hpp"
#include "part1.hpp"
#include "part2.hpp"
#include <Arduino.h>
#include <Servo.h>

// ===== pick what to run =====
const int PART = 1;   // 1 = extension part 1, 2 = extension part 2
const int TASK = 1;   // part 1 only: which task (1-4)

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

  stop();
  delay(1000);                    // a second to get your hand out of the way

  if (PART == 1) runTask(TASK);   // part 1 is a fixed sequence: run it once
  else           part2Begin();
}

void loop() {
  if (PART == 2) part2Step();     // part 2 is a state machine: one quick step per loop
}
