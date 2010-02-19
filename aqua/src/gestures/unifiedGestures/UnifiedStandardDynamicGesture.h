/**
 * UnifiedStandardDynamicGesture.h
 *
 * This class is the foundation for most universal gestures - it provides
 * the ability to keep track of ups, downs, and moves and keeps track of
 * the centroid location for all of the points.
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
#ifndef _UNIFIEDSTANDARDDYNAMICGESTURE_H_
#define _UNIFIEDSTANDARDDYNAMICGESTURE_H_

#include <map>
#include <vector>

#include "../../EventProcessor.h"
#include "../../events/Event.h"
#include "../../events/UnifiedEvent.h"
#include "../Gesture.h"
#include "TouchData.h"

class UnifiedStandardDynamicGesture : public Gesture {

// Attributes
protected:
    /**
     * The collection of the known points, indexed by id.
     */
    std::map<int, TouchData> _knownPoints;
    
    /**
     * The centroid before the move.
     */
    float                    _oldCentroid[3];
    
    /**
     * The centroid after the move.
     */
    float                    _newCentroid[3];
    
    /**
     * The sum of the x and y coordinates of all the points.
     */
    double					 _sumX, _sumY, _sumZ;

// Methods
public:
    UnifiedStandardDynamicGesture(EventProcessor* publisher, 
            int regionID = -1);
            
    virtual bool handleEvent(Event* event);
    
private:
    bool handleDown(Event* e);
    bool handleMove(Event* e);
    bool handleUp  (Event* e);
    
    // These methods are to be implemented in subclasses.
    virtual bool processDown(TouchData& data) = 0;
    virtual bool processMove(TouchData& data) = 0;
    virtual bool processUp  (TouchData& data) = 0;
    
    TouchData createTouchData(Event* e);
};

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new UnifiedStandardDynamicGesture(publisher, regionID);
	}
}
#else
// TODO linux support
#endif

#endif /* STANDARDDYNAMICGESTURE_H */
