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

#include <vector>
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
// TODO linux support
#endif

#include "../events/Event.h"
#include "../EventProcessor.h"

class Gesture : public EventProcessor {

// Attributes
private:
    EventProcessor* _publisher;
    // Thread lock
    #ifdef _WIN32
    CRITICAL_SECTION myLock;
    #else
    // TODO linux support
    #endif
    
protected: // TODO change back to private
    int             _regionID;

// Methods
public:
    Gesture(EventProcessor* publisher, int regionID = -1) {
        #ifdef _WIN32
        InitializeCriticalSection(&myLock);
        #else
        // TODO linux support
        #endif
        
        _publisher = publisher;
        _regionID = regionID;
    };
    virtual ~Gesture() {
        #ifdef _WIN32
        DeleteCriticalSection(&myLock);
        #else
        // TODO linux support
        #endif
    
    };
    virtual bool processEvent(Event* event) {
        bool result;
        #ifdef _WIN32
        EnterCriticalSection(&myLock);
        result = handleEvent(event);
        LeaveCriticalSection(&myLock);
        #else
        // TODO linux support
        result = handleEvent(event);
        #endif
        return result;
    };
    
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
    };
    
    /**
     * This is the method that that Gesture must implment - this is to hide
     * concurrency-related issues for the gesture implementer, which are handled
     * correctly in the processEvent method.
     *
     */
    virtual bool handleEvent(Event* event) = 0;
    
};

#endif