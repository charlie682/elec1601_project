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
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  noTone(irLedPin);                          // Turn off the IR LED
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}

// Home values for now
// Change these to LAB values tomorrow
const long leftWallFreq = 44000;      // about 5.5 cm
const long rightWallFreq = 41900;     // about 6 cm
const long rightCloseFreq = 47000;    // temporary
const long frontWallFreq = 40250;

const int stopSpeed = 1490;
const int baseLeft = 1375;
const int baseRight = 1600;

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

void setup() {
  Serial.begin(9600);
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);

  stop();

  // LED pins
  pinMode(leftLedPin, OUTPUT);
  pinMode(midLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);

  // LEDs OFF
  digitalWrite(leftLedPin, LOW);
  digitalWrite(midLedPin, LOW);
  digitalWrite(rightLedPin, LOW);

  delay(1000);
}

void loop() {

  // 1. Check all sensors
  bool wallOnLeft = (irDetect(leftIrLedPin, leftReceiverPin, leftWallFreq) == 0);
  bool wallOnRight = (irDetect(rightIrLedPin, rightReceiverPin, rightWallFreq) == 0);
  bool rightIsClose = (irDetect(rightIrLedPin, rightReceiverPin, rightCloseFreq) == 0);
  bool wallInFront = (irDetect(midIrLedPin, midReceiverPin, frontWallFreq) == 0);

  // 2. Check if this is Scenario 6
  bool scenario6 = wallOnLeft && wallOnRight && rightIsClose && !wallInFront;

  // 3. Scenario 6 detected
  if (scenario6) {

    // Scenario 6 LED
    // Right = OFF
    // Middle = ON
    // Left = ON

    digitalWrite(rightLedPin, LOW);
    digitalWrite(midLedPin, HIGH);
    digitalWrite(leftLedPin, HIGH);

    delay(5000);
    
    Serial.println("Scenario 6 detected");

    // 4. Rotate anti-clockwise
    turnAntiClockwise(10);

    // 5. Move forward
    straight();
    delay(330);
    stop();

    // 6. Rotate clockwise
    turnClockwise(10);

    // 7. Move backwards
    backward();
    delay(330);
    stop();

    // 8. Stop after completing Scenario 6
    stop();
  }

  // 9. Scenario 6 is not detected
  else {

    stop();

    digitalWrite(rightLedPin, LOW);
    digitalWrite(midLedPin, LOW);
    digitalWrite(leftLedPin, LOW);

    Serial.println("Scenario 6 not detected - stopped");

    delay(20);
  }
}
