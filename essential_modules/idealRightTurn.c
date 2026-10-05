#include "utils.h"

// the robot first loses right wall detection
// it then moves forward to be centered in corridor
// then we do right turn here 

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

