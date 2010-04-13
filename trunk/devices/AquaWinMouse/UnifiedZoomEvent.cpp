/**
 * UnifiedZoomEvent.cpp
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
#include "UnifiedZoomEvent.h"

#include <stdio.h>

using namespace std;

UnifiedZoomEvent::UnifiedZoomEvent(char *data) : Event(data) {
    int i;
    int dataPos = (_name.length() + _description.length() + 2 + 17);
    
    memcpy(&_zoomScale, &data[dataPos], 4);
    dataPos += 4;
    
    memcpy(_zoomCenter, &data[dataPos], 12);
    
    // Handle endianness.
    if (EndianConverter::isLittleEndian()) {
        _zoomScale = EndianConverter::swapFloatEndian(_zoomScale);
        for (int i = 0; i < 3; i++) {
            _zoomCenter[i] = EndianConverter::swapFloatEndian(_zoomCenter[i]);
        }
    }
}

UnifiedZoomEvent::UnifiedZoomEvent(string& name, string& desc, 
        char type, int id, float* location, float scale, float* center) : 
        Event(name, desc, type, id, location) {
    
    _zoomScale = scale;
    memcpy(_zoomCenter, center, 12);    
}
            
float UnifiedZoomEvent::getScale() {
    return _zoomScale;
}

float* UnifiedZoomEvent::getCenter() {
    return _zoomCenter;
}
    
/**
 * Constructs a char array with this event's data.  Data:
 *  - 4 bytes : zoom scale (float)
 *  - 4 bytes : zoom center x-coord
 *  - 4 bytes : zoom center y-coord
 *  - 4 bytes : zoom center z-coord
 */
char* UnifiedZoomEvent::serializeData(short& outLength) {
    
    outLength = UNIFIEDZOOMEVENT_DATA_LENGTH;
    
    float tempScale, tempX, tempY, tempZ;
    tempScale = _zoomScale;
    // zoom center
    tempX = _zoomCenter[0];
    tempY = _zoomCenter[1];
    tempZ = _zoomCenter[2];
    
    if (EndianConverter::isLittleEndian()) {
        tempScale = EndianConverter::swapFloatEndian(tempScale);
        tempX = EndianConverter::swapFloatEndian(tempX);
        tempY = EndianConverter::swapFloatEndian(tempY);
        tempZ = EndianConverter::swapFloatEndian(tempZ);
    }
    memcpy(_dataBuffer + 0, &tempScale, 4);
    memcpy(_dataBuffer + 4, &tempX, 4);
    memcpy(_dataBuffer + 8, &tempY, 4);
    memcpy(_dataBuffer + 12, &tempZ, 4);
    
    return _dataBuffer;

}