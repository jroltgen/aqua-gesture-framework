/**
 * PrintEventGesture.cpp
 * 
 * Prints "Hello World" when an event is received.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */

#include <stdio.h>

#include "PrintEventGesture.h"

using namespace std;

bool PrintEventGesture::handleEvent(Event* event) {
    printEvent(event);
    return false;
}

string PrintEventGesture::getType(char type) {
    switch(type) {
    case EVENT_TYPE_DOWN:
        return string("DOWN");
    case EVENT_TYPE_MOVE:
        return string("MOVE");
    case EVENT_TYPE_UP:
        return string("UP");
    case EVENT_TYPE_HOVER:
        return string("OTHER");
    case EVENT_TYPE_OTHER:
        return string("HOVER");
    }
}

void PrintEventGesture::printEvent(Event* event) {
    printf("[PrintEventGesture] Received Event.\n");
    printf("\t%s : %s\n", event->getName().c_str(), event->getDesc().c_str());
    printf("\tType: %c ---", getType(event->getType()));
    printf(" ID: %d\n", event->getID());
    printf("\tX: %f\n\tY: %f\n\tZ: %f\n", event->getX(), event->getY(), 
            event->getZ());
}