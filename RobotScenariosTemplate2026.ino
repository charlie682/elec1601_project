#include <Servo.h>

// ===================== PINS =====================
const int leftIrLedPin = 10;
const int leftReceiverPin = 11;
const int leftLedPin = A2;

const int midIrLedPin = 6;
const int midReceiverPin = 7;
const int midLedPin = A1;

const int rightIrLedPin = 2;
const int rightReceiverPin = 3;
const int rightLedPin = A0;

const int servoLeftPin = 13;
const int servoRightPin = 12;

Servo servoLeft;
Servo servoRight;

// ===================== IR CALIBRATION (lab robot) =====================
const int N = 5;
const long CAL_FREQ_FRONT[N] = {38500, 40000, 41000, 42000, 45750};
const int  CAL_DIST_FRONT[N] = {10, 8, 6, 5, 3};   // 1.5cm -> 46000

const long CAL_FREQ_LEFT[N] = {38500, 39000, 40750, 41500, 45250};
const int  CAL_DIST_LEFT[N] = {10, 8, 6, 5, 3};    // 1.5cm -> 48750

const long CAL_FREQ_RIGHT[N] = {37750, 39250, 40500, 41500, 44500};
const int  CAL_DIST_RIGHT[N] = {10, 8, 6, 5, 3};   // 1.5cm -> 48500

const float FAR_DIST = 99;       // returned when nothing is detected at all

// Frequency scan range for measuring distance (high -> low)
const long SCAN_TOP = 46000;
const long SCAN_BOTTOM = 37500;
const long SCAN_STEP = 250;

// Quick single-frequency "too close" checks used while driving (~5cm points)
const long TOO_CLOSE_FREQ_LEFT = 41500;
const long TOO_CLOSE_FREQ_RIGHT = 41500;

// ===================== SCENARIO THRESHOLDS (cm) - TUNE IN THE LAB =====================
const float OPEN_DIST = 10.0;        // reading at/above this = no wall seen
const float FRONT_CLEAR = 10.0;      // front at/above this = clear ahead
const float CENTRE_MIN = 4.5;        // side band that counts as "centred"
const float CENTRE_MAX = 7.5;
const float CENTRE_DIFF = 1.5;       // left and right must agree within this
const float CLOSE_SIDE = 4.5;        // closer than this = hugging that wall
const float FRONT_TURN_MIN = 4.0;    // front wall range for a turn position
const float FRONT_TURN_MAX = 8.0;
const float PARALLEL_ASYM = 2.5;     // scenario 5/6: far side must be this much farther
const float ANGLED_ASYM = 4.0;       // scenario 7/8: far side must be this much farther
const float DEADEND_DIFF = 3.0;      // scenario 4: left/right must agree within this

// ===================== TIMING / SPEEDS =====================
const int DISPLAY_MS = 5000;         // LED hold. Template says 1s, the brief says 5s+
const int stopSpeed = 1490;
const int baseLeft = 1600;           // forward (verified)
const int baseRight = 1375;          // forward (verified)
const int backLeft = 1380;           // mirror of forward around 1490 - TUNE
const int backRight = 1605;          // mirror of forward around 1490 - TUNE
const int correction = 40;           // extra speed on one wheel to steer away from a wall
const long MS_PER_360 = 5175;        // time for a full spin - TUNE
const int MS_PER_CM = 66;            // 330ms ~ 5cm

// Re-centring (scenario 5/6)
const int SHIFT_DEG = 20;
const unsigned long SHIFT_MAX_MS = 1200;
const float TARGET_LEFT_CM = 5.5;
const float TARGET_RIGHT_CM = 6.0;

// Straightening (scenario 7/8)
const int ANGLED_FIRST_TURN_DEG = 25;
const int ANGLED_STEP_DEG = 5;

// Set true to only print what the robot sees and which scenario it picks,
// without moving. Handy for checking all 9 positions quickly.
const bool DRY_RUN = false;

int stopped = 0;

