/**
 * Unified2DRotateEvent.cpp
 *
 * An event to use for sending zoom-related information.
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
#include "Unified2DRotateEvent.h"

#include <stdio.h>

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Event* createEvent(char* data) {
		return new Unified2DRotateEvent(data);
	}
}
#else
// TODO linux support
#endif

using namespace std;

Unified2DRotateEvent::Unified2DRotateEvent(char *data) : Event(data) {
    int i;
    int dataPos = (_name.length() + _description.length() + 2 + 17);
    
    memcpy(&_rotateAngle, &data[dataPos], 4);
    dataPos += 4;
    
    memcpy(_rotateCenter, &data[dataPos], 8);
    
    // Handle endianness.
    if (EndianConverter::isLittleEndian()) {
        for (int i = 0; i < 2; i++) {
            _rotateCenter[i] = EndianConverter::swapFloatEndian(_rotateCenter[i]);
        }
    }
}

Unified2DRotateEvent::Unified2DRotateEvent(string& name, string& desc, 
        char type, int id, float* location, float angle, float* center) : 
        Event(name, desc, type, id, location) {
    
    _rotateAngle = angle;
    memcpy(_rotateCenter, center, 8);    
}
            
float Unified2DRotateEvent::getAngle() {
    return _rotateAngle;
}

float* Unified2DRotateEvent::getCenter() {
    return _rotateCenter;
}
    
/**
 * Constructs a char array with this event's data.  Data:
 *  - 4 bytes : zoom scale (float)
 *  - 4 bytes : zoom center x-coord
 *  - 4 bytes : zoom center y-coord
 *  - 4 bytes : zoom center z-coord
 */
char* Unified2DRotateEvent::serializeData(short& outLength) {
    
    outLength = UNIFIED2DROTATEEVENT_DATA_LENGTH;
    
    float tempAngle, tempX, tempY;
	
    tempAngle = _rotateAngle;
    
    // rotate center
    tempX = _rotateCenter[0];
    tempY = _rotateCenter[1];
    
    if (EndianConverter::isLittleEndian()) {
        tempAngle = EndianConverter::swapFloatEndian(tempAngle);
        tempX = EndianConverter::swapFloatEndian(tempX);
        tempY = EndianConverter::swapFloatEndian(tempY);
    }
    
    memcpy(_dataBuffer + 0, &tempAngle, 4);
    memcpy(_dataBuffer + 4, &tempX, 4);
    memcpy(_dataBuffer + 8, &tempY, 4);
    
    return _dataBuffer;

}