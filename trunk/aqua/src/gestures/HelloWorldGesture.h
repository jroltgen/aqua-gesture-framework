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
#ifndef _HELLOWORLDGESTURE_H_
#define _HELLOWORLDGESTURE_H_

#include "../events/Event.h"
#include "Gesture.h"  

class HelloWorldGesture : public Gesture {

// Attributes


// Methods
public:
    HelloWorldGesture(EventProcessor* publisher, int regionID = -1) :
            Gesture(publisher, regionID) {};
    virtual bool processEvent(Event* event);
    
private:
    void printHello();
};

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new HelloWorldGesture(publisher, regionID);
	}
}
#else
// TODO linux support
#endif

#endif