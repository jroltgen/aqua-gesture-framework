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

using namespace std;

UnifiedDragGesture::UnifiedDragGesture(EventProcessor* publisher, 
        int regionID) : UnifiedStandardDynamicGesture(publisher, regionID) {
    
    _offsetExists = false;
}

bool UnifiedDragGesture::processDown(TouchData touchData) {
	int i;
    
    if(_offsetExists == false){
        _offsetExists = true;
        for (i = 0; i < 3; i++) {
            _offset[i] = 0.0;
            _offsetCentroid[i] = 0.0;
        }
    } else {
        adjustOffset();
    }
    return false;
}

bool UnifiedDragGesture::processMove(TouchData touchData){
		printf("Processing move\n");
		////debug_msg();
		vector<Event*> events;
		adjustOffset();
		updateOffsetCentroid();
		events.push_back(new DragEvent(_offsetCentroid.getX(), _offsetCentroid.getY()));
		return events;
	}

bool UnifiedDragGesture::processDeath(TouchData touchData){
		////debug_msg();
		if(_knownPoints.size() == 0){
			b_offset = false;
			_offset = Location(0,0);
			_offsetCentroid = Location(0,0);
		}else{
			adjustOffset();
		}
		return vector<Event*>();
	}

void UnifiedDragGesture::adjustOffset() {
    int i;
    
    for (i = 0; i < 3; i++) {
        _offset[i] = _newCentroid[i] - _oldCentroid[i];
        
		////debug_msg();
		_offset = Location (_newCentroid.getX() - _oldCentroid.getX(),
			_newCentroid.getY() - _oldCentroid.getY());
	}

void UnifiedDragGesture::updateOffsetCentroid(){
		////debug_msg();
		float x = _newCentroid.getX() - _offset.getX();
		float y = _newCentroid.getY() - _offset.getY();
		_offsetCentroid	= Location(x,y);
	}
}
