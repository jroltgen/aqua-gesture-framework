/**
 * HandIDTouchEvent.cpp
 *
 * This represents a touch event that also has a hand ID associated with it.
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
#include <stdio.h>
 
#include "HandIDTouchEvent.h"

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Event* createEvent(char* data) {
		return new HandIDTouchEvent(data);
	}
}
#else
extern "C" {
    Event* createEvent(char* data) {
		return new HandIDTouchEvent(data);
	}
}
#endif

HandIDTouchEvent::HandIDTouchEvent(char* data) : Event(data) {
    
	int dataPos = (_name.length() + _description.length() + 2 + 17);
    
    memcpy(&_handID, &data[dataPos], 4);
    
    // Handle endianness.
    if (EndianConverter::isLittleEndian()) {
        _handID = EndianConverter::swapIntEndian(_handID);
    }
}

HandIDTouchEvent::HandIDTouchEvent(std::string& name, std::string& desc, 
		char type, int id, float* location, int handID) : Event(name, desc, 
		type, id, location) {
	_handID = handID;
}

HandIDTouchEvent::~HandIDTouchEvent() {
}

char* HandIDTouchEvent::serializeData(short &lengthOut) {
    lengthOut = HANDIDTOUCHEVENT_DATA_LENGTH;
    
    int tempHandID = _handID;

    if (EndianConverter::isLittleEndian()) {
		tempHandID = EndianConverter::swapIntEndian(_handID);
    }
    
    memcpy(_dataBuffer, &tempHandID, 4);
    
    return _dataBuffer;
}
