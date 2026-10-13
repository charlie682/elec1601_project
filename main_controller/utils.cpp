#include "utils.hpp"
#include "essential_modules.hpp"


void displayCode(LedMode r, LedMode m, LedMode l) {
  stop();                                   // wheels stopped before the pause
  for (int i = 0; i < 6; i++) {             // 6 s total; flashing LEDs toggle every 1 s
    bool phase = (i % 2 == 0);
    digitalWrite(RightredLedPin, r == LED_ON || (r == LED_FLASH && phase));
    digitalWrite(FrontredLedPin, m == LED_ON || (m == LED_FLASH && phase));
    digitalWrite(LeftredLedPin,  l == LED_ON || (l == LED_FLASH && phase));
    delay(1000);
  }
}


void forwardMs(int ms)  { straight(); delay(ms); stop(); }
void backwardMs(int ms) { backward(); delay(ms); stop(); }

// IR distance  
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

  if (maxF == 0) return FAR_DIST; // no detecton

  // boundary conditiosn on our table
  if (irReceiverPin == FrontirReceiverPin) {
    if (maxF <= CAL_FREQ_FRONT[0])         return CAL_DIST_FRONT[0];
    if (maxF >= CAL_FREQ_FRONT[N - 1]) return CAL_DIST_FRONT[N - 1];
  } else if (irReceiverPin == LeftirReceiverPin) {
    if (maxF <= CAL_FREQ_LEFT[0])         return CAL_DIST_LEFT[0];
    if (maxF >= CAL_FREQ_LEFT[N - 1]) return CAL_DIST_LEFT[N - 1];
  } else if (irReceiverPin == RightirReceiverPin) {
    if (maxF <= CAL_FREQ_RIGHT[0])         return CAL_DIST_RIGHT[0];
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

// check if angled
bool angledInto(int irLed, int irRx) {
  int before = irDistanceCm(irLed, irRx);
  backward(); delay(PROBE_MS); stop();
  int after = irDistanceCm(irLed, irRx);
  straight(); delay(PROBE_MS); stop();  // drive back to where we were

  Serial.print("probe: "); Serial.print(before);
  Serial.print(" -> ");    Serial.println(after);
  return (after - before) >= PROBE_CHANGE;
}

// LED controls 

void LEDSwitch(int irLedPin, long frequency, int state) {
    if (state) {
        digitalWrite(irLedPin, HIGH); 
    } else {
        digitalWrite(irLedPin, LOW);
    }
}

// Servo controls 

void turnRightXDeg(int deg){
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);

  long time = (long)deg * 5100 / 360;   // multiply before dividing
  delay(time);
  stop();
}

void turnLeftXDeg(int deg){
  servoLeft.writeMicroseconds(1450); // NEEDS CHANGING and fixing!
  servoRight.writeMicroseconds(1450);

  long time = (long)deg * 4800 / 360;   // multiply before dividing
  delay(time);
  stop();
}

void straight(){
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1375);
}

void backward() {
  servoLeft.writeMicroseconds(1375);
  servoRight.writeMicroseconds(1600);
}

void stop(){
  servoLeft.writeMicroseconds(1490);
  servoRight.writeMicroseconds(1490);
}

void turnLeft45Deg(){
  servoLeft.writeMicroseconds(1460);
  servoRight.writeMicroseconds(1460);
  delay(1000);
  stop();
}

void turnRight45Deg(){
  servoLeft.writeMicroseconds(1520);
  servoRight.writeMicroseconds(1520);
  delay(1000);
  stop();
}

void turnLeft90Deg(){
  servoLeft.writeMicroseconds(1460);
  servoRight.writeMicroseconds(1460);
  delay(1350);
  stop();
}

void turnRight90Deg(){

  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
  delay(1350);
  stop();
}

void turnLeft180Deg(){

  servoLeft.writeMicroseconds(1490);
  servoRight.writeMicroseconds(1490);
  delay(5000);
  stop();
}

void turnRight180Deg(){

  servoLeft.writeMicroseconds(1490);
  servoRight.writeMicroseconds(1490);
  delay(5000);
  stop();
}

