ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

// pins 
const int FrontirLedPin=6, FrontirReceiverPin=7;   // Select these to match the IR LED/receiver pair that you are using
const int FrontredLedPin = A1;                    // Select this to match the red LED next to the IR receiver you are using
const int LeftirLedPin=10, LeftirReceiverPin=11;   // Select these to match the IR LED/receiver pair that you are using
const int LeftredLedPin = A2;      
const int RightirLedPin=2, RightirReceiverPin=3;   // Select these to match the IR LED/receiver pair that you are using
const int RightredLedPin = A0;

// IR calibration table 
// currently just placeholders. calibrate on first lab
const int N = 4;
const long CAL_FREQ_FRONT[N] = {38000, 39000, 40000, 41000};  
const int  CAL_DIST_FRONT[N] = {10, 8, 5, 3};

const long CAL_FREQ_LEFT[N] = {38000, 39000, 40000, 41000};  
const int  CAL_DIST_LEFT[N] = {10, 8, 5, 3};

const long CAL_FREQ_RIGHT[N] = {38000, 39000, 40000, 41000};  
const int  CAL_DIST_RIGHT[N] = {10, 8, 5, 3};

const int FAR_DIST = 99; // returned when we dont detect any wall 

// IR distance

int irDetect(int irLedPin, int irReceiverPin, long frequency);

int irDistance(int irLedPin, int irReceiverPin);


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