// ===================== IR MEASUREMENT =====================
int irDetect(int irLedPin, int irReceiverPin, long frequency) {
  tone(irLedPin, frequency);
  delay(1);
  int ir = digitalRead(irReceiverPin);   // 0 = detect, 1 = no detect
  noTone(irLedPin);
  delay(1);
  return ir;
}

// Scan from high to low; the first frequency that detects twice in a row
// is the edge. The second hit stops a single stray hit giving a false reading.
long findEdgeFreq(int ledPin, int rxPin) {
  for (long f = SCAN_TOP; f >= SCAN_BOTTOM; f -= SCAN_STEP) {
    if (irDetect(ledPin, rxPin, f) == 0 && irDetect(ledPin, rxPin, f - SCAN_STEP) == 0) {
      return f;
    }
  }
  return 0;   // nothing detected
}

// Convert an edge frequency to cm by interpolating between calibration points
float freqToCm(long f, const long calFreq[], const int calDist[]) {
  if (f == 0) return FAR_DIST;
  if (f <= calFreq[0]) return calDist[0];
  if (f >= calFreq[N - 1]) return calDist[N - 1];

  for (int i = 0; i < N - 1; i++) {
    if (f <= calFreq[i + 1]) {
      float t = (float)(f - calFreq[i]) / (float)(calFreq[i + 1] - calFreq[i]);
      return calDist[i] + t * (calDist[i + 1] - calDist[i]);
    }
  }
  return FAR_DIST;
}

float leftDistanceCm()  { return freqToCm(findEdgeFreq(leftIrLedPin,  leftReceiverPin),  CAL_FREQ_LEFT,  CAL_DIST_LEFT); }
float rightDistanceCm() { return freqToCm(findEdgeFreq(rightIrLedPin, rightReceiverPin), CAL_FREQ_RIGHT, CAL_DIST_RIGHT); }
float frontDistanceCm() { return freqToCm(findEdgeFreq(midIrLedPin,   midReceiverPin),   CAL_FREQ_FRONT, CAL_DIST_FRONT); }

// ===================== LEDS =====================
void showLEDs(bool l, bool m, bool r) {
  digitalWrite(leftLedPin, l);    // true = HIGH, false = LOW
  digitalWrite(midLedPin, m);
  digitalWrite(rightLedPin, r);
}

void flashRightLed(int times) {   // 1 second on, 1 second off
  for (int i = 0; i < times; i++) {
    digitalWrite(rightLedPin, HIGH);
    delay(1000);
    digitalWrite(rightLedPin, LOW);
    delay(1000);
  }
}

// ===================== MOVEMENT =====================
void stop() {
  servoLeft.writeMicroseconds(stopSpeed);
  servoRight.writeMicroseconds(stopSpeed);
}

void straight() {
  servoLeft.writeMicroseconds(baseLeft);
  servoRight.writeMicroseconds(baseRight);
}

void backward() {
  servoLeft.writeMicroseconds(backLeft);
  servoRight.writeMicroseconds(backRight);
}

void turnClockwise(int deg) {
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
  delay((long)deg * MS_PER_360 / 360);
  stop();
}

void turnAntiClockwise(int deg) {
  servoLeft.writeMicroseconds(1455);
  servoRight.writeMicroseconds(1455);
  delay((long)deg * MS_PER_360 / 360);
  stop();
}

void backwardFor(unsigned long ms) {
  backward();
  delay(ms);
  stop();
}

// Drive forward for ms milliseconds. If one side gets too close, speed up
// that side's wheel so the robot curves away from the wall.
void driveForwardCorrected(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    bool tooCloseLeft  = (irDetect(leftIrLedPin,  leftReceiverPin,  TOO_CLOSE_FREQ_LEFT)  == 0);
    bool tooCloseRight = (irDetect(rightIrLedPin, rightReceiverPin, TOO_CLOSE_FREQ_RIGHT) == 0);

    int l = baseLeft;
    int r = baseRight;
    if (tooCloseLeft) {
      l = baseLeft + correction;      // left wheel faster -> curve right, away from left wall
    } else if (tooCloseRight) {
      r = baseRight - correction;     // right wheel faster -> curve left, away from right wall
    }

    servoLeft.writeMicroseconds(l);
    servoRight.writeMicroseconds(r);
    delay(10);
  }
  stop();
}

