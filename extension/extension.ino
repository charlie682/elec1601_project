#include "utils.hpp"
#include "part1.hpp"
#include "part2.hpp"
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

  stop();
  delay(1000);  
  
  part2Begin();
}

void loop() {
  part2Step();
}
