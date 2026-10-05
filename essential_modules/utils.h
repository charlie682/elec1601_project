ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

const int FrontirLedPin=6, FrontirReceiverPin=7;   // Select these to match the IR LED/receiver pair that you are using
const int FrontredLedPin = A1;                    // Select this to match the red LED next to the IR receiver you are using
const int LeftirLedPin=10, LeftirReceiverPin=11;   // Select these to match the IR LED/receiver pair that you are using
const int LeftredLedPin = A2;      
const int RightirLedPin=2, RightirReceiverPin=3;   // Select these to match the IR LED/receiver pair that you are using
const int RightredLedPin = A0;

// LED controls

void LEDSwitch(int irLedPin, long frequency, int state);

// Servo controls

void straight();

void stop();

void turnRightXDeg(int deg);

void turnLeftXDeg(int deg);

void turnLeft45Deg();

void turnRight45Deg();

void turnLeft90Deg();

void turnRight90Deg();

void turnLeft180Deg();

void turnRight180Deg();

endif