#include "utils.hpp"
#include "essential_modules.hpp"

// Every module ENDS with the robot stopped, so the next sensor read is accurate.
// LED order below is (left, mid, right).

void unknown() {
  setLeds(false, false, false);
  stop();
}

void middleLongCorridor() {
  setLeds(false, false, true);
  driveMs(MS_PER_5CM);               // small step, then main loop re-senses
}

void idealLeftTurn() {
  setLeds(false, true, false);
  turnLeftXDeg(90);
  driveMs(EXIT_MS);
}

void idealRightTurn() {
  setLeds(false, true, false);
  turnRightXDeg(90);
  driveMs(EXIT_MS);
}

void deadEnd() {
  setLeds(true, false, false);
  turnRightXDeg(180);
  driveMs(EXIT_MS);
}

// Scenario 6: too close to the RIGHT wall -> nudge left (anticlockwise) a bit
void badRightParallel() {
  setLeds(true, true, false);
  turnLeftXDeg(10);        // anticlockwise
  driveMs(MS_PER_5CM);
  turnRightXDeg(10);       // clockwise (straighten up)
  reverseMs(MS_PER_5CM);
}

// Scenario 5: too close to the LEFT wall -> nudge right (clockwise) a bit
void badLeftParallel() {
  setLeds(true, false, true);
  turnRightXDeg(10);       // clockwise
  driveMs(MS_PER_5CM);
  turnLeftXDeg(10);        // anticlockwise (straighten up)
  reverseMs(MS_PER_5CM);
}

void badRight30Deg() {
  setLeds(true, true, false);
  blinkLed(RightredLedPin, 3);        // "flash"; tone(pin,1) was invalid
  turnLeftXDeg(20);

  for (int i = 0; i < 3; i++) {
    int d0 = irDistanceCm(RightirLedPin, RightirReceiverPin);
    driveMs(400);
    int d1    = irDistanceCm(RightirLedPin, RightirReceiverPin);
    int front = irDistanceCm(FrontirLedPin, FrontirReceiverPin);

    if (front <= 5) { turnLeftXDeg(15); continue; }
    if (d0 == FAR_DIST || d1 == FAR_DIST) break;   // lost the wall, don't trust the maths

    if (d1 - d0 < -1)      turnLeftXDeg(8);    // getting closer to right wall
    else if (d1 - d0 > 1)  turnRightXDeg(8);   // over-corrected
    else break;                                // parallel
  }
  driveMs(EXIT_MS);
}

void badLeft30Deg() {
  setLeds(true, true, true);
  turnRightXDeg(20);

  for (int i = 0; i < 3; i++) {
    int d0 = irDistanceCm(LeftirLedPin, LeftirReceiverPin);
    driveMs(400);
    int d1    = irDistanceCm(LeftirLedPin, LeftirReceiverPin);
    int front = irDistanceCm(FrontirLedPin, FrontirReceiverPin);

    if (front <= 5) { turnRightXDeg(15); continue; }
    if (d0 == FAR_DIST || d1 == FAR_DIST) break;

    if (d1 - d0 < -1)      turnRightXDeg(8);   // getting closer to left wall
    else if (d1 - d0 > 1)  turnLeftXDeg(8);    // over-corrected
    else break;
  }
  driveMs(EXIT_MS);
}