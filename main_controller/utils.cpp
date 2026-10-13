#include "utils.hpp"

// ---------- SENSING ----------
int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency);
  delay(1);
  int ir = digitalRead(irReceiverPin);   // 0 = detect, 1 = no detect
  noTone(irLedPin);
  delay(1);
  return ir;
}

// Sweep from high freq (close) to low freq (far). The first frequency that
// still "sees" the wall tells us how close it is, then we convert via the table.
int irDistanceCm(int irLedPin, int irReceiverPin)
{
  const long *freqs;
  const int  *dists;
  if (irReceiverPin == FrontirReceiverPin)     { freqs = CAL_FREQ_FRONT; dists = CAL_DIST_FRONT; }
  else if (irReceiverPin == LeftirReceiverPin) { freqs = CAL_FREQ_LEFT;  dists = CAL_DIST_LEFT; }
  else                                         { freqs = CAL_FREQ_RIGHT; dists = CAL_DIST_RIGHT; }

  long maxF = 0;
  for (long f = 49000; f >= 37000; f -= 500) {
    if (irDetect(irLedPin, irReceiverPin, f) == 0) { maxF = f; break; }
  }
  if (maxF == 0) return FAR_DIST;

  if (maxF <= freqs[0])     return dists[0];
  if (maxF >= freqs[N - 1]) return dists[N - 1];

  for (int i = 0; i < N - 1; i++) {
    if (maxF <= freqs[i + 1])
      return map(maxF, freqs[i], freqs[i + 1], dists[i], dists[i + 1]);
  }
  return FAR_DIST;
}

// ---------- INDICATOR LEDS ----------
// These drive the RED LEDs. (The old LEDSwitch drove the IR emitters, which
// showed nothing AND could blind the receivers.)
void setLeds(bool left, bool mid, bool right)
{
  digitalWrite(LeftredLedPin,  left  ? HIGH : LOW);
  digitalWrite(FrontredLedPin, mid   ? HIGH : LOW);
  digitalWrite(RightredLedPin, right ? HIGH : LOW);
}

void blinkLed(int pin, int times)
{
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH); delay(150);
    digitalWrite(pin, LOW);  delay(150);
  }
}

// ---------- MOVEMENT ----------
void stop() {
  servoLeft.writeMicroseconds(STOP_US);
  servoRight.writeMicroseconds(STOP_US);
}

void straight() {
  servoLeft.writeMicroseconds(FWD_LEFT_US);
  servoRight.writeMicroseconds(FWD_RIGHT_US);
}

void backward() {
  servoLeft.writeMicroseconds(FWD_RIGHT_US);   // mirror of straight()
  servoRight.writeMicroseconds(FWD_LEFT_US);
}

void driveMs(int ms)   { straight(); delay(ms); stop(); delay(SETTLE_MS); }
void reverseMs(int ms) { backward(); delay(ms); stop(); delay(SETTLE_MS); }

void turnRightXDeg(int deg) {
  servoLeft.writeMicroseconds(TURN_CW_US);
  servoRight.writeMicroseconds(TURN_CW_US);
  delay((long)deg * MS_PER_360 / 360);   // multiply before dividing
  stop();
  delay(SETTLE_MS);
}

void turnLeftXDeg(int deg) {
  servoLeft.writeMicroseconds(TURN_CCW_US);
  servoRight.writeMicroseconds(TURN_CCW_US);
  delay((long)deg * MS_PER_360 / 360);
  stop();
  delay(SETTLE_MS);
}