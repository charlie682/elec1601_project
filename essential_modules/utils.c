
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

