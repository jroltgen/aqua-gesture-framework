/**
 * SimpleEvent.h
 *
 * This class is the base class for most simple events.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef SIMPLEEVENT_H_
#define SIMPLEEVENT_H_

#include "Event.h"

class SimpleEvent : public Event {
    
// Attributes

// Methods
public:
    SimpleEvent(char* data);
    virtual ~SimpleEvent();

private:
    virtual char* serializeData(int& lengthOut);
};

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Event* createEvent(char* data) {
		return new SimpleEvent(data);
	}
}
#else
// TODO linux support
#endif

#endif
