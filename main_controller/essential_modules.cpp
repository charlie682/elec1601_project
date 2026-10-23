#include "utils.hpp"
#include "essential_modules.hpp"

void unknown() {
    //LED right: OFF
    //LED mid: OFF
    //LED left: OFF

    displayCode(LED_OFF, LED_OFF, LED_OFF);

    stop();
}

void middleLongCorridor() {
    // LED right: on
    // LED mid: off
    // LED left: off

    displayCode(LED_ON, LED_OFF, LED_OFF);

    forwardMs(600);
}

void idealLeftTurn() {
    // LED right: off
    // LED mid: on
    // LED left: off

    displayCode(LED_ON, LED_ON, LED_OFF);

    turnLeftXDeg(90);

    forwardMs(800); 
}


void idealRightTurn() {
    // LED right: off
    // LED mid: on
    // LED left: off

    displayCode(LED_OFF, LED_ON, LED_OFF);


    turnRightXDeg(90);

    forwardMs(800);
}

void deadEnd() {
    //LED right: OFF
    //LED mid: OFF
    //LED left: ON

    displayCode(LED_OFF, LED_OFF, LED_ON);


    turnRightXDeg(190);

    forwardMs(600); 
}

void badRightParallel() {
    // LED right: off
    // LED mid: on
    // LED left: on

    displayCode(LED_OFF, LED_ON, LED_ON);

    // anti clockwise
    turnLeftXDeg(20);

    // straight
    forwardMs(500);

    // clockwise
    turnRightXDeg(20);

    // backward
    backwardMs(200);   
}

void badLeftParallel() {
    // LED right: on
    // LED mid: off
    // LED left: on

    displayCode(LED_ON, LED_OFF, LED_ON);

    // clockwise
    turnRightXDeg(20);

    // straight
    forwardMs(500);

    // anti-clockwise
    turnLeftXDeg(20);

    // backward
    backwardMs(200);

}

void badRight30Deg() {
    // LED right: flash
    // LED mid: on
    // LED left: on

    // still needs to flash !!! todo
    displayCode(LED_FLASH, LED_ON, LED_ON);

    turnLeftXDeg(30);

    // anti clockwise
    turnLeftXDeg(20);

    // straight
    forwardMs(500);

    // clockwise
    turnRightXDeg(20);

    // backward
    backwardMs(200);

    // int i = 0;
    // while (i < 3) {
    //     i++;
    //     int d0 = irDistanceCm(RightredLedPin, RightirReceiverPin); //distance 

    //     straight();
    //     delay(100); // move straight to test if we get closer to the right wall, if we do then we need to do larger turn

    //     int d1 = irDistanceCm(RightredLedPin, RightirReceiverPin);
    //     int front = irDistanceCm(FrontredLedPin, FrontirReceiverPin);

    //     if (front <= 5) {  
    //         turnLeftXDeg(15);
    //         continue;
    //     }

    //     if (d1 - d0 < -1) turnLeftXDeg(8); // still angled towards right
    //     else if (d1 - d0 > 1) turnRightXDeg(8); // over angled
    //     else break; // essentially parallel 

    // }

    // forwardMs(100);
}

void badLeft30Deg() {
    // LED right: on
    // LED mid: on
    // LED left: on

    displayCode(LED_ON, LED_ON, LED_ON);

    turnRightXDeg(30);

    // clockwise
    turnRightXDeg(20);

    // straight
    forwardMs(500);

    // anti-clockwise
    turnLeftXDeg(20);

    // backward
    backwardMs(200);

    // int i = 0;
    // while (i < 3) {
    //     i++;
    //     int d0 = irDistanceCm(LeftredLedPin, LeftirReceiverPin); //distance 

    //     straight();
    //     delay(100); // move straight to test if we get closer to the right wall, if we do then we need to do larger turn

    //     int d1 = irDistanceCm(LeftredLedPin, LeftirReceiverPin);
    //     int front = irDistanceCm(FrontredLedPin, FrontirReceiverPin);

    //     if (front <= 5) {  
    //         turnRightXDeg(15);
    //         continue;
    //     }

    //     if (d1 - d0 < -1) turnRightXDeg(8); // still angled towards right
    //     else if (d1 - d0 > 1) turnLeftXDeg(8); // over angled
    //     else break; // essentially parallel 

    // }

    // forwardMs(100);
}

//  into a corner (right turn)
// L45000, M43250 R43000
// close front, close left, no right 
void ownChoice1() {
    displayCode(LED_OFF, LED_FLASH, LED_OFF);

    backwardMs(300);

    turnRightXDeg(55);
}

// FOR NEXT TIME

//parallel to a left turn corner
// nothing on left, long wall on right, wall in front but not in boundary
void ownChoice2() {
    displayCode(LED_FLASH, LED_FLASH, LED_OFF);

    forwardMs(600);
}