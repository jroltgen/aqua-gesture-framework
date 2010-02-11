/**
 * EventProcessor.h
 * 
 * This functions as the base class for all classes that can receive events. 
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