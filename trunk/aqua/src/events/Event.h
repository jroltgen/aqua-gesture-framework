/**
 * Event.h
 *
 * Functions as the base class for all events and maintains information that is 
 * common to all events.  Though some events may not utilize these attributes, 
 * this information is stored here anyway to help facilitate the desired 
 * “Unified Event Architecture”
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _EVENT_H_
#define _EVENT_H_

// TODO define these somewhere else (maybe)
#define EVENT_TYPE_DOWN  0
#define EVENT_TYPE_MOVE  1
#define EVENT_TYPE_UP    2
#define EVENT_TYPE_HOVER 3
#define EVENT_TYPE_OTHER 4

#include <stdio.h> // TODO remove
#include <string>

#include "../utils/EndianConverter.h"

class Event {

// Attributes
private:
    std::string _name;
    std::string _description;
    char        _type;
    int         _id;
    float       _location[3];

// Methods
public:
    Event(char* eventData) {
        int i;
        
        // Get the name - this should read up to a null character.
        _name = eventData;
        eventData += (_name.length() + 1);
        
        // Get the description - this should behave similarly.
        _description = eventData;
        eventData += (_name.length() + 1);
        
        // Get the type.
        _type = *eventData;
        eventData++;
        
        // Get the ID.
        memcpy(&_id, eventData, 4);
        eventData++;
    
        // Get the location.
        memcpy(_location, eventData, 12);
        
        // Swap endianness if needed.
        if (EndianConverter::isLittleEndian()) {
            
            _id = EndianConverter::swapIntEndian(_id);
            for(i = 0; i < 3; i++) {
                _location[i] = EndianConverter::swapFloatEndian(_location[i]);
            }
        }
    }
    
    virtual ~Event() {}
    
    /**
     * Serializes this event's data.
     */
    char* serialize() {
        int i;
        int myLength = (_name.length() + _description.length() + 2) + 17;
        
        int   subclassLength = 0;
        char* subclassData = serializeData(subclassLength);
        
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
        memcpy(bufferPtr, &_id, 4);
        if (EndianConverter::isLittleEndian()) {
            *bufferPtr = EndianConverter::swapIntEndian(*bufferPtr);
        }
        bufferPtr += 4;
        
        // Location
        for (i = 0; i < 3; i++) {
            memcpy(bufferPtr, &(_location[i]), 4);
            *bufferPtr = EndianConverter::swapFloatEndian(*bufferPtr);
            bufferPtr += 4;
        }
        
        // Subclass data
        memcpy(bufferPtr, subclassData, subclassLength);
        
        delete subclassData;
        return ret;
    };
    
    void  setID(int id) {
        _id = id;
    };
    
    char getType() { return _type; };
    int  getID()   { return _id;   };
    
private:
    virtual char* serializeData(int &lengthOut) = 0;
    
};

#endif

