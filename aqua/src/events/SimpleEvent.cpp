/**
 * SimpleEvent.cpp
 *
 * This class is the base class for most simple events.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include <stdio.h>
 
#include "SimpleEvent.h"

SimpleEvent::SimpleEvent(char* data) : Event(data) {
    // Here is where we would use data to initiailize our members, but
    // we don't have any, so we don't have to do anything.
}

SimpleEvent::~SimpleEvent() {
}

char* SimpleEvent::serializeData(int &lengthOut) {
    lengthOut = 0;
    return new char[0];
}
