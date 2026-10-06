#include "utils.hpp"
#include "essential_modules.hpp"

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
  for (long f = 42000; f >= 38000; f -= 500)
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
  for (int i = 0; i < 4 - 1; i++) {
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

// LED controls 

void LEDSwitch(int irLedPin, long frequency, int state) {
    if (state) {
        tone(irLedPin, frequency); 
    } else {
        noTone(irLedPin);
    }
}

// Servo controls 

void turnRightXDeg(int deg){
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);

  long time = (long)deg * 5175 / 360;   // multiply before dividing
  delay(time);
  stop();
}

void turnLeftXDeg(int deg){
  servoLeft.writeMicroseconds(1450); // NEEDS CHANGING and fixing!
  servoRight.writeMicroseconds(1450);

  long time = (long)deg * 5175 / 360;   // multiply before dividing
  delay(time);
  stop();
}

void straight(){
  servoLeft.writeMicroseconds(1375);
  servoRight.writeMicroseconds(1600);
}

void backward() {
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1375);
}

void stop(){
  Serial.println("stop");
  servoLeft.writeMicroseconds(1490);
  servoRight.writeMicroseconds(1490);
}

void turnLeft45Deg(){
  Serial.println("Left45");
  servoLeft.writeMicroseconds(1460);
  servoRight.writeMicroseconds(1460);
  delay(1000);
  stop();
}

void turnRight45Deg(){
  Serial.println("Right45");
  servoLeft.writeMicroseconds(1520);
  servoRight.writeMicroseconds(1520);
  delay(1000);
  stop();
}

void turnLeft90Deg(){
  Serial.println("Left90");
  servoLeft.writeMicroseconds(1460);
  servoRight.writeMicroseconds(1460);
  delay(1350);
  stop();
}

void turnRight90Deg(){
  Serial.println("Right90");
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
  delay(1350);
  stop();
}

void turnLeft180Deg(){
  Serial.println("Left180");
  servoLeft.writeMicroseconds(1490);
  servoRight.writeMicroseconds(1490);
  delay(5000);
  stop();
}

void turnRight180Deg(){
  Serial.println("Right180");
  servoLeft.writeMicroseconds(1490);
  servoRight.writeMicroseconds(1490);
  delay(5000);
  stop();
}

