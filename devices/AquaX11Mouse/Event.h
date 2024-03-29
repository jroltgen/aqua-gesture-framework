/**
 * Event.h
 *
 * Functions as the base class for all events and maintains information that is 
 * common to all events.  Though some events may not utilize these attributes, 
 * this information is stored here anyway to help facilitate the desired 
 * “Unified Event Architecture”
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010 Iowa State University.
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
#ifndef _EVENT_H_
#define _EVENT_H_

// TODO define these somewhere else (maybe)
#define EVENT_TYPE_DOWN  0
#define EVENT_TYPE_MOVE  1
#define EVENT_TYPE_UP    2
#define EVENT_TYPE_HOVER 3
#define EVENT_TYPE_OTHER 4

#include <string>

#include "utils/EndianConverter.h"

class Event {

// Attributes
protected:
    std::string _name;
    std::string _description;
    char        _type;
    int         _id;
    float       _location[3];

// Methods
public:
    Event(char* eventData) {
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
        
        //printf("Event rcvd: %s %s - ID: %d, Type: %d\n\tLoc: %f, %f, %f\n", 
		//		_name.c_str(), _description.c_str(), _id, _type,
		//		_location[0], _location[1], _location[2]);
    }
    
    Event(std::string& name, std::string& desc, char type, 
            int id, float* location) {
        _name = name;
        _description = desc;
        _type = type;
        _id = id;
        _location[0] = location[0];
        _location[1] = location[1];
        _location[2] = location[2];
    }
    
    virtual ~Event() {}
    
    char* serialize(short& outLength) {
        int i;
        short myLength;
        char* subclassData;
        short subclassLength;
        
        subclassData = serializeData(subclassLength);
        
        myLength = (_name.length() + _description.length() + 2) + 17;
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
        return ret;
    }
    
    void setID(int id) { _id = id; };
    std::string getName() { return _name; };
    std::string getDesc() { return _description; };
    float* getLocation() { return _location; };
    float getX() { return _location[0]; };
    float getY() { return _location[1]; };
    float getZ() { return _location[2]; };
    char getType() { return _type; };
    int getID() { return _id; };
        
private:
    virtual char* serializeData(short &lengthOut) = 0;
    
};

#endif
