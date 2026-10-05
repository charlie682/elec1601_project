#include "utils.h"

// robot sees front, left and right walls
// when moving straight, we should track distance to verify if it is a long dead end (end of maze)

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

