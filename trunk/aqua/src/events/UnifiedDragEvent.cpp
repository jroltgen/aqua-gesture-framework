/**
 * UnifiedDragEvent.cpp
 *
 * An event to use for sending drag-related information.
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
#include "../utils/EndianConverter.h"
#include "UnifiedDragEvent.h"

using namespace std;

UnifiedDragEvent::UnifiedDragEvent(string& name, string& desc, char type, 
        int id, float* location, float dx, float dy, float dz) : 
        Event(name, desc, type, id, location) {
	_dx = 0.0;
	_dy = 0.0;
    _dz = 0.0;
}

UnifiedDragEvent::UnifiedDragEvent(char *data) : Event(data) {
    int dataPos = (_name.length() + _description.length() + 2 + 17);
    
    _dx = (float)data[dataPos];
    _dy = (float)data[dataPos + 4];
    _dz = (float)data[dataPos + 8];
    
    // Handle endianness.
    if (EndianConverter::isLittleEndian()) {
        _dx = EndianConverter::swapFloatEndian(_dx);
        _dy = EndianConverter::swapFloatEndian(_dy);
        _dz = EndianConverter::swapFloatEndian(_dz);
    }
}

float UnifiedDragEvent::getdx(){
    return _dx;
}

float UnifiedDragEvent::getdy() {
    return _dy;
}

float UnifiedDragEvent::getdz() {
    return _dz;
}

/**
 * Constructs the data packet with this event data. Message format for this
 * event:
 * 		- 4 bytes	: dx
 *      - 4 bytes   : dy
 *      - 4 bytes   : dz
 */
char* UnifiedDragEvent::serializeData(short& lengthOut) {

	char* buffer = new char[12];
	lengthOut = 12;
    float tempX, tempY, tempZ;
	
    // dx, dy, dz
    tempX = _dx;
    tempY = _dy;
    tempZ = _dz;
    
    if (EndianConverter::isLittleEndian()) {
        tempX = EndianConverter::swapFloatEndian(tempX);
        tempY = EndianConverter::swapFloatEndian(tempY);
        tempZ = EndianConverter::swapFloatEndian(tempZ);
    }
    memcpy(buffer + 0, &tempX, 4);
    memcpy(buffer + 4, &tempY, 4);
    memcpy(buffer + 8, &tempZ, 4);
    
    return buffer; 
}