// ===================== SCENARIO DETECTION =====================
bool isCentred(float l, float r) {
  return (l >= CENTRE_MIN && l <= CENTRE_MAX) &&
         (r >= CENTRE_MIN && r <= CENTRE_MAX) &&
         (fabs(l - r) <= CENTRE_DIFF);
}

bool isScenario1(float l, float r, float f) {   // middle of corridor
  return isCentred(l, r) && (f >= FRONT_CLEAR);
}

bool isScenario2(float l, float r, float f) {   // right turn: right open, left wall, front wall
  return (r >= OPEN_DIST) &&
         (l >= CENTRE_MIN && l <= CENTRE_MAX) &&
         (f >= FRONT_TURN_MIN && f <= FRONT_TURN_MAX);
}

bool isScenario3(float l, float r, float f) {   // left turn: left open, right wall, front wall
  return (l >= OPEN_DIST) &&
         (r >= CENTRE_MIN && r <= CENTRE_MAX) &&
         (f >= FRONT_TURN_MIN && f <= FRONT_TURN_MAX);
}

bool isScenario4(float l, float r, float f) {   // dead end: all three see a wall, roughly centred
  return (l < OPEN_DIST) && (r < OPEN_DIST) &&
         (f <= FRONT_TURN_MAX) &&
         (fabs(l - r) <= DEADEND_DIFF);
}

bool isScenario5(float l, float r, float f) {   // close to left wall, parallel
  return (l < CLOSE_SIDE) && (r - l >= PARALLEL_ASYM) && (f >= FRONT_CLEAR);
}

bool isScenario6(float l, float r, float f) {   // close to right wall, parallel
  return (r < CLOSE_SIDE) && (l - r >= PARALLEL_ASYM) && (f >= FRONT_CLEAR);
}

bool isScenario7(float l, float r, float f) {   // close to left wall, angled at it
  return (l < CLOSE_SIDE) && (r - l >= ANGLED_ASYM) && (f < FRONT_CLEAR);
}

bool isScenario8(float l, float r, float f) {   // close to right wall, angled at it
  return (r < CLOSE_SIDE) && (l - r >= ANGLED_ASYM) && (f < FRONT_CLEAR);
}

// ===================== SCENARIO ACTIONS =====================

// Drive forward until both sides read "centred" (or time runs out / wall ahead)
void forwardUntilCentred(unsigned long maxMs) {
  unsigned long start = millis();
  straight();
  while (millis() - start < maxMs) {
    float l = leftDistanceCm();
    float r = rightDistanceCm();
    if (isCentred(l, r)) break;
    if (frontDistanceCm() < 3.5) break;   // safety: don't drive into a wall
  }
  stop();
}

// Scenario 5: turn away from the left wall, drive until further from it,
// turn back parallel, then reverse to roughly the starting position.
void recentreAwayFromLeft() {
  turnClockwise(SHIFT_DEG);
  unsigned long t0 = millis();
  straight();
  while (millis() - t0 < SHIFT_MAX_MS) {
    if (leftDistanceCm() >= TARGET_LEFT_CM) break;
  }
  stop();
  unsigned long driven = millis() - t0;
  turnAntiClockwise(SHIFT_DEG);
  backwardFor((unsigned long)(driven * cos(SHIFT_DEG * PI / 180.0)));
}

// Scenario 6: mirror of the above
void recentreAwayFromRight() {
  turnAntiClockwise(SHIFT_DEG);
  unsigned long t0 = millis();
  straight();
  while (millis() - t0 < SHIFT_MAX_MS) {
    if (rightDistanceCm() >= TARGET_RIGHT_CM) break;
  }
  stop();
  unsigned long driven = millis() - t0;
  turnClockwise(SHIFT_DEG);
  backwardFor((unsigned long)(driven * cos(SHIFT_DEG * PI / 180.0)));
}

