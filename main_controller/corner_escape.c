#include "utils.h"
#include "essential_modules.h"

// Corner start scenario: detect a corner, square up to the front wall,
// reverse a little, then run the existing ideal turn.
// Uses team helpers: irDistanceCm, turnLeftXDeg, turnRightXDeg, backward,
// stop, idealRightTurn, idealLeftTurn.

// ---------- Tuning ----------
const int  CE_MISSES_TO_STOP = 6;
const long CE_FREQ_START = 30000, CE_FREQ_END = 56000, CE_FREQ_STEP = 250;
const int  CE_SWEEPS    = 3;      // sweeps averaged per measurement
const long CE_TOL_HZ    = 250;    // smaller change = "no change"
const int  CE_MAX_STEPS = 25;     // safety limit

const int  CE_FRONT_CLOSE = 5;    // cm
const int  CE_SIDE_CLOSE  = 8;    // cm
const int  CE_SIDE_OPEN   = 10;   // cm

const int  CE_STEP_DEG  = 3;      // size of each tiny alignment rotation
const int  CE_REV_MS    = 250;    // how long to back up (tune)

// ---------- Front-sensor edge frequency ----------
long ceSweepFront() {
    long edge = -1;
    int hit = 0, done = 0, miss = 0;

    for (long f = CE_FREQ_START; f <= CE_FREQ_END && !done; f += CE_FREQ_STEP) {
        tone(FrontirLedPin, f);
        delay(1);
        int r = digitalRead(FrontirReceiverPin);   // 0 = detect
        noTone(FrontirLedPin);
        delay(1);

        if (r == 0) { edge = f; hit = 1; miss = 0; }
        else if (hit && ++miss >= CE_MISSES_TO_STOP) done = 1;
    }
    return edge;
}

long ceFrontEdge() {
    long sum = 0;
    int n = 0;
    for (int i = 0; i < CE_SWEEPS; i++) {
        long e = ceSweepFront();
        if (e >= 0) { sum += e; n++; }
    }
    return n ? sum / n : -1;
}

// ---------- Movement (team helpers) ----------
// dir +1 = turn right, -1 = turn left
void ceRotate(int dir) {
    if (dir > 0) turnRightXDeg(CE_STEP_DEG);
    else         turnLeftXDeg(CE_STEP_DEG);
    stop();
    delay(300);                    // settle before measuring
}

void ceReverse() {
    backward();
    delay(CE_REV_MS);
    stop();
    delay(300);
}

// ---------- Corner detection ----------
// 1 = front+left corner, 2 = front+right corner, 0 = neither
int ceDetectCorner() {
    int dF = irDistanceCm(FrontirLedPin, FrontirReceiverPin);
    int dL = irDistanceCm(LeftirLedPin,  LeftirReceiverPin);
    int dR = irDistanceCm(RightirLedPin, RightirReceiverPin);

    Serial.print("dF="); Serial.print(dF);
    Serial.print(" dL="); Serial.print(dL);
    Serial.print(" dR="); Serial.println(dR);

    if (dF < CE_FRONT_CLOSE && dL < CE_SIDE_CLOSE && dR > CE_SIDE_OPEN) return 1;
    if (dF < CE_FRONT_CLOSE && dR < CE_SIDE_CLOSE && dL > CE_SIDE_OPEN) return 2;
    return 0;
}

// ---------- Square up to the front wall ----------
// edge up = wall closer = right way; edge down = reverse / passed the peak
void ceAlignToFrontWall() {
    long prev = ceFrontEdge();
    int dir = +1;
    int improved = 0, reversed = 0;

    for (int s = 0; s < CE_MAX_STEPS; s++) {
        ceRotate(dir);
        long now = ceFrontEdge();

        Serial.print("align "); Serial.print(prev);
        Serial.print(" -> "); Serial.println(now);

        if (now > prev + CE_TOL_HZ) {
            improved = 1;
        } else if (now < prev - CE_TOL_HZ) {
            if (improved) { ceRotate(-dir); return; }   // passed the peak
            if (reversed) return;                       // already at the peak
            dir = -dir;
            reversed = 1;
        } else if (improved) {
            return;                                     // plateau
        }
        prev = now;
    }
}

// ---------- Entry point: call once from setup() ----------
// returns 1 if a corner was handled, 0 otherwise
int escapeCornerIfNeeded() {
    int corner = ceDetectCorner();
    if (corner == 0) return 0;

    ceAlignToFrontWall();
    ceReverse();

    if (corner == 1) idealRightTurn();
    else             idealLeftTurn();
    return 1;
}