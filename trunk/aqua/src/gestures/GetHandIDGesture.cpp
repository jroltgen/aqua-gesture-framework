/**
 * GetHandIDGesture.cpp
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
#include <math.h>
#include <string>

#include "GetHandIDGesture.h"
#include "../events/HandIDTouchEvent.h"

using namespace std;

/**
 * Default Constructor
 */
GetHandIDGesture::GetHandIDGesture(
        EventProcessor* publisher, int regionID) : Gesture(publisher,
        regionID) {
}

bool GetHandIDGesture::handleEvent(Event* e) {
	string handString("HandPosition");
	string unifiedString("UnifiedEvent");
	if (e->getDesc().compare(handString) == 0) {
		handleHandUpdate(e);
		return false;
	} else if (e->getName().compare(unifiedString) == 0) {
		handleUnifiedEvent(e);
		// WE CONSUME THESE EVENTS.
		return true;
	}
}

/**
 * Update this hand position's entry in the list.
 */
void GetHandIDGesture::handleHandUpdate(Event* e) {	
	//printf("Handling hand update.\n");
	if (_handPositions.find(e->getID()) == _handPositions.end()) {
		// We need to create new space for this new hand.
		//printf("New hand ID: %d\n", e->getID());
		_handPositions[e->getID()] = new float[3];
	}
	_handPositions[e->getID()][0] = e->getX();
	_handPositions[e->getID()][1] = e->getY();
	_handPositions[e->getID()][2] = e->getZ();

}

void GetHandIDGesture::handleUnifiedEvent(Event* e) {
	// Compute the distance to a particular hand using just the 2D coordinates.
	map<int, float*>::iterator it;

	float minDistance = -1;
	int handID = -1;
	//printf("Handling unified event");
	if (e->getType() == EVENT_TYPE_DOWN) {
		// Calculate the hand id
		for (it = _handPositions.begin(); it != _handPositions.end(); it++) {
			float xDif = e->getX() - (*it).second[0];
			float yDif = e->getY() - (*it).second[1];
			float zDif = e->getZ() - (*it).second[2];
	
			float distance = sqrt(xDif * xDif + yDif * yDif);
	
			if (distance < minDistance || minDistance < 0) {
				minDistance = distance;
				handID = (*it).first;
			}
		}
		_eventIDToHandID[e->getID()] = handID;

	} else if (e->getType() == EVENT_TYPE_MOVE) {
		handID = _eventIDToHandID[e->getID()];
	} else if (e->getType() == EVENT_TYPE_UP) {
		handID = _eventIDToHandID[e->getID()];
		_eventIDToHandID.erase(e->getID());
	} else { 
		printf("[GetHandIDGesture] Unrecognized unified event.\n");
		return;
	}
	//printf("Got hand id: %d\n", handID);
	string name("HandIDTouchEvent");
	string desc("");
	HandIDTouchEvent event(name, desc, e->getType(), e->getID(), 
			e->getLocation(), handID);

	publishEvent(&event);
}
