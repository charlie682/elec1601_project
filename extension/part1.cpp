#include "utils.hpp"
#include "part1.hpp"

static void badLeftParallel() {
    turnRightXDeg(20); // clockwise
    forwardMs(500);      // straight
    turnLeftXDeg(20);    // anti-clockwise
    backwardMs(200);     // backward
}

// 6
static void badRightParallel() {
    turnLeftXDeg(20);    // anti-clockwise
    forwardMs(500);      // straight
    turnRightXDeg(20);   // clockwise
    backwardMs(200);     // backward
}

// 7
static void badLeft30Deg() {
    turnRightXDeg(30);   // undo the 30 deg
    badLeftParallel();   // now it's the parallel case
}

// 8
static void badRight30Deg() {
    turnLeftXDeg(30);
    badRightParallel();
}

// ---------- building blocks ----------

// scenarios 5-8: sort out a bad starting position before driving off
void takeOff() {
    readSensors();
    printSensors("take off");

    bool nearLeft  = dL <= NEAR;
    bool nearRight = dR <= NEAR;
    bool seesLeft  = dL < FAR_DIST;
    bool seesRight = dR < FAR_DIST;

    // up against one wall: parallel still sees the far wall, 30 deg doesn't (same test as essential)
    if (nearLeft && !nearRight) {
        if (seesRight) badLeftParallel();
        else           badLeft30Deg();
    }
    else if (nearRight && !nearLeft) {
        if (seesLeft)  badRightParallel();
        else           badRight30Deg();
    }
}

// scenario 1: drive down the corridor until a wall is ahead.
// the wheels stay on while we read the sensors, so it never stops to look.
void followUntilWall() {
    straight();
    while (true) {
        readSensors();
        showWalls();
        printSensors("follow");

        if (dF <= FRONT_TURN_MAX) break;                   // wall ahead at turning distance

        // drifted right up against a wall with nothing ahead -> essential correction
        if (dF == FAR_DIST && dL <= NEAR && dR > NEAR)      badLeftParallel();
        else if (dF == FAR_DIST && dR <= NEAR && dL > NEAR) badRightParallel();
        else steer();                                       // normal case: small steering nudges
    }
    stop();
    readSensors();                                          // clean reading now we're stopped
    printSensors("stopped at wall");
}

// scenarios 2/3: turn towards whichever side is open.
// nothing is hard-coded so the mirrored maze works too.
void turnToOpenSide() {
    if (dL > dR) turnLeftXDeg(90);
    else         turnRightXDeg(90);
}

// scenario 4: wall ahead (we only call this after followUntilWall) and walls both sides
bool atDeadEnd() {
    return dL < OPEN && dR < OPEN;
}

// ---------- the tasks: each one builds on the one before ----------

// (1) bad start, follow corridor, stop at the wall
void task1() {
    takeOff();
    followUntilWall();
}

// (2) task 1, turn, follow the next corridor, stop at the wall
void task2() {
    task1();
    turnToOpenSide();
    followUntilWall();
}

// (3) keep turning and following until we stop at a dead end
void task3() {
    task1();
    while (!atDeadEnd()) {
        turnToOpenSide();
        followUntilWall();
    }
}

// (4) task 3, 180 on the spot (scenario 4), then task 3 again back to the start
void task4() {
    task3();
    turnRightXDeg(180);
    task3();
}

void runTask(int task) {
    switch (task) {
        case 1: task1(); break;
        case 2: task2(); break;
        case 3: task3(); break;
        case 4: task4(); break;
    }
    stop();
}
