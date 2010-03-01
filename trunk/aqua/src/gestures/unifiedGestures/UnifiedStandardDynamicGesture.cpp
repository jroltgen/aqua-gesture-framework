/**
 * UnifiedStandardDynamicGesture.cpp
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
#include <stdio.h>

#include "UnifiedStandardDynamicGesture.h"

using namespace std;

/**
 * Default Constructor
 */
UnifiedStandardDynamicGesture::UnifiedStandardDynamicGesture(
        EventProcessor* publisher, int regionID) : Gesture(publisher,
        regionID) {
    int i;
        
    _sumX = 0.0;
    _sumY = 0.0;
    _sumZ = 0.0;
 
    for (i = 0; i < 3; i++) {
        _oldCentroid[i] = 0.0;
        _newCentroid[i] = 0.0;
		_originalCentroid[i] = 0.0;
        _offset[i] = 0.0;
    }
}

/**
 * Processes change to an individual point.
 * Changes can be
 *	- Down: We first add the new point to the set of existing points
 *	- Move:  
 *	- Up:   We first remove the point from the set of existing points
 *  
 * After this is done, recalculate the centroid, and then call appropriate 
 * methods to update the set of points and the list of events.
 */
bool UnifiedStandardDynamicGesture::handleEvent(Event* e) {
	switch (e->getType()) {
	case EVENT_TYPE_DOWN:
		return handleDown(e);
	case EVENT_TYPE_MOVE:
		return handleMove(e);
	case EVENT_TYPE_UP:
		return handleUp(e);
    default:
        return false;
	}
}

TouchData UnifiedStandardDynamicGesture::createTouchData(Event* e) {
    TouchData ret(e->getLocation());
    return ret;
}

/**
 * Update the state and the event list resulting from the creation of a new point.
 * Creates a new point, adds it to the vector of known points, updates the centroid.
 * 
 * @param  touchPoint
 * 		The new touch point
 * @return
 * 		A vector of pointers to events where the last pointer points to the birth event.
 */
bool UnifiedStandardDynamicGesture::handleDown(Event* e) {

    int i;
    float averages[3];
	
    TouchData* data;

	_knownPoints[e->getID()] = createTouchData(e);
    data = &_knownPoints[e->getID()];
    
	_sumX += data->getLocation()[0];
	_sumY += data->getLocation()[1];
    _sumZ += data->getLocation()[2];
    
	// Store the old centroid, recompute the new centroid
	averages[0] = _sumX / _knownPoints.size();
	averages[1] = _sumY / _knownPoints.size();
    averages[2] = _sumZ / _knownPoints.size();
	
    
    for (i = 0; i < 3; i++) {
        _oldCentroid[i] = _newCentroid[i];
        _newCentroid[i] =  averages[i];
		
		
		
		if (_knownPoints.size() == 1) {
			// This is the first point down - so the original centroid will
			// set here.  Do not change the offset.
			_originalCentroid[i] = _newCentroid[i];
		} else {
            // Otherwise, this is at least the second point, so we need
            // to update the offset.
            _offset[i] += _newCentroid[i] - _oldCentroid[i];
        }
		
    }
	return processDown(*data);
} 

/**
 * Updates the state and the event list resulting from moving a point.
 * Updates the TouchData of the point in the list of known points, and the centroid.
 * 
 * @param  touchPoint
 * 		The touch point which was changed
 * @return  
 * 		A vector of pointers to events where the last pointer points to the move event.
 */
bool UnifiedStandardDynamicGesture::handleMove(Event* e) {
    float newx, newy, newz, oldx, oldy, oldz;
    float averages[3];
    int i;
	
    if (_knownPoints.find(e->getID()) == _knownPoints.end()) {
        printf("[UnifiedSDGesture] Error:ID not found in map %d\n", e->getID());
        return false;
    }
    
    TouchData* data = &_knownPoints[e->getID()];
    
    data->setLocation(e->getLocation());
    
	newx = data->getX();
    newy = data->getY();
    newz = data->getZ();
    oldx = data->getOldX();
    oldy = data->getOldY();
    oldz = data->getOldZ();
    
	_sumX += newx - oldx;
	_sumY += newy - oldy;
    _sumZ += newz - oldz;
        
	// Store the old centroid, recompute the new centroid.
    averages[0] = _sumX / _knownPoints.size();
    averages[1] = _sumY / _knownPoints.size();
    averages[2] = _sumZ / _knownPoints.size();
    
    for (i = 0; i < 3; i++) {
        _oldCentroid[i] = _newCentroid[i];
        _newCentroid[i] = averages[i];
		// Here the offset does not change, but the original centroid must
		// move appropriately.
		_originalCentroid[i] += _newCentroid[i] - _oldCentroid[i];
    }
    
	return processMove(*data);
}

/**
 * Updates the state and the event list resulting from the death of a point.
 * @param  touchPoint: the point to be removed
 * @return a vector of pointers to events where the last pointer points to the death event.
 */
bool UnifiedStandardDynamicGesture::handleUp(Event* e) {

    int i;
    float averages[3];
	
    if (_knownPoints.find(e->getID()) == _knownPoints.end()) {
        printf("[UnifiedSDGesture] Error:ID not found in map %d\n", e->getID());
        return false;
    }
    
    TouchData* data = &_knownPoints[e->getID()];
    data->setLocation(e->getLocation());
    
    _sumX -= data->getX();
    _sumY -= data->getY();
	_sumZ -= data->getZ();
	
    _knownPoints.erase(e->getID());
    
    if (_knownPoints.size() > 0) {
        averages[0] = _sumX / _knownPoints.size();
		averages[1] = _sumY / _knownPoints.size();
        averages[2] = _sumZ / _knownPoints.size();
        
        for (i = 0; i < 3; i++) {
            _oldCentroid[i] = _newCentroid[i];
            _newCentroid[i] = averages[i];
			// Here the centroid has moved - we will not move the original
			// centroid, but will update the offset to refelct the change.
			_offset[i] += _newCentroid[i] - _oldCentroid[i];
        }
		
	} else {
        for (i = 0; i < 3; i++) {
            _oldCentroid[i] = 0.0;
            _newCentroid[i] = 0.0;
			_originalCentroid[i] = 0.0;
            _offset[i] = 0.0;
        }
	}

	return processUp(*data);
}

/**
 * Prints info related to this standard dynamic gesture.
 */
void UnifiedStandardDynamicGesture::printInfo() {
    
    printf("Known points: %d\n", _knownPoints.size());
    
    printf("Old centroid:  %5.3f, %5.3f, %5.3f\n",
            _oldCentroid[0], _oldCentroid[1], _oldCentroid[2]);
    
    printf("New centroid:  %5.3f, %5.3f, %5.3f\n",
            _newCentroid[0], _newCentroid[1], _newCentroid[2]);
            
    printf("Orig centroid: %5.3f, %5.3f, %5.3f\n",
            _originalCentroid[0], _originalCentroid[1], _originalCentroid[2]);
            
    printf("Offset:        %5.3f, %5.3f, %5.3f\n",
            _offset[0], _offset[1], _offset[2]);
            
    printf("--------------------------\n");

}
