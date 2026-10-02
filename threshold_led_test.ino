// Lights the red LED whenever that sensor detects a wall at FREQ.
// Use this to SEE detection happen live instead of reading Serial —
// handy for checking sensor alignment, or confirming a calibrated threshold.

const int LeftIrLedPin  = 10, LeftIrReceiverPin  = 11, LeftRedLedPin  = A2;
const int MidIrLedPin   = 6,  MidIrReceiverPin   = 7,  MidRedLedPin   = A1;
const int RightIrLedPin = 2,  RightIrReceiverPin = 3,  RightRedLedPin = A0;

// Change this to whichever frequency you want to test/watch.
const long FREQ = 52250;

int irDetect(int irLedPin, int irReceiverPin, long frequency) {
  tone(irLedPin, frequency);
  delay(1);
  int ir = digitalRead(irReceiverPin);   // 0 = detect, 1 = no detect
  noTone(irLedPin);
  delay(1);
  return ir;
}

void setup() {
  Serial.begin(9600);
  pinMode(LeftIrReceiverPin, INPUT);
  pinMode(MidIrReceiverPin, INPUT);
  pinMode(RightIrReceiverPin, INPUT);

  pinMode(LeftRedLedPin, OUTPUT);
  pinMode(MidRedLedPin, OUTPUT);
  pinMode(RightRedLedPin, OUTPUT);
}

void loop() {
  bool left  = irDetect(LeftIrLedPin,  LeftIrReceiverPin,  FREQ) == 0;
  bool mid   = irDetect(MidIrLedPin,   MidIrReceiverPin,   FREQ) == 0;
  bool right = irDetect(RightIrLedPin, RightIrReceiverPin, FREQ) == 0;

  digitalWrite(LeftRedLedPin,  left  ? HIGH : LOW);
  digitalWrite(MidRedLedPin,   mid   ? HIGH : LOW);
  digitalWrite(RightRedLedPin, right ? HIGH : LOW);

  // optional: still prints, so you can confirm what the LEDs are showing
  Serial.print("L="); Serial.print(left);
  Serial.print(" M="); Serial.print(mid);
  Serial.print(" R="); Serial.println(right);
}
