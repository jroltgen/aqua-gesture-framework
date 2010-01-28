/**
 * YouRockGesture.cpp
 * 
 * Prints "Congratulations, you're awesome." when an event is received.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */

#include <stdio.h>
 
#include "YouRockGesture.h"

bool YouRockGesture::handleEvent(Event* event) {
    printHello();
    return false;
}

void YouRockGesture::printHello() {
    printf("Congratulations, you're awesome. My region ID is: %d\n", _regionID);
}