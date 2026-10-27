#include "utils.hpp"
#include "part1.hpp"

static void badLeftParallel() {
    turnRightXDeg(20); // clockwise
    forwardMs(500);      // straight
    turnLeftXDeg(20);    // anti-clockwise
    backwardMs(200);     // backward
}

static void badRightParallel() {
    turnLeftXDeg(20);    // anti-clockwise
    forwardMs(500);      // straight
    turnRightXDeg(20);   // clockwise
    backwardMs(200);     // backward
}

static void badLeft30Deg() {
    turnRightXDeg(30);   // undo the 30 deg
    badLeftParallel();   // now it's the parallel case
}

static void badRight30Deg() {
    turnLeftXDeg(30);
    badRightParallel();
}


void startingPos() {
    readSensors();

    bool nearLeft = dL <= NEAR;
    bool nearRight = dR <= NEAR;
    bool detectLeft = dL < FAR_DIST;
    bool detectRight = dR < FAR_DIST;

    if (nearLeft && !nearRight) {
        if (detectRight) badLeftParallel();
        else badLeft30Deg();
    }
    else if (nearRight && !nearLeft) {
        if (detectLeft) badRightParallel();
        else badRight30Deg();
    }
}

// drive striaght until there is a front wall, adjusting if off center
void straightUntilWall() {
    straight();
    while (true) {
        readSensors();
        displayLEDwall();

        if (dF <= FRONT_TURN_MAX) break;   

        if (dF == FAR_DIST && dL <= NEAR && dR > NEAR) badLeftParallel();
        else if (dF == FAR_DIST && dR <= NEAR && dL > NEAR) badRightParallel();
        else steer();    
    }
    stop();
    readSensors();  
}


// turn to whichever side is open
void turnToOpenSide() {
    if (dL > dR) turnLeftXDeg(90);
    else         turnRightXDeg(90);
}

// check if dead end 
bool atDeadEnd() {
    return dL < OPEN && dR < OPEN;
}

void task1() {
    startingPos();
    straightUntilWall();
}

void task2() {
    task1();
    turnToOpenSide();
    straightUntilWall();
}

void task3() {
    task1();
    while (!atDeadEnd()) {
        turnToOpenSide();
        straightUntilWall();
    }
}

//task 3 twice
void task4() {
    task3();

    if (dL <= dR) {turnLeftXDeg(180);}
    else {turnRightXDeg(180);}
    task3();
}

