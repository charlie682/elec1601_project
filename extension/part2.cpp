#include "utils.hpp"
#include "part2.hpp"

const long OPENING_MS = 600; // drive into the middle of a gap before turning
const int  FINISH_CM  = 5;
const unsigned long SETTLE_MS = 150; // stand still before spinning, like part 1 did

enum State {STRAIGHT, PRE_TURN, TURNING, FINISH, DONE};

// initial values!
State state = STRAIGHT;
unsigned long stateStart  = 0;
unsigned long timeSinceSideWallOpening = 0;
bool seenRightWall = false;
int timeMeasuredOpen  = 0;
unsigned long turnTime = 0; // how long the current spin should last
bool turnRight = true; // which way the current spin goes


static void setState(State s) {
    state = s;
    stateStart = millis();
}

static void turn(bool right, int deg) {
    stop();
    turnRight = right;
    turnTime = spinMs(deg, right);
    setState(TURNING);
}

const unsigned long LONG_MIN_MS = 2000; //halfway between long/short 
const unsigned long LONG_MAX_MS = 6000; 

static bool isLongDeadEnd() {
    unsigned long ms = millis() - timeSinceSideWallOpening;  
    return ms > LONG_MIN_MS && ms < LONG_MAX_MS;
}

void part2Begin() {
    timeSinceSideWallOpening = millis();
    straight();
    setState(STRAIGHT);
}

// each step we need to know what the previous state was, hence state machine 
void part2Step() {

   if (state == TURNING) {
        unsigned long t = millis() - stateStart;
        if (t < SETTLE_MS) return; 
        if (t < SETTLE_MS + turnTime) {  
            if (turnRight) rotateRight();
            else           rotateLeft();
            return;
        }
        seenRightWall = false; 
        timeMeasuredOpen = 0;
        timeSinceSideWallOpening = millis();
        straight();
        setState(STRAIGHT);
        return;
    }

    if (state == DONE) { 
        bool on = (millis() / 500) % 2;
        digitalWrite(LeftredLedPin, on);
        digitalWrite(FrontredLedPin, on);
        digitalWrite(RightredLedPin, on);
        return;
    }

    readSensors();
    displayLEDwall();

    bool leftWall = dL < OPEN;
    bool rightWall = dR < OPEN;
    bool frontWall = dF <= FRONT_TURN_MAX;

    if (!leftWall || !rightWall) timeSinceSideWallOpening = millis(); //then there is a gap

    switch (state) {
        case STRAIGHT:
            if (rightWall) {seenRightWall = true; timeMeasuredOpen = 0;}
            else timeMeasuredOpen++;

            if (frontWall) {
                if (!rightWall)  turn(true, 90); //go right if open
                else if (!leftWall) turn(false, 90);  // go left if open
                else if (isLongDeadEnd()) setState(FINISH); // check if long corridor
                else turn(dR < dL, 180); // short dead end: 180 towards the closer wall, like part 1 task4
            }
            else if (seenRightWall && timeMeasuredOpen >= 2) setState(PRE_TURN); // get ready to turn
            else steer();
            break;

        case PRE_TURN: // get ready to turn
            if (frontWall || millis() - stateStart >= OPENING_MS) turn(true, 90);
            else steer();
            break;

        case FINISH: //found long corridor
            if (dF <= FINISH_CM) {stop(); setState(DONE); }
            else steer();
            break;
    }
}
