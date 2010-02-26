/**
 * Unified2DRotateGesture.cpp
 *
 * Processes a 2D Rotate gesture.
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
#include "Unified2DRotateGesture.h"
#include "../../events/Unified2DRotateEvent.h"

#include <math.h>

using namespace std;

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new Unified2DRotateGesture(publisher, regionID);
	}
}
#else
extern "C" {
    Gesture* createGesture(EventProcessor* publisher, int regionID) {
		return new Unified2DRotateGesture(publisher, regionID);
	}
}
#endif

using namespace std;

Unified2DRotateGesture::Unified2DRotateGesture(EventProcessor* publisher, 
        int regionID) : UnifiedStandardDynamicGesture(publisher, regionID) {
}

bool Unified2DRotateGesture::processDown(TouchData& touchData) {
    calculateAngleChange();
    return false;
}

bool Unified2DRotateGesture::processMove(TouchData& touchData){
    float angle = calculateAngleChange();
    string name("Unified2DRotateEvent");
    string desc("Unified2DRotateEvent");

    Unified2DRotateEvent e(name, desc, EVENT_TYPE_OTHER, 0, _originalCentroid,
            angle, _newCentroid);
	publishEvent(&e);
    return false;
}

bool Unified2DRotateGesture::processUp(TouchData& touchData){   
    calculateAngleChange();
	return false;
}

float Unified2DRotateGesture::calculateAngleChange() {
    float retAngle = 0.0;
    map<int, TouchData>::iterator it;
    
    for (it = _knownPoints.begin(); it != _knownPoints.end(); it++) {
        // Get this point's touch data.
        TouchData* data = &it->second;
        
        // Find the angle of this touch point from the centroid.
        float x = _newCentroid[0] - data->getX();
        float y = _newCentroid[1] - data->getY();
        float angle = atan2(y, x);
        
        // Calculate the difference in angle.
        data->setAngle(angle);
        float deltaAngle = data->getAngle() - data->getOldAngle();
        
        // Normalize the change in angle.
        if (deltaAngle > PI) {
            deltaAngle -= 2 * PI;
        } else if (deltaAngle < -PI) {
            deltaAngle += 2 * PI;
        }
        
        retAngle += deltaAngle;
    }
    // Get the average angle.
    retAngle = retAngle / _knownPoints.size();
    return retAngle;
}
