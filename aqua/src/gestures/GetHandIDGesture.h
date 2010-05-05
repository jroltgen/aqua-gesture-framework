/**
 * GetHandIDGesture.h
 *
 * This class provides unified event to hand event associations.  The gesture
 * will take all events with the name "UnifiedEvent" and associate them with
 * hand positions which are sent as events with the name "HandPositionEvent".
 *
 * It will then create isntances of "HandIDTouchEvent" and send them along.
 * These events will have additional information contained within, namely
 * the handID of the touchpoint.
 *
 * The HandIDTouchEvent also contains a field which represents the confidence
 * level - i.e. how sure we are that this touch point should be associated
 * with this particular hand.
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
#ifndef _GETHANDIDGESTURE_H_
#define _GETHANDIDGESTURE_H_

#include <map>
#include <vector>

#include "../EventProcessor.h"
#include "../events/Event.h"
#include "Gesture.h"

class GetHandIDGesture : public Gesture {

// Attributes
private:
    /**
     * The collection of the known hand positions - this will be used to
     * determine the hand which is closest to this touch point.  It maps
     * a particular hand ID to a triple (x,y,x).
     */
	std::map<int, float*> _handPositions;
	std::map<int, int>    _eventIDToHandID;

// Methods
public:
    GetHandIDGesture(EventProcessor* publisher, int regionID = -1);
            
    virtual bool handleEvent(Event* event);
    
private:
    void handleHandUpdate(Event* e);
	void handleUnifiedEvent(Event* e);
};

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new GetHandIDGesture(publisher, regionID);
	}
}
#else
extern "C" {
    Gesture* createGesture(EventProcessor* publisher, int regionID) {
        return new GetHandIDGesture(publisher, regionID);
    }
}
#endif

#endif /* GETHANDIDGESTURE_H */