// Scenario 7: pointing at the left wall -> rotate clockwise until the front sensor clears
void straightenFromLeft() {
  turnClockwise(ANGLED_FIRST_TURN_DEG);
  for (int i = 0; i < 4; i++) {
    if (frontDistanceCm() >= FRONT_CLEAR) break;
    turnClockwise(ANGLED_STEP_DEG);
  }
}

// Scenario 8: pointing at the right wall -> rotate anticlockwise until the front sensor clears
void straightenFromRight() {
  turnAntiClockwise(ANGLED_FIRST_TURN_DEG);
  for (int i = 0; i < 4; i++) {
    if (frontDistanceCm() >= FRONT_CLEAR) break;
    turnAntiClockwise(ANGLED_STEP_DEG);
  }
}

// ===================== SETUP / LOOP =====================
void setup() {
  Serial.begin(9600);

  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);
  stop();

  pinMode(leftLedPin, OUTPUT);
  pinMode(midLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  showLEDs(false, false, false);

  delay(1000);
}

void loop() {
  if (stopped) return;   // a scenario already finished

  // Measure left, middle, right distances
  float distL = leftDistanceCm();
  float distR = rightDistanceCm();
  float distF = frontDistanceCm();

  // Detect scenario (if tests overlap, the higher number wins)
  int scenario = 0;
  if (isScenario1(distL, distR, distF)) scenario = 1;
  if (isScenario2(distL, distR, distF)) scenario = 2;
  if (isScenario3(distL, distR, distF)) scenario = 3;
  if (isScenario4(distL, distR, distF)) scenario = 4;
  if (isScenario5(distL, distR, distF)) scenario = 5;
  if (isScenario6(distL, distR, distF)) scenario = 6;
  if (isScenario7(distL, distR, distF)) scenario = 7;
  if (isScenario8(distL, distR, distF)) scenario = 8;

  Serial.print("L="); Serial.print(distL);
  Serial.print(" R="); Serial.print(distR);
  Serial.print(" F="); Serial.print(distF);
  Serial.print(" -> scenario "); Serial.println(scenario);

  if (DRY_RUN) {
    delay(1500);
    return;   // report only, loop() measures again
  }

  switch (scenario) {

    case 0:   // unknown
      showLEDs(false, false, false);
      stop();
      stopped = 1;
      break;

    case 1:   // middle of corridor
      showLEDs(false, false, true);
      delay(DISPLAY_MS);
      driveForwardCorrected(5 * MS_PER_CM);   // ~5cm
      stopped = 1;
      break;

    case 2:   // ideal right turn
      showLEDs(false, true, false);
      delay(DISPLAY_MS);
      turnClockwise(90);
      forwardUntilCentred(2500);
      stopped = 1;
      break;

    case 3:   // ideal left turn
      showLEDs(false, true, true);
      delay(DISPLAY_MS);
      turnAntiClockwise(90);
      forwardUntilCentred(2500);
      stopped = 1;
      break;

    case 4:   // dead end
      showLEDs(true, false, false);
      delay(DISPLAY_MS);
      turnClockwise(180);
      driveForwardCorrected(6 * MS_PER_CM);   // at least 5cm
      stopped = 1;
      break;

    case 5:   // close to left wall, parallel
      showLEDs(true, false, true);
      delay(DISPLAY_MS);
      recentreAwayFromLeft();
      stopped = 1;
      break;

    case 6:   // close to right wall, parallel
      showLEDs(true, true, false);
      delay(DISPLAY_MS);
      recentreAwayFromRight();
      stopped = 1;
      break;

    case 7:   // close to left wall, angled at it
      showLEDs(true, true, true);
      delay(DISPLAY_MS);
      straightenFromLeft();
      stopped = 1;
      break;

    case 8:   // close to right wall, angled at it
      showLEDs(false, false, false);
      flashRightLed(3);   // ~6 seconds of flashing
      straightenFromRight();
      stopped = 1;
      break;

    default:
      showLEDs(false, false, false);
      stop();
      stopped = 1;
  }
}
