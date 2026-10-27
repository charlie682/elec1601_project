#include "utils.hpp"

int dF = FAR_DIST, dL = FAR_DIST, dR = FAR_DIST;


int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  noTone(irLedPin);                          // Turn off the IR LED
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}


// one reading of all three sensors into dF, dL, dR (same calls as main_controller's loop)
void readSensors() {
  dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
  dL = irDistanceCm(LeftirLedPin,  LeftirReceiverPin);
  dR = irDistanceCm(RightirLedPin, RightirReceiverPin);
}


void displayLEDwall() {
  digitalWrite(LeftredLedPin,  dL < OPEN);
  digitalWrite(FrontredLedPin, dF <= FRONT_TURN_MAX);
  digitalWrite(RightredLedPin, dR < OPEN);
}


void straight() {
  servoLeft.writeMicroseconds(STANDARD_LEFT_FORWARD);
  servoRight.writeMicroseconds(STANDARD_RIGHT_FORWARD);
}

void backward() {
  servoLeft.writeMicroseconds(STANDARD_RIGHT_FORWARD);
  servoRight.writeMicroseconds(STANDARD_LEFT_FORWARD);
}

void stop() {
  servoLeft.writeMicroseconds(STOP_US);
  servoRight.writeMicroseconds(STOP_US);
}


void veerLeft() {
  servoLeft.writeMicroseconds(STANDARD_LEFT_FORWARD - STEER);
  servoRight.writeMicroseconds(STANDARD_RIGHT_FORWARD);
}

void veerRight() {
  servoLeft.writeMicroseconds(STANDARD_LEFT_FORWARD);
  servoRight.writeMicroseconds(STANDARD_RIGHT_FORWARD + STEER);
}


long spinMs(int deg, bool right) {
  return (long)deg * (right ? RIGHT_SPIN_MS : LEFT_SPIN_MS) / 360;
}


void steer() {
  bool leftWall  = dL < OPEN;
  bool rightWall = dR < OPEN;
  int err = 0;

  if (leftWall && rightWall) err = dR - dL;       
  else if (leftWall)         err = WALL_CM - dL;   
  else if (rightWall)        err = dR - WALL_CM;  

  if (err >= SIDE_DIFFERENCE) veerRight(); // closer to left wall, so turn slightly right
  else if (err <= -SIDE_DIFFERENCE) veerLeft(); // closer to right wall, so turn slightly left 
  else straight(); // else go straight
}


void forwardMs(int ms)  { straight(); delay(ms); stop(); }
void backwardMs(int ms) { backward(); delay(ms); stop(); }

void turnRightXDeg(int deg) { rotateRight(); delay(spinMs(deg, true));  stop(); }
void turnLeftXDeg(int deg)  { rotateLeft();  delay(spinMs(deg, false)); stop(); }




void rotateLeft() {
  servoLeft.writeMicroseconds(1455);
  servoRight.writeMicroseconds(1455);
}

void rotateRight() {
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
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
