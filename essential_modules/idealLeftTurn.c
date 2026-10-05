#include "utils.h"

// the robot first loses left wall detection
// it then moves forward to be centered in corridor
// then we do left turn here 

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