/**
 * EventProcessor.h
 * 
 * This functions as the base class for all classes that can receive events. 
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _EVENTPROCESSOR_H_
#define _EVENTPROCESSOR_H_

#include "events/Event.h"

class EventProcessor {

// Attributes

// Methods
public:
    virtual ~EventProcessor() {};
    
    /**
     * Returns a boolean for whether this event was “consumed” or not.  The 
     * caller determines what to do with the return value.
     */
    virtual bool processEvent(Event* e) = 0;
    
    /**
     * This method is primarily here to allow the ClientConnection to implement 
     * this interface and still let gestures publish events easily.  This 
     * method is implemented in this class to simply call processEvent(Event e),
     * so that events which subclass may choose only to implement the 
     * processEvent() method if they do not wish to care about the regionID.
     */
    virtual bool processEvent(Event* e, int regionID) {
        return processEvent(e);
    }
};

#endif