#include <Servo.h>

const int FrontirLedPin=6, FrontirReceiverPin=7;   // Select these to match the IR LED/receiver pair that you are using
const int FrontredLedPin = A1;                    // Select this to match the red LED next to the IR receiver you are using
const int LeftirLedPin=10, LeftirReceiverPin=11;   // Select these to match the IR LED/receiver pair that you are using
const int LeftredLedPin = A2;      
const int RightirLedPin=2, RightirReceiverPin=3;   // Select these to match the IR LED/receiver pair that you are using
const int RightredLedPin = A0;


Servo servoLeft; 
Servo servoRight; 

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  noTone(irLedPin);                          // Turn off the IR LED
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}

// IR distance measurement function

int irDistance(int irLedPin, int irReceiverPin)
{
   int distance = 0;
   for(long f = 38000; f <= 42000; f += 1000)
   {
      distance += irDetect(irLedPin, irReceiverPin, f);
   }
   return distance;
}

void turnRight360Deg(int deg){
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);

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

int sensorLeft;
int sensorRight;
int sensorFront; 

void loop()
{
  //2/5*1023 = 409
  // sensorLeft = irDetect(LeftirLedPin, LeftirReceiverPin, 41500); 
  // sensorRight = irDetect(RightirLedPin, RightirReceiverPin, 41700);
  // sensorFront = irDetect(FrontirLedPin, FrontirReceiverPin, 40200); // detects the wall from 8 cm
  
  // Serial.println(sensorLeft);
  // Serial.println(sensorRight);
  // Serial.println(sensorFront);

  
  // if (!sensorLeft) {
    
  //   //turn left slowly
  //   //turnRight90Deg();
  //   stop();
  // 	delay(3000);
    
  // } else if (!sensorRight) {
    
  //   //turn right slowly
  // 	stop();
  // 	delay(3000);

  // } else if (!sensorFront) {
  //   stop();
  //   // turnLeft90Deg();
  //   // turnLeft90Deg();
  //   delay(3000);
  // }
  
  // //continue at medium speed
  // servoLeft.writeMicroseconds(1600);
  // servoRight.writeMicroseconds(1375);

  stop();


  for (int freq = 38000; freq < 43000; freq += 500) {

    sensorLeft = irDetect(LeftirLedPin, LeftirReceiverPin, freq); 
    sensorRight = irDetect(RightirLedPin, RightirReceiverPin, freq);
    sensorFront = irDetect(FrontirLedPin, FrontirReceiverPin, freq); 
    
    // prints out current frequency checking for each sensor
    Serial.print("Frequency: ");
    Serial.println(freq);
    Serial.println(sensorLeft);
    Serial.println(sensorRight);
    Serial.println(sensorFront);

    delay(10000); //10 second delay to measure distance

    
  }

}


