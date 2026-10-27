#ifndef UTILS_HPP
#define UTILS_HPP

#include <Arduino.h>
#include <Servo.h>

extern Servo servoLeft;
extern Servo servoRight;


// pins
const int FrontirLedPin=6, FrontirReceiverPin=7;
const int FrontredLedPin = A1;
const int LeftirLedPin=10, LeftirReceiverPin=11;
const int LeftredLedPin = A2;
const int RightirLedPin=2, RightirReceiverPin=3;
const int RightredLedPin = A0;

const int servoLeftPin = 13;
const int servoRightPin = 12;

// IR calibration table
const int N = 5;
const long CAL_FREQ_FRONT[N] = {38500, 40000, 41000, 42000, 45750};
const int  CAL_DIST_FRONT[N] = {10, 8, 6, 5, 3};

const long CAL_FREQ_LEFT[N] = {38500, 39000, 40750, 41500, 45250};
const int  CAL_DIST_LEFT[N] = {10, 8, 6, 5, 3};

const long CAL_FREQ_RIGHT[N] = {37750, 39250, 40500, 41500, 44500};
const int  CAL_DIST_RIGHT[N] = {10, 8, 6, 5, 3};

const int FAR_DIST = 99;  

// for classification
const int NEAR = 3; // shortest dist
const int OPEN = 10;  // bigger than 10 is open wall
const int SIDE_DIFFERENCE = 2; // difference that counts as "off centre"
const int FRONT_CLEAR = 10; // move forward in open path
const int FRONT_TURN_MIN = 3;  // wall ahead at turning distance
const int FRONT_TURN_MAX = 8;  // between 3 and 8 cm

// speeds  or turn rates
const int  STOP_US = 1490;
const int  STANDARD_LEFT_FORWARD = 1600, STANDARD_RIGHT_FORWARD = 1375; 
const long RIGHT_SPIN_MS = 5100;   // ms for a full 360 spinning right
const long LEFT_SPIN_MS  = 4410;   // ms for a full 360 spinning left
const int  MS_PER_CM     = 66;     // straight() does 5 cm in 330 ms (stats.txt)

const int WALL_CM = 6;    
const int STEER = 60;    

// latest readings, filled in by readSensors()
extern int dF, dL, dR;

// sensors
int  irDetect(int irLedPin, int irReceiverPin, long frequency);
int  irDistanceCm(int irLedPin, int irReceiverPin);
void readSensors();
void printSensors(const char* tag);
void displayLEDwall();

// wheels - these just set the servos and return (no delay)
void straight();
void backward();
void stop();
void veerLeft();
void veerRight();
void rotateLeft();
void rotateRight();
void steer();
long spinMs(int deg, bool right);

// wheels - these block (delay) until the move is done
void forwardMs(int ms);
void backwardMs(int ms);
void turnRightXDeg(int deg);
void turnLeftXDeg(int deg);

#endif
