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

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Event* createEvent(char* data) {
		return new SimpleEvent(data);
	}
}
#else
// TODO linux support
#endif

SimpleEvent::SimpleEvent(char* data) : Event(data) {
    // Here is where we would use data to initiailize our members, but
    // we don't have any, so we don't have to do anything.
}

SimpleEvent::SimpleEvent(std::string& name, std::string& desc, char type, 
        int id, float* location) : Event(name, desc, type, id, location) {
    // Again, we don't have to initialize any of our own members.
}

SimpleEvent::~SimpleEvent() {
}

char* SimpleEvent::serializeData(short &lengthOut) {
    lengthOut = 0;
    return new char[0];
}
