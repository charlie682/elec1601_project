#include <Servo.h>

// ir sensor pins
const int leftIrLedPin = 10;
const int leftReceiverPin = 11;
const int leftLedPin = A2;

const int midIrLedPin = 6;
const int midReceiverPin = 7;
const int midLedPin = A1;

const int rightIrLedPin = 2;
const int rightReceiverPin = 3;
const int rightLedPin = A0;

// servo pins
const int servoLeftPin = 13;
const int servoRightPin = 12;

Servo servoLeft;
Servo servoRight;

int irDetect(int irLedPin, int irReceiverPin, long frequency) {
  tone(irLedPin, frequency);
  delay(1);
  int ir = digitalRead(irReceiverPin);
  noTone(irLedPin);
  delay(1);
  return ir;
}

// Home values for now — not used by the movement tests below,
// kept here so you don't lose them when you merge this back in later.
const long leftWallFreq = 44000;
const long leftCloseFreq = 47000;
const long rightWallFreq = 41900;
const long frontWallFreq = 40250;

const int stopSpeed = 1490;
const int baseRight = 1375;   // TODO: verify direction below — may need swapping with baseRight
const int baseLeft = 1600;  // TODO: verify direction below — may need swapping with baseLeft

void stop() {
  servoLeft.writeMicroseconds(stopSpeed);
  servoRight.writeMicroseconds(stopSpeed);
}

void straight() {
  servoLeft.writeMicroseconds(baseLeft);
  servoRight.writeMicroseconds(baseRight);
}

void backward() {
  servoLeft.writeMicroseconds(baseRight);
  servoRight.writeMicroseconds(baseLeft);
}

void turnClockwise(int deg) {
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
  long time = (long)deg * 5175 / 360;
  delay(time);
  stop();
}

void turnAntiClockwise(int deg) {
  servoLeft.writeMicroseconds(1455);
  servoRight.writeMicroseconds(1455);
  long time = (long)deg * 5175 / 360;
  delay(time);
  stop();
}

// Thin named wrappers matching how the scenario specs phrase things —
// purely for readability, same underlying functions.
void turnRight90()  { turnClockwise(90); }
void turnLeft90()   { turnAntiClockwise(90); }
void turnRight180() { turnClockwise(180); }
void turnLeft180()  { turnAntiClockwise(180); }

void setup() {
  Serial.begin(9600);
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);

  stop();

  pinMode(leftLedPin, OUTPUT);
  pinMode(midLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  digitalWrite(leftLedPin, LOW);
  digitalWrite(midLedPin, LOW);
  digitalWrite(rightLedPin, LOW);

  delay(1000);

  // ===== TEST SEQUENCE =====
  // Give yourself room on the table/floor before uploading — the robot
  // WILL physically move during this. Watch each step before the next
  // one fires; there's a 3s gap between each.

  Serial.println("TEST: straight() for 500ms");
  straight();
  delay(3000);
  stop();
  delay(3000);

  // Serial.println("TEST: backward() for 500ms");
  // delay(1000);
  // backward();
  // delay(3000);
  // stop();
  // delay(3000);

  // Serial.println("TEST: turnRight90()");
  // turnRight90();
  // delay(3000);

  // Serial.println("TEST: turnLeft90()");
  // turnLeft90();
  // delay(3000);

  // Serial.println("TEST: turnRight180()");
  // turnRight180();
  // delay(3000);

  // Serial.println("TEST: turnLeft180()");
  // turnLeft180();
  // delay(3000);

  Serial.println("TEST SEQUENCE COMPLETE");
}

void loop() {
  // nothing — test runs once in setup()
}
