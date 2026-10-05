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

// sensor frequencies
// These are home values for now
// Change these to the LAB values tomorrow
const long leftWallFreq = 44000;  //5.5cm from left == frequency of 41500
const long rightWallFreq = 41900; //6cm from right == frequency of 41700
const long frontWallFreq = 40250;

// servo speeds
const int stopSpeed = 1490;

const int baseLeft = 1600;
const int baseRight = 1375;

const int correction = 40;


// Function for detecting the wall
// Returns:
// 0 = wall detected
// 1 = no wall detected
int checkSensor(int ledPin, int receiverPin, long frequency) {
  tone(ledPin, frequency);
  delay(1);

  int result = digitalRead(receiverPin);

  noTone(ledPin);
  delay(1);

  return result;
}


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
  bool wallOnLeft = (checkSensor(leftIrLedPin, leftReceiverPin, leftWallFreq) == 0);
  bool wallOnRight = (checkSensor(rightIrLedPin, rightReceiverPin, rightWallFreq) == 0);
  bool wallInFront = (checkSensor(midIrLedPin, midReceiverPin, frontWallFreq) == 0);
  
  // 2. Check if this is Scenario 1
  bool scenario1 = wallOnLeft && wallOnRight && !wallInFront;
  
  // 3. Scenario 1 detected
  if (scenario1) {

    // Scenario 1 LED
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
    // Turn away from the right wall
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
