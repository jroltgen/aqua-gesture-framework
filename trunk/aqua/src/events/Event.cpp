/**
 * Event.cpp
 *
 * Functions as the base class for all events and maintains information that is 
 * common to all events.  Though some events may not utilize these attributes, 
 * this information is stored here anyway to help facilitate the desired 
 * “Unified Event Architecture”
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
#include "Event.h"

using namespace std;

Event::Event(char* eventData) {
    int i;
    char* ptr = eventData;
    
    // Get the name - this should read up to a null character.
    _name = ptr;
    ptr += (_name.length() + 1);
        
    // Get the description - this should behave similarly.
    _description = ptr;
    ptr += (_description.length() + 1);
        
    // Get the type.
    _type = *ptr;
    ptr++;
        
    // Get the ID.
    memcpy(&_id, ptr, 4);
    ptr += 4;
    
    // Get the location.
    memcpy(_location, ptr, 12);
        
    // Swap endianness if needed.
    if (EndianConverter::isLittleEndian()) {
            
        _id = EndianConverter::swapIntEndian(_id);
        for(i = 0; i < 3; i++) {
            _location[i] = EndianConverter::swapFloatEndian(_location[i]);
        }
    }
    
    //printf("Event received: %s %s - ID: %d, Type: %d\n", _name.c_str(), _description.c_str(), _id, _type);
}
    
Event::Event(std::string& name, std::string& desc, char type, 
        int id, float* location) {
    _name = name;
    _description = desc;
    _type = type;
    _id = id;
    _location[0] = location[0];
    _location[1] = location[1];
    _location[2] = location[2];
}
    
Event::~Event() {}
    
/**
 * Serializes this event's data.
 */
char* Event::serialize(short& outLength) {
    int i;
    short myLength = (_name.length() + _description.length() + 2) + 17;
        
    short subclassLength = 0;
    char* subclassData = serializeData(subclassLength);
    
    outLength = myLength + subclassLength;
        
    int tempInt;
    float tempFloat;
        
    char* ret = new char[myLength + subclassLength];
    char* bufferPtr = ret;
        
    // Name
    memcpy(bufferPtr, _name.c_str(), _name.length() + 1);
    bufferPtr += _name.length() + 1;
        
    // Description
    memcpy(bufferPtr, _description.c_str(), _description.length() + 1);
    bufferPtr += _description.length() + 1;
        
    // Type
    *bufferPtr = _type;
    bufferPtr++;
        
    // ID
    tempInt = _id;
    if (EndianConverter::isLittleEndian()) {
        tempInt = EndianConverter::swapIntEndian(tempInt);
    }
    memcpy(bufferPtr, &tempInt, 4);
    bufferPtr += 4;
        
    // Location
    for (i = 0; i < 3; i++) {
        tempFloat = _location[i];
        if (EndianConverter::isLittleEndian()) {
            tempFloat = EndianConverter::swapFloatEndian(tempFloat);
        }
        memcpy(bufferPtr, &tempFloat, 4);
        bufferPtr += 4;
    }
        
    // Subclass data
    memcpy(bufferPtr, subclassData, subclassLength);
        
    delete subclassData;
    return ret;
};
    
void Event::setID(int id) {
    _id = id;
};
    
std::string Event::getName() { 
    return _name; 
};

std::string Event::getDesc() { 
    return _description; 
};

float Event::getX() { 
    return _location[0]; 
};

float Event::getY() { 
    return _location[1]; 
};

float Event::getZ() {  
    return _location[2]; 
};

char Event::getType() { 
    return _type;
};

int Event::getID() {    
    return _id;  
};
