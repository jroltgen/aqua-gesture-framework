/**
 * Gesture.h
 * 
 * This functions as the base class for all gestures.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _GESTURE_H_
#define _GESTURE_H_

#include "../events/Event.h"
#include "../EventProcessor.h"

class Gesture : public EventProcessor {

// Attributes
private:
    EventProcessor* _publisher;
protected: // TODO change back to private
    int             _regionID;

// Methods
public:
    Gesture(EventProcessor* publisher, int regionID = -1) {
        _publisher = publisher;
        _regionID = regionID;
    }
    virtual bool processEvent(Event* event) = 0;
    
protected:   
    /**
     * Publishes an event to this gesture’s publisher.  This method is 
     * implemented here to allow subclasses to publish events without worrying 
     * about whether this gesture is a global gesture or region gesture – 
     * simply call the publishEvent method.
     */
    void publishEvent(Event* event) {
        if (_regionID == -1) {
            _publisher->processEvent(event);
        } else {
            _publisher->processEvent(event, _regionID);
        }
    }
};

#endif