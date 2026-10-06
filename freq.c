const int FrontirLedPin=6, FrontirReceiverPin=7;
const int LeftirLedPin=10, LeftirReceiverPin=11;
const int RightirLedPin=2, RightirReceiverPin=3;

const int MISSES_TO_STOP = 6;   // tolerates a gap of up to ~1,500 Hz before deciding detection has really stopped

// index 0 = Left, 1 = Mid/Front, 2 = Right
long firstEdge[3];   // last detect before the FIRST miss
long lastEdge[3];    // last detect before MISSES_TO_STOP misses in a row (the "real" edge)
bool hitYet[3];
bool firstDone[3];
bool stopped[3];
int  missRun[3];

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency);
  delay(1);
  int ir = digitalRead(irReceiverPin);   // 0 = detect, 1 = no detect
  noTone(irLedPin);
  delay(1);
  return ir;
}

void setup()
{
  Serial.begin(9600);
  pinMode(LeftirReceiverPin, INPUT);
  pinMode(FrontirReceiverPin, INPUT);
  pinMode(RightirReceiverPin, INPUT);
}

void printEdge(const char *name, int i)
{
  Serial.print(name);
  Serial.print("=");
  if (lastEdge[i] < 0) {
    Serial.print("none");
  } else {
    Serial.print(firstEdge[i]);
    Serial.print("/");
    Serial.print(lastEdge[i]);
  }
  Serial.print("  ");
}

void loop()
{
  for (int i = 0; i < 3; i++) {
    firstEdge[i] = -1; lastEdge[i] = -1;
    hitYet[i] = false; firstDone[i] = false; stopped[i] = false; missRun[i] = 0;
  }

  // Lab robot's values sit lower than the take-home board's did,
  // so this range is narrower (faster sweep). Widen it if you get
  // "none" at a distance where you expect a real reading.
  for (long freq = 34000; freq <= 46000; freq += 250) {

    int reading[3];
    reading[0] = irDetect(LeftirLedPin,  LeftirReceiverPin,  freq);
    reading[1] = irDetect(FrontirLedPin, FrontirReceiverPin, freq);
    reading[2] = irDetect(RightirLedPin, RightirReceiverPin, freq);

    for (int i = 0; i < 3; i++) {
      if (reading[i] == 0) {
        if (!stopped[i]) {
          lastEdge[i] = freq;
          hitYet[i] = true;
          missRun[i] = 0;
          if (!firstDone[i]) firstEdge[i] = freq;
        }
      } else if (hitYet[i]) {
        firstDone[i] = true;
        missRun[i]++;
        if (missRun[i] >= MISSES_TO_STOP) stopped[i] = true;
      }
    }
  }

  Serial.print("Edge (first miss / real edge):  ");
  printEdge("L", 0);
  printEdge("M", 1);
  printEdge("R", 2);
  Serial.println();

  delay(3000);
}
