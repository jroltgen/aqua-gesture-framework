/**
 * UnifiedDragGesture.cpp
 *
 * Processes a drag.
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010 Iowa State University.
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
#include "UnifiedDragGesture.h"
#include "../../events/UnifiedDragEvent.h"

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new UnifiedDragGesture(publisher, regionID);
	}
}
#else
extern "C" {
    Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new UnifiedDragGesture(publisher, regionID);
	}
}
#endif

using namespace std;

UnifiedDragGesture::UnifiedDragGesture(EventProcessor* publisher, 
        int regionID) : UnifiedStandardDynamicGesture(publisher, regionID) {
}

bool UnifiedDragGesture::processDown(TouchData& touchData) {
    return false;
}

bool UnifiedDragGesture::processMove(TouchData& touchData){
    
    string name("UnifiedDragEvent");
    string desc("UnifiedDragEvent");

	UnifiedDragEvent e(name, desc, 
            EVENT_TYPE_OTHER, 0, _originalCentroid, 
            _newCentroid[0] - _oldCentroid[0],
            _newCentroid[1] - _oldCentroid[1],
            _newCentroid[2] - _oldCentroid[2]);
	publishEvent(&e);
    return false;
}

bool UnifiedDragGesture::processUp(TouchData& touchData){
	return false;
}


