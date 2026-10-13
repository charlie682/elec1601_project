#ifndef UTILS_HPP
#define UTILS_HPP

#include <Arduino.h>
#include <Servo.h>

extern Servo servoLeft;
extern Servo servoRight;

// ---------- PINS ----------
const int FrontirLedPin = 6,  FrontirReceiverPin = 7;
const int LeftirLedPin  = 10, LeftirReceiverPin  = 11;
const int RightirLedPin = 2,  RightirReceiverPin = 3;
const int FrontredLedPin = A1, LeftredLedPin = A2, RightredLedPin = A0;

// Servo signal pins - these are what your original main_controller used.
// If the wheels don't move at all, check which pins the servo wires
// are actually plugged into and change these two numbers.
const int SERVO_LEFT_PIN  = 9;
const int SERVO_RIGHT_PIN = 5;

// ---------- MOTION TUNING (all in one place) ----------
const int STOP_US     = 1490;
const int FWD_LEFT_US = 1600;   // left wheel forward  = ABOVE 1490
const int FWD_RIGHT_US = 1375;  // right wheel forward = BELOW 1490 (mirrored servo)
const int TURN_CW_US  = 1525;   // both > 1490 -> spin clockwise (turn right)
const int TURN_CCW_US = 1455;   // both < 1490 -> turn left. Same distance from 1490 as CW (35). TEST a 90 deg left turn and adjust.
const long MS_PER_360 = 5175;   // measured time for a full spin
const int  MS_PER_5CM = 330;    // measured time to drive 5 cm
const int  SETTLE_MS  = 150;    // pause after each move so the robot stops rocking
const int  EXIT_MS    = 1500;   // drive after a turn (needs tuning)

// ---------- IR CALIBRATION TABLE ----------
// Re-measure these on the LAB robot with freq.c. Higher freq = wall is closer.
const int N = 5;
const long CAL_FREQ_FRONT[N] = {38500, 40000, 41000, 42000, 45750};
const int  CAL_DIST_FRONT[N] = {10, 8, 6, 5, 3};
const long CAL_FREQ_LEFT[N]  = {38500, 39000, 40750, 41500, 45250};
const int  CAL_DIST_LEFT[N]  = {10, 8, 6, 5, 3};
const long CAL_FREQ_RIGHT[N] = {37750, 39250, 40500, 41500, 44500};
const int  CAL_DIST_RIGHT[N] = {10, 8, 6, 5, 3};
const int  FAR_DIST = 99;       // returned when no wall is detected

// ---------- SENSING ----------
int irDetect(int irLedPin, int irReceiverPin, long frequency);
int irDistanceCm(int irLedPin, int irReceiverPin);

// ---------- INDICATOR LEDS (the RED ones) ----------
void setLeds(bool left, bool mid, bool right);
void blinkLed(int pin, int times);

// ---------- MOVEMENT ----------
void straight();
void backward();
void stop();
void driveMs(int ms);      // forward, then stop
void reverseMs(int ms);    // backward, then stop
void turnRightXDeg(int deg);
void turnLeftXDeg(int deg);

#endif