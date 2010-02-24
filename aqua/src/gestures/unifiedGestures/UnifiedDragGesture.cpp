/**
 * UnifiedDragGesture.cpp
 *
 * Processes a drag.
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
// TODO linux support
#endif

using namespace std;

UnifiedDragGesture::UnifiedDragGesture(EventProcessor* publisher, 
        int regionID) : UnifiedStandardDynamicGesture(publisher, regionID) {
}

bool UnifiedDragGesture::processDown(TouchData& touchData) {
    UnifiedDragEvent e(string("UnifiedDragEvent"), string("UnifiedDragEvent"), 
            EVENT_TYPE_OTHER, 0, _originalCentroid, 
            _newCentroid[0] - _oldCentroid[0],
            _newCentroid[1] - _oldCentroid[1],
            _newCentroid[2] - _oldCentroid[2]);
    //publishEvent(&e);
    return false;
}

bool UnifiedDragGesture::processMove(TouchData& touchData){
	UnifiedDragEvent e(string("UnifiedDragEvent"), string("UnifiedDragEvent"), 
            EVENT_TYPE_OTHER, 0, _originalCentroid, 
            _newCentroid[0] - _oldCentroid[0],
            _newCentroid[1] - _oldCentroid[1],
            _newCentroid[2] - _oldCentroid[2]);
    publishEvent(&e);
    //printInfo();
    return false;
}

bool UnifiedDragGesture::processUp(TouchData& touchData){
	UnifiedDragEvent e(string("UnifiedDragEvent"), string("UnifiedDragEvent"), 
            EVENT_TYPE_OTHER, 0, _originalCentroid, 
            _newCentroid[0] - _oldCentroid[0],
            _newCentroid[1] - _oldCentroid[1],
            _newCentroid[2] - _oldCentroid[2]);
    publishEvent(&e);
    //printInfo();
	return false;
}

void UnifiedDragGesture::printInfo() {
    int i;
    
    printf("Known points: %d\n", _knownPoints.size());
        std::map<int, TouchData> _knownPoints;
    
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
