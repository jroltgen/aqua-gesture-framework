/**
 * UnifiedZoomGesture.cpp
 *
 * Processes a zoom.
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
#include "UnifiedZoomGesture.h"
#include "../../events/UnifiedZoomEvent.h"

#include <math.h>

using namespace std;

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new UnifiedZoomGesture(publisher, regionID);
	}
}
#else
extern "C" {
    Gesture* createGesture(EventProcessor* publisher, int regionID) {
		return new UnifiedZoomGesture(publisher, regionID);
	}
}
#endif

using namespace std;

UnifiedZoomGesture::UnifiedZoomGesture(EventProcessor* publisher, 
        int regionID) : UnifiedStandardDynamicGesture(publisher, regionID) {
}

bool UnifiedZoomGesture::processDown(TouchData& touchData) {
    calculateScaleChange();
    return false;
}

bool UnifiedZoomGesture::processMove(TouchData& touchData){

    string name("UnifiedZoomEvent");
    string desc("UnifiedZoomEvent");

    float scale = calculateScaleChange();
    UnifiedZoomEvent e(name, desc, EVENT_TYPE_OTHER, 0, _originalCentroid,
            scale, _newCentroid);
	publishEvent(&e);
    return false;
}

bool UnifiedZoomGesture::processUp(TouchData& touchData){   
    calculateScaleChange();
	return false;
}

float UnifiedZoomGesture::calculateScaleChange() {
    float retScale = 0.0;
    map<int, TouchData>::iterator it;
    
    for (it = _knownPoints.begin(); it != _knownPoints.end(); it++) {
        // Get this point's touch data.
        TouchData* data = &it->second;
        
        // Find the distance of this touch point from the centroid.
        float x = _newCentroid[0] - data->getX();
        float y = _newCentroid[1] - data->getY();
        float z = _newCentroid[2] - data->getZ();
        float distance = sqrt(x * x + y * y + z * z);
        
        // Calculate the ratio of the point distance in this iteration
        // from the previos iteration.
        data->setDistance(distance);
        
        if (data->getOldDistance() != 0) {
            retScale += data->getDistance() / data->getOldDistance();
        } else {
            retScale += 1;
        }
    }
    // Get the average scale.
    retScale = retScale / _knownPoints.size();
    return retScale;
}
