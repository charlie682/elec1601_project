#include "utils.hpp"
#include "part2.hpp"

// ===================== EXTENSION PART 2 =====================
// A state machine. Every loop() we read the sensors once, make one decision,
// set the wheels and return. Anything timed (driving on, spinning) is checked
// with millis() instead of delay(), so the robot keeps sensing while it moves.
//
// Strategy: right-hand rule. Keep the right wall and take every right opening.
// A maze with no loops gets fully explored this way, so we must reach the long
// dead end. Short dead ends are spun out of. Works on the mirrored maze as well.

const bool FOLLOW_RIGHT = true;   // false = left-hand rule

// ---- tune these on the real maze ----
const int  CELL_CM    = 20;    // maze is a 20 x 20 cm grid
const long OPENING_MS = 600;   // keep driving after our side opens, so we turn in the middle of the gap
const int  OPEN_READS = 2;     // readings in a row before we trust an opening (ignores one bad IR read)
const int  FINISH_CM  = 5;     // how close to creep to the end wall of the long dead end

enum State { FOLLOW, PRE_TURN, TURNING, FINISH, DONE };
const char* const STATE_NAME[] = { "FOLLOW", "PRE_TURN", "TURNING", "FINISH", "DONE" };

State state = FOLLOW;
unsigned long stateStart  = 0;  // millis() when we entered the current state
unsigned long turnTime    = 0;  // how long the current spin lasts (ms)
unsigned long walledSince = 0;  // millis() when both sides last became walls = start of this straight
bool seenOurWall = false;       // seen our wall since the last turn? (so we don't turn back the way we came)
int  openReads   = 0;           // readings in a row with our wall missing

static void setState(State s) {
    state = s;
    stateStart = millis();
}

// start a spin; part2Step() ends it once turnTime has passed
static void startTurn(bool right, int deg) {
    turnTime = spinMs(deg, right);
    if (right) rotateRight();
    else       rotateLeft();
    setState(TURNING);
}

// long dead end = a walled-in straight exactly two walls long.
// length = distance driven since both sides became walls + the gap we stopped short of the end wall
static bool isLongDeadEnd() {
    long straightCm = (long)((millis() - walledSince) / MS_PER_CM) + FRONT_TURN_MAX;
    Serial.print("dead end, straight = "); Serial.print(straightCm); Serial.println(" cm");
    return straightCm > CELL_CM * 3 / 2 && straightCm < CELL_CM * 5 / 2;   // 1.5 to 2.5 walls
}

void part2Begin() {
    walledSince = millis();
    straight();
    setState(FOLLOW);
}

void part2Step() {
    unsigned long now = millis();

    // spinning: nothing useful to sense, and skipping the IR scan keeps the turn timing exact
    if (state == TURNING) {
        if (now - stateStart >= turnTime) {
            seenOurWall = false;
            openReads   = 0;
            walledSince = now;
            straight();
            setState(FOLLOW);
        }
        return;
    }

    // finished: wheels already stopped, blink all LEDs
    if (state == DONE) {
        bool on = (now / 500) % 2;
        digitalWrite(LeftredLedPin, on);
        digitalWrite(FrontredLedPin, on);
        digitalWrite(RightredLedPin, on);
        return;
    }

    readSensors();
    displayLEDwall();

    bool leftWall  = dL < OPEN;
    bool rightWall = dR < OPEN;
    bool frontWall = dF <= FRONT_TURN_MAX;
    bool ourWall   = FOLLOW_RIGHT ? rightWall : leftWall;    // the wall we follow
    bool otherWall = FOLLOW_RIGHT ? leftWall  : rightWall;

    if (!leftWall || !rightWall) walledSince = now;          // a gap on a side: straight hasn't started yet

    switch (state) {
        case FOLLOW:
            if (ourWall) { seenOurWall = true; openReads = 0; }
            else         openReads++;

            if (frontWall) {
                if (!ourWall)            startTurn(FOLLOW_RIGHT, 90);    // corner / T: our way first
                else if (!otherWall)     startTurn(!FOLLOW_RIGHT, 90);   // corner the other way
                else if (isLongDeadEnd()) setState(FINISH);              // walls 3 sides, right length
                else                     startTurn(FOLLOW_RIGHT, 180);   // short dead end: spin out
            }
            else if (seenOurWall && openReads >= OPEN_READS) setState(PRE_TURN);  // our side opened
            else steer();
            break;

        case PRE_TURN:   // drive into the middle of the opening, then turn into it
            if (frontWall || now - stateStart >= OPENING_MS) startTurn(FOLLOW_RIGHT, 90);
            else steer();
            break;

        case FINISH:     // long dead end: creep up to the end wall facing forward, then stop for good
            if (dF <= FINISH_CM) { stop(); setState(DONE); }
            else steer();
            break;

        default:
            break;
    }
}
