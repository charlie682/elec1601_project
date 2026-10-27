#include "utils.hpp"
#include "part2.hpp"

const int  CELL_CM    = 20;
const long OPENING_MS = 600; // drive into the middle of a gap before turning
const int  TIMES_MEASURED_OPEN = 2; // right side must read open this many times in a row
const int  FINISH_CM  = 5;

enum State { FOLLOW, PRE_TURN, TURNING, FINISH, DONE };

// initial values!
State state = FOLLOW;
unsigned long stateStart  = 0;
unsigned long timeSinceSideWall = 0;
bool seenRightWall = false;
int timeMeasuredOpen  = 0;
unsigned long turnTime = 0; // how long the current spin should last


static void setState(State s) {
    state = s;
    stateStart = millis();
}

//spin ends when turntime is elapsed
static void turn(bool right, int deg) {
    turnTime = spinMs(deg, right);
    if (right) rotateRight();
    else rotateLeft();
    setState(TURNING);
}

const unsigned long LONG_MIN_MS = 1500; //halfway between long/short 
const unsigned long LONG_MAX_MS = 3000; //above long reading (test this in the lab!!!)

static bool isLongDeadEnd() {
    unsigned long ms = millis() - timeSinceSideWall;  
    Serial.print("dead end ms = "); Serial.println(ms); //for testing
    return ms > LONG_MIN_MS && ms < LONG_MAX_MS;
}

void part2Begin() {
    timeSinceSideWall = millis();
    straight();
    setState(FOLLOW);
}

// each step we need to know what the previous state was, hence state machine 
void part2Step() {

    if (state == TURNING) {
        if (millis() - stateStart >= turnTime) { 
            seenRightWall = false;
            timeMeasuredOpen = 0;
            timeSinceSideWall = millis();
            stop();
            straight(); 
            setState(FOLLOW);
        }
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

    if (!leftWall || !rightWall) timeSinceSideWall = millis(); //then there is a gap

    switch (state) {
        case FOLLOW:
            if (rightWall) {seenRightWall = true; timeMeasuredOpen = 0;}
            else timeMeasuredOpen++;

            if (frontWall) {
                if (!rightWall)  turn(true, 90); //go right if open
                else if (!leftWall) turn(false, 90);  // go left if open
                else if (isLongDeadEnd()) setState(FINISH); // check if long corridor
                else turn(true, 180); // short dead end so 180 away
            }
            else if (seenRightWall && timeMeasuredOpen >= TIMES_MEASURED_OPEN) setState(PRE_TURN); // get ready to turn
            else steer();
            break;

        case PRE_TURN: // get ready to turn
            if (frontWall || millis() - stateStart >= OPENING_MS) turn(true, 90);
            else steer();
            break;

        case FINISH: //found long corridor
            if (dF <= FINISH_CM) { stop(); setState(DONE); }
            else steer();
            break;
    }
}


// #include "utils.hpp"
// #include "part2.hpp"

// const int CELL_CM    = 20;
// const int OPENING_MS = 600;   // drive into the middle of a gap before turning
// const int FINISH_CM  = 5;

// void part2Begin() {
//     while (true) {
//         unsigned long start = millis();   // when both sides became walls
//         bool seenRight = false;

//         // follow the corridor until there's a wall ahead or the right side opens
//         while (true) {
//             readSensors();
//             displayLEDwall();
//             if (dF <= FRONT_TURN_MAX) break;               // wall ahead
//             if (dL >= OPEN || dR >= OPEN) start = millis(); // a gap: dead end hasn't started yet
//             if (dR < OPEN) seenRight = true;
//             else if (seenRight) break;                      // right side opened
//             steer();
//         }

//         if (dF > FRONT_TURN_MAX) { forwardMs(OPENING_MS); turnRightXDeg(90); } // right opening
//         else if (dR >= OPEN)     turnRightXDeg(90);                            // corner right
//         else if (dL >= OPEN)     turnLeftXDeg(90);                             // corner left
//         else {                                                                 // dead end
//             long cm = (millis() - start) / MS_PER_CM + dF;
//             Serial.print("dead end = "); Serial.println(cm);
//             if (cm > CELL_CM * 3 / 2) break;   // long one: finish
//             turnRightXDeg(180);                // short one: spin out
//         }
//     }

//     // finish: creep up to the end wall, stop, blink
//     while (true) { readSensors(); if (dF <= FINISH_CM) break; steer(); }
//     stop();
//     while (true) {
//         bool on = (millis() / 500) % 2;
//         digitalWrite(LeftredLedPin, on);
//         digitalWrite(FrontredLedPin, on);
//         digitalWrite(RightredLedPin, on);
//     }
// }