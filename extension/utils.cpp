#include "utils.hpp"

int dF = FAR_DIST, dL = FAR_DIST, dR = FAR_DIST;

// ================= IR (copied from main_controller) =================

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  noTone(irLedPin);                          // Turn off the IR LED
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}

int irDistanceCm(int irLedPin, int irReceiverPin)
{
  long maxF = 0;

  // Scan from high to low so we get closest dist first
  for (long f = 49000; f >= 37000; f -= 500)
  {
    if (irDetect(irLedPin, irReceiverPin, f) == 0)
    {
      maxF = f;
      break;
    }
  }

  if (maxF == 0) return FAR_DIST; // no detection

  // boundary conditions on our table
  if (irReceiverPin == FrontirReceiverPin) {
    if (maxF <= CAL_FREQ_FRONT[0])     return CAL_DIST_FRONT[0];
    if (maxF >= CAL_FREQ_FRONT[N - 1]) return CAL_DIST_FRONT[N - 1];
  } else if (irReceiverPin == LeftirReceiverPin) {
    if (maxF <= CAL_FREQ_LEFT[0])      return CAL_DIST_LEFT[0];
    if (maxF >= CAL_FREQ_LEFT[N - 1])  return CAL_DIST_LEFT[N - 1];
  } else if (irReceiverPin == RightirReceiverPin) {
    if (maxF <= CAL_FREQ_RIGHT[0])     return CAL_DIST_RIGHT[0];
    if (maxF >= CAL_FREQ_RIGHT[N - 1]) return CAL_DIST_RIGHT[N - 1];
  }

  // choose dist between the calibration points roughly linear estimate
  for (int i = 0; i < N - 1; i++) {
    if (irReceiverPin == FrontirReceiverPin) {
      if (maxF <= CAL_FREQ_FRONT[i + 1])
        return map(maxF, CAL_FREQ_FRONT[i], CAL_FREQ_FRONT[i + 1], CAL_DIST_FRONT[i], CAL_DIST_FRONT[i + 1]);
    } else if (irReceiverPin == LeftirReceiverPin) {
      if (maxF <= CAL_FREQ_LEFT[i + 1])
        return map(maxF, CAL_FREQ_LEFT[i], CAL_FREQ_LEFT[i + 1], CAL_DIST_LEFT[i], CAL_DIST_LEFT[i + 1]);
    } else if (irReceiverPin == RightirReceiverPin) {
      if (maxF <= CAL_FREQ_RIGHT[i + 1])
        return map(maxF, CAL_FREQ_RIGHT[i], CAL_FREQ_RIGHT[i + 1], CAL_DIST_RIGHT[i], CAL_DIST_RIGHT[i + 1]);
    }
  }

  return FAR_DIST;
}

// one reading of all three sensors into dF, dL, dR (same calls as main_controller's loop)
void readSensors() {
  dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
  dL = irDistanceCm(LeftirLedPin,  LeftirReceiverPin);
  dR = irDistanceCm(RightirLedPin, RightirReceiverPin);
}

void printSensors(const char* tag) {
  Serial.print("F:");  Serial.print(dF);
  Serial.print(" L:"); Serial.print(dL);
  Serial.print(" R:"); Serial.print(dR);
  Serial.print("  ");  Serial.println(tag);
}

// red LED on = that sensor sees a wall (handy for debugging on the maze)
void showWalls() {
  digitalWrite(LeftredLedPin,  dL < OPEN);
  digitalWrite(FrontredLedPin, dF <= FRONT_TURN_MAX);
  digitalWrite(RightredLedPin, dR < OPEN);
}

// ================= servo controls =================

void straight() {
  servoLeft.writeMicroseconds(LEFT_FWD_US);
  servoRight.writeMicroseconds(RIGHT_FWD_US);
}

void backward() {
  servoLeft.writeMicroseconds(RIGHT_FWD_US);
  servoRight.writeMicroseconds(LEFT_FWD_US);
}

void stop() {
  servoLeft.writeMicroseconds(STOP_US);
  servoRight.writeMicroseconds(STOP_US);
}

// gentle curve while still driving forward: slow down the wheel on the side we turn to
void veerLeft() {
  servoLeft.writeMicroseconds(LEFT_FWD_US - STEER);
  servoRight.writeMicroseconds(RIGHT_FWD_US);
}

void veerRight() {
  servoLeft.writeMicroseconds(LEFT_FWD_US);
  servoRight.writeMicroseconds(RIGHT_FWD_US + STEER);
}

// spin on the spot (same values as turnLeftXDeg / turnRightXDeg in main_controller)
void spinLeft() {
  servoLeft.writeMicroseconds(1455);
  servoRight.writeMicroseconds(1455);
}

void spinRight() {
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
}

// how long to spin for a given angle
long spinMs(int deg, bool right) {
  return (long)deg * (right ? RIGHT_SPIN_MS : LEFT_SPIN_MS) / 360;
}

// keep centred while driving, using whatever side walls we can see.
// err > 0 means we are too close to the LEFT wall.
void steer() {
  bool leftWall  = dL < OPEN;
  bool rightWall = dR < OPEN;
  int err = 0;

  if (leftWall && rightWall) err = dR - dL;        // centre between both walls
  else if (leftWall)         err = WALL_CM - dL;   // only left wall: hold WALL_CM from it
  else if (rightWall)        err = dR - WALL_CM;   // only right wall: hold WALL_CM from it

  if (err >= SIDE_DIFF)       veerRight();
  else if (err <= -SIDE_DIFF) veerLeft();
  else                        straight();
}

// blocking moves (same as main_controller)
void forwardMs(int ms)  { straight(); delay(ms); stop(); }
void backwardMs(int ms) { backward(); delay(ms); stop(); }

void turnRightXDeg(int deg) { spinRight(); delay(spinMs(deg, true));  stop(); }
void turnLeftXDeg(int deg)  { spinLeft();  delay(spinMs(deg, false)); stop(); }
