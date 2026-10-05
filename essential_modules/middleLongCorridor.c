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

const long leftWallFreq = 44000;  //5.5cm from left == frequency of 41500
const long rightWallFreq = 41900; //6cm from right == frequency of 41700
const long frontWallFreq = 40250;

// servo speeds
const int stopSpeed = 1490;
const int baseLeft = 1600;
const int baseRight = 1375;
const int correction = 40;


void stop() {
  servoLeft.writeMicroseconds(stopSpeed);
  servoRight.writeMicroseconds(stopSpeed);
}


void setup() {
  Serial.begin(9600);
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);

  stop();

  // LEDs
  pinMode(leftLedPin, OUTPUT);
  pinMode(midLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);

  // Start with LEDs off
  digitalWrite(leftLedPin, LOW);
  digitalWrite(midLedPin, LOW);
  digitalWrite(rightLedPin, LOW);

  delay(1000);
}


void loop() {
  // 1. Check all three sensors
  bool wallOnLeft = (irDetect(leftIrLedPin, leftReceiverPin, leftWallFreq) == 0);
  bool wallOnRight = (irDetect(rightIrLedPin, rightReceiverPin, rightWallFreq) == 0);
  bool wallInFront = (irDetect(midIrLedPin, midReceiverPin, frontWallFreq) == 0);
  
  // 2. Check if this is Scenario 1
  bool scenario1 = wallOnLeft && wallOnRight && !wallInFront;
  
  // 3. Scenario 1 detected
  if (scenario1) {

    // Scenario 1 LED
    // Right = ON
    // Middle = OFF
    // Left = OFF

    digitalWrite(rightLedPin, HIGH);
    digitalWrite(midLedPin, LOW);
    digitalWrite(leftLedPin, LOW);

    int leftSpeed = baseLeft;
    int rightSpeed = baseRight;

    // 4. Left wall detected
    if (wallOnLeft) {
      leftSpeed = baseLeft + correction;
    }

    // 5. Right wall detected
    if (wallOnRight) {
      rightSpeed = baseRight - correction;
    }

    // 6. Drive
    servoLeft.writeMicroseconds(leftSpeed);
    servoRight.writeMicroseconds(rightSpeed);
  }

  // 7. Scenario 1 is not detected
  else {

    stop();

    digitalWrite(rightLedPin, LOW);
    digitalWrite(midLedPin, LOW);
    digitalWrite(leftLedPin, LOW);

    Serial.println("Scenario 1 not detected - stopped");
  }

  // Check sensors again after 20 ms
  delay(20);
}
