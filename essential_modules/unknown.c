#include "utils.h"

void unknown() {
    //LED right: OFF
    //LED mid: OFF
    //LED left: OFF

    LEDSwitch(RightirLedPin, 38000, 0);
    LEDSwitch(FrontirLedPin, 38000, 0);
    LEDSwitch(LeftirLedPin, 38000, 0);

    stop();
}