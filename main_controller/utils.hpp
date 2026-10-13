#ifndef UTILS_HPP
#define UTILS_HPP

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

extern Servo servoLeft; 
extern Servo servoRight; 

// pins 
const int FrontirLedPin=6, FrontirReceiverPin=7;   // Select these to match the IR LED/receiver pair that you are using
const int FrontredLedPin = A1;                    // Select this to match the red LED next to the IR receiver you are using
const int LeftirLedPin=10, LeftirReceiverPin=11;   // Select these to match the IR LED/receiver pair that you are using
const int LeftredLedPin = A2;      
const int RightirLedPin=2, RightirReceiverPin=3;   // Select these to match the IR LED/receiver pair that you are using
const int RightredLedPin = A0;

const int servoLeftPin = 13;
const int servoRightPin = 12;

// IR calibration table 
const int N = 5;
const long CAL_FREQ_FRONT[N] = {38500, 40000, 41000, 42000, 45750};  
const int  CAL_DIST_FRONT[N] = {10, 8, 6, 5, 3}; //1.5 46000

const long CAL_FREQ_LEFT[N] = {38500, 39000, 40750, 41500, 45250};  
const int  CAL_DIST_LEFT[N] = {10, 8, 6, 5, 3}; //1.5 48750

const long CAL_FREQ_RIGHT[N] = {37750, 39250, 40500, 41500, 44500};  
const int  CAL_DIST_RIGHT[N] = {10, 8, 6, 5, 3}; //1.5 48500

const int FAR_DIST = 99; // returned when we dont detect any wall 

// for classificaiton 
const int NEAR = 3;  //shortest dist
const int OPEN = 10; //bigger than 10 is open wall
const int SIDE_DIFF = 2; // if the difference is 2, then we are in corridor center
const int FRONT_CLEAR = 10;  // move forward in open path
const int FRONT_TURN_MIN = 4;  // wall ahead at turning distance
const int FRONT_TURN_MAX = 8;  // between 4 and 8 cm
const int PROBE_MS = 100; // for checking parallel/angled (TUNE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
const int PROBE_CHANGE = 2;  // if side reading grows by >= 2 cm -> angled


enum LedMode { LED_OFF, LED_ON, LED_FLASH };
void displayCode(LedMode r, LedMode m, LedMode l);

void forwardMs(int ms);
void backwardMs(int ms);

// IR distance

int irDetect(int irLedPin, int irReceiverPin, long frequency);

int irDistanceCm(int irLedPin, int irReceiverPin);

bool angledInto(int irLed, int irRx)

// LED controls

void LEDSwitch(int irLedPin, long frequency, int state);

// Servo controls

void straight();

void backward();

void stop();

void turnRightXDeg(int deg);

void turnLeftXDeg(int deg);

void turnLeft45Deg();

void turnRight45Deg();

void turnLeft90Deg();

void turnRight90Deg();

void turnLeft180Deg();

void turnRight180Deg();

#endif