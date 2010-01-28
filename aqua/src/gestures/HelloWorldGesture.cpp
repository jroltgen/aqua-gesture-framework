/**
 * HelloWorldGesture.cpp
 * 
 * Prints "Hello World" when an event is received.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */

#include <stdio.h>
 
#include "HelloWorldGesture.h"

bool HelloWorldGesture::handleEvent(Event* event) {
    printHello();
    return false;
}

void HelloWorldGesture::printHello() {
    printf("Hello, world! My region ID is: %d\n", _regionID);
}