/**
 * PrintEventGesture.h
 * 
 * Prints event information.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _PRINTEVENTGESTURE_H_
#define _PRINTEVENTGESTURE_H_

#include <string>

#include "../events/Event.h"
#include "Gesture.h"  

class PrintEventGesture : public Gesture {

// Attributes


// Methods
public:
    PrintEventGesture(EventProcessor* publisher, int regionID = -1) :
            Gesture(publisher, regionID) {};
    virtual bool handleEvent(Event* event);
    
private:
    void printEvent(Event* e);
    std::string getType(char type);
};

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new PrintEventGesture(publisher, regionID);
	}
}
#else
// TODO linux support
#endif

#endif