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
    SimpleEvent(std::string& name, std::string& desc, char type, int id, 
            float* location);
    virtual ~SimpleEvent();

private:
    virtual char* serializeData(short& lengthOut);
};

#endif
