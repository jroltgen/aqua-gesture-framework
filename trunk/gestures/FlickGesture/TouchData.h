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

#include <stdio.h>
#include <string>

class TouchData {

// Attributes
private:
    float _distance;    // Used in zoom processing.
    float _oldDistance; // Used in zoom processing.
    float _angle;       // Used in rotate processing.
    float _oldAngle;    // Used in rotate processing.
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
        _distance = 0.0;
        _oldDistance = 0.0;
        _angle = 0.0;
        _oldAngle = 0.0;
    }
    
	TouchData(float* loc) {
        int i;
        for (i = 0; i < 3; i++) {
            _location[i] = loc[i];
            _oldLocation[i] = 0.0;
        }
        _distance = 0.0;
        _oldDistance = 0.0;
        _angle = 0.0;
        _oldAngle = 0.0;
    }
    /*
    void printData() {
        printf("Data:\n");
        printf("Location: %f, %f, %f\n", _location[0], _location[1], _location[2]);
        printf("Location: %f, %f, %f\n", _oldLocation[0], _oldLocation[1], _oldLocation[2]);
        printf("Distance: %f\n", _distance);
        printf("Old Distance: %f\n", _oldDistance);
    }
    */
    
    void setLocation(const float *newLocation) {
        int i;
        for (i = 0; i < 3; i++) {
            _oldLocation[i] = _location[i];
            _location[i] = newLocation[i];
        }
    }
    
    void setDistance(float newDistance) {
        _oldDistance = _distance;
        _distance = newDistance;
    }
    
    void setAngle(float newAngle) {
        _oldAngle = _angle;
        _angle = newAngle;
    }
    
    float getOldAngle() {
        return _oldAngle;
    }
    
    float getAngle() {
        return _angle;
    }
	
    float* getOldLocation()	{
        return _oldLocation;
    }
    
	float* getLocation() {
        return _location;
    }
    
    float getOldDistance() {
        return _oldDistance;
    }
    
    float getDistance() {
        return _distance;
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
        return _oldLocation[0];
    }
    
    float getOldY() {
        return _oldLocation[1];
    }
    
    float getOldZ() {
        return _oldLocation[2];
    }
};
#endif
