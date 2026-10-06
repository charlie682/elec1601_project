#include "utils.h"
#include "essential_modules.h"

void unknown() {
    //LED right: OFF
    //LED mid: OFF
    //LED left: OFF

    LEDSwitch(RightirLedPin, 38000, 0);
    LEDSwitch(FrontirLedPin, 38000, 0);
    LEDSwitch(LeftirLedPin, 38000, 0);

    stop();
}

void middleLongCorridor() {
    // LED right: on
    // LED mid: off
    // LED left: off

    LEDSwitch(RightirLedPin, 38000, 1);
    LEDSwitch(FrontirLedPin, 38000, 0);
    LEDSwitch(LeftirLedPin, 38000, 0);

    straight();
    delay(500); // will need checking
}

void idealLeftTurn() {
    // LED right: off
    // LED mid: on
    // LED left: off

    LEDSwitch(RightirLedPin, 38000, 0);
    LEDSwitch(FrontirLedPin, 38000, 1);
    LEDSwitch(LeftirLedPin, 38000, 0);

    turnLeftXDeg(90);

    straight();
    delay(1500); // 1.5 seconds but will need checking 
}


void idealRightTurn() {
    // LED right: off
    // LED mid: on
    // LED left: off

    LEDSwitch(RightirLedPin, 38000, 0);
    LEDSwitch(FrontirLedPin, 38000, 1);
    LEDSwitch(LeftirLedPin, 38000, 0);

    turnRightXDeg(90);

    straight();
    delay(1500); // 1.5 seconds but will need checking 
}

void deadEnd() {
    //LED right: OFF
    //LED mid: OFF
    //LED left: ON

    LEDSwitch(RightirLedPin, 38000, 0);
    LEDSwitch(FrontirLedPin, 38000, 0);
    LEDSwitch(LeftirLedPin, 38000, 1);

    turnRightXDeg(180);

    straight();
    delay(1500); // 1.5 seconds but will need checking
}

void badRightParallel() {
    // LED right: off
    // LED mid: on
    // LED left: on

    LEDSwitch(RightirLedPin, 38000, 0); 
    LEDSwitch(FrontirLedPin, 38000, 1);
    LEDSwitch(LeftirLedPin, 38000, 1);

    // clockwise
    turnLeftXDeg(10);

    // straight
    straight();
    delay(300);

    // anti-clockwise
    turnRightXDeg(10);

    // backward
    backward();
    delay(300);
}

void badLeftParallel() {
    // LED right: on
    // LED mid: off
    // LED left: on

    LEDSwitch(RightirLedPin, 38000, 1); 
    LEDSwitch(FrontirLedPin, 38000, 0);
    LEDSwitch(LeftirLedPin, 38000, 1);

    // clockwise
    turnRightXDeg(10);

    // straight
    straight();
    delay(300);

    // anti-clockwise
    turnLeftXDeg(10);

    // backward
    backward();
    delay(300);
}

void badRight30Deg() {
    // LED right: flash
    // LED mid: on
    // LED left: on

    LEDSwitch(RightirLedPin, 1, 1); // 1 second flash
    LEDSwitch(FrontirLedPin, 38000, 0);
    LEDSwitch(LeftirLedPin, 38000, 0);

    turnLeftXDeg(20);

    int i = 0;
    while (i < 3) {
        i++;
        int d0 = irDistanceCm(RightirLedPin, RightirReceiverPin); //distance 

        straight();
        delay(400); // move straight to test if we get closer to the right wall, if we do then we need to do larger turn

        int d1 = irDistanceCm(RightirLedPin, RightirReceiverPin);
        int front = irDistanceCm(FrontirLedPin, FrontirReceiverPin);

        if (front <= 5) {  
            turnLeftXDeg(15);
            continue;
        }

        if (d1 - d0 < -1) turnLeftXDeg(8); // still angled towards right
        else if (d1 - d0 > 1) turnRightXDeg(8); // over angled
        else break; // essentially parallel 

    }

    straight();
    delay(1500); // 1.5 seconds but will need checking
}

void badLeft30Deg() {
    // LED right: on
    // LED mid: on
    // LED left: on

    LEDSwitch(RightirLedPin, 38000, 1); 
    LEDSwitch(FrontirLedPin, 38000, 1);
    LEDSwitch(LeftirLedPin, 38000, 1);

    turnRightXDeg(20);

    int i = 0;
    while (i < 3) {
        i++;
        int d0 = irDistanceCm(LeftirLedPin, LeftirReceiverPin); //distance 

        straight();
        delay(400); // move straight to test if we get closer to the right wall, if we do then we need to do larger turn

        int d1 = irDistanceCm(LeftirLedPin, LeftirReceiverPin);
        int front = irDistanceCm(FrontirLedPin, FrontirReceiverPin);

        if (front <= 5) {  
            turnRightXDeg(15);
            continue;
        }

        if (d1 - d0 < -1) turnRightXDeg(8); // still angled towards right
        else if (d1 - d0 > 1) turnLeftXDeg(8); // over angled
        else break; // essentially parallel 

    }

    straight();
    delay(1500); // 1.5 seconds but will need checking
}
