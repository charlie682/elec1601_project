#include "utils.hpp"
#include "part2.hpp"

const long OPENING_MS = 600; // drive into the middle of a gap before turning
const int  FINISH_CM  = 5;
const unsigned long SETTLE_MS = 150; // stand still before rotation 

enum State {STRAIGHT, PRE_TURN, TURNING, EXPLORE, FINISH, DONE};

// initial values!
State state = STRAIGHT;
unsigned long stateStart  = 0;
unsigned long TimeAtLastSideWallOpening = 0;
bool followRight = true; // which wall we follow, flips at every four-way junction
bool seenFollowWall = false;
int timesMeasuredOpen = 0;
unsigned long turnTime = 0; // how long the current spin should last
bool turnRight = true; // which way the current spin goes
State afterTurn = STRAIGHT; // state to go to once the spin is done

// four-way exploring: check the right branch, then the left, for the long dead end
int explored = 0; // branches checked so far at this four-way
bool returning = false; // neither branch was it, heading back to the four-way
bool followBefore = true; // wall we were following when we reached the four-way

// how many reads of sensors there has been an opening
int gapReads = 0; 


static void setState(State s) {
    state = s;
    stateStart = millis();
}

static void turn(bool right, int deg, State next = STRAIGHT) {
    stop();
    if (deg == 90) centreBeforeTurn(); // to avoid bumping wall!!!!
    turnRight = right;
    turnTime = spinMs(deg, right);
    afterTurn = next;
    setState(TURNING);
}

// boudnary between long corridor 
const unsigned long LONG_MIN_MS = 2000; 
const unsigned long LONG_MAX_MS = 6000; 

static bool isLongDeadEnd() {
    unsigned long ms = millis() - TimeAtLastSideWallOpening;  
    return ms > LONG_MIN_MS && ms < LONG_MAX_MS;
}

void part2Begin() {
    TimeAtLastSideWallOpening = millis();
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
            else rotateLeft();
            return;
        }
        seenFollowWall = false; 
        timesMeasuredOpen = 0;
        TimeAtLastSideWallOpening = millis();
        straight();
        setState(afterTurn);
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
    bool followWall = followRight ? rightWall : leftWall; // the wall we're following
    bool otherWall  = followRight ? leftWall : rightWall;

    // increment the number of gap reads if there is a gap
    if (!leftWall || !rightWall) gapReads++;
    else gapReads = 0;
    if (gapReads >= 5) TimeAtLastSideWallOpening = millis(); //reset the timer if there is definitely a gap

    switch (state) {
        case STRAIGHT:
            //here we check if we have seen multiple openings in the right wall. 
            //if we only see one, it could be a bug so we dont want to commit to that turn yet. 
            // if we see right walls, then it gets reset as no longer opening
            if (followWall) {seenFollowWall = true; timesMeasuredOpen = 0;}
            else timesMeasuredOpen++;

            if (frontWall) {
                if (!followWall) turn(followRight, 90); // go our way if open
                else if (!otherWall) turn(!followRight, 90); // otherwise the other way if open
                else if (isLongDeadEnd()) setState(FINISH); // check if long corridor
                else turn(dR < dL, 180); // short dead end: 180 towards the closer wall, like part 1 task4
            }
            else if (seenFollowWall && timesMeasuredOpen >= 2) setState(PRE_TURN); // get ready to turn
            else steer();
            break;

        case PRE_TURN: // get ready to turn, compare to calculated timing to ensure centered
            if (frontWall) turn(followRight, 90);
            else if (millis() - stateStart >= OPENING_MS) {
                bool fourWay = !leftWall && !rightWall && dF > OPEN; // left, right and front all open
                if (returning) { // back at a four-way with no dead end: carry on the way we were going
                    returning = false;
                    turn(false, 90); // our original heading is on the left now
                    followRight = !followBefore; // and follow the other wall from now on
                }
                else if (fourWay) { // explore the right branch first
                    followBefore = followRight;
                    explored = 0;
                    turn(true, 90, EXPLORE);
                }
                else turn(followRight, 90);
            }
            else steer();
            break;

        case EXPLORE: // driving down a four-way branch looking for the long dead end
            if (frontWall && leftWall && rightWall) { // hit a dead end
                if (isLongDeadEnd()) setState(FINISH); // found it
                else {
                    explored++;
                    if (explored == 1) {
                        turn(dR < dL, 180, EXPLORE);
                    } // short one: back through the four-way into the left branch
                    else { // left was a short dead end too: head back to the four-way following the left wall
                        returning = true;
                        followRight = false;
                        turn(dR < dL, 180);
                    }
                }
            }
            else if (frontWall || millis() - TimeAtLastSideWallOpening > LONG_MAX_MS) { // not a dead end: just keep going
                followRight = !followBefore; // follow the other wall from now on
                setState(STRAIGHT);
            }
            else steer();
            break;

        case FINISH: //found long corridor
            if (dF <= FINISH_CM) {
                stop();
                setState(DONE);
            }
            else steer();
            break;
    }
}
