#include "utils.h"

void badLeft30Deg() {
    // LED right: on
    // LED mid: on
    // LED left: on

    LEDSwitch(RightirLedPin, 38000, 0); 
    LEDSwitch(FrontirLedPin, 38000, 0);
    LEDSwitch(LeftirLedPin, 38000, 0);

    turnRightXDeg(20);

    int i = 0;
    while (i < 3) {
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

        i++;
    }

    straight();
    delay(1500); // 1.5 seconds but will need checking
}