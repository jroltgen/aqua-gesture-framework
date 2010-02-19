/**
 * TouchData.h
 *
 * This class represents "Touch Data" or universal information used for
 * standard dynamic gestures.
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
#ifndef _TOUCHDATA_H_
#define _TOUCHDATA_H_

#include <string>

class TouchData {

// Attributes
private:
    float _location[3];
    float _oldLocation[3];
    
// Methods
public:
    TouchData() {
        int i;
        for (i = 0; i < 3; i++) {
            _location[i] = 0.0;
            _oldLocation[i] = 0.0;
        }
    }
    
	TouchData(float* loc) {
        int i;
        for (i = 0; i < 3; i++) {
            _location[i] = loc[i];
            _oldLocation[i] = 0.0;
        }
    }
    
    void setLocation(const float *newLocation) {
        int i;
        for (i = 0; i < 3; i++) {
            _oldLocation[i] = _location[i];
            _location[i] = newLocation[i];
        }
    }
	
    float* getOldLocation()	{
        return _oldLocation;
    }
    
	float* getLocation() {
        return _location;
    }
    
    float getX() {
        return _location[0];
    }
    
    float getY() {
        return _location[1];
    }
    
    float getZ() {
        return _location[2];
    }
    
    float getOldX() {
        return _location[0];
    }
    
    float getOldY() {
        return _location[1];
    }
    
    float getOldZ() {
        return _location[2];
    }
};
#endif
