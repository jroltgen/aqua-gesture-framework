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
#ifndef _YOUROCKGESTURE_H_
#define _YOUROCKGESTURE_H_

#include "../events/Event.h"
#include "Gesture.h"  

class YouRockGesture : public Gesture {

// Attributes


// Methods
public:
    YouRockGesture(EventProcessor* publisher, int regionID = -1) :
            Gesture(publisher, regionID) {};
    virtual bool processEvent(Event* event);
    
private:
    void printHello();
};

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new YouRockGesture(publisher, regionID);
	}
}
#else
// TODO linux support
#endif

#endif