/**
 * Gesture.h
 * 
 * This functions as the base class for all gestures.
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010  Jay Roltgen.
 *
 * Aqua is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
// TODO ls
#endif

#include "Event.h"
#include "EventProcessor.h"
//#include "../events/Event.h"
//#include "../EventProcessor.h"

class Gesture : public EventProcessor {

// Attributes
private:
    EventProcessor* _publisher;
    // Thread lock
    #ifdef _WIN32
    CRITICAL_SECTION myLock;
    #else
    pthread_mutex_t myLock;
    #endif
    
protected:
    int             _regionID;

// Methods
public:
    Gesture(EventProcessor* publisher, int regionID = -1) {
        #ifdef _WIN32
        InitializeCriticalSection(&myLock);
        #else
        pthread_mutex_init(&myLock, NULL);
        #endif
        
        _publisher = publisher;
        _regionID = regionID;
    };

    virtual ~Gesture() {
        #ifdef _WIN32
        DeleteCriticalSection(&myLock);
        #else
        pthread_mutex_destroy(&myLock);
        #endif
    };

    virtual bool processEvent(Event* event) {
        bool result;
        #ifdef _WIN32
        EnterCriticalSection(&myLock);
        result = handleEvent(event);
        LeaveCriticalSection(&myLock);
        #else
        pthread_mutex_lock(&myLock);
        result = handleEvent(event);
		pthread_mutex_unlock(&myLock);
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
