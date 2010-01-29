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
#dfeine EVENT_TYPE_OTHER 4

#include <string>

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
    Event(char* eventData);
    virtual ~Event();
    
    char* serialize() {
        // TODO implement this method
        return NULL;
    }
    
    void  setID(int id) {
        _id = id;
    }
    
    char getType() { return _type; }
    int  getID()   { return _id;   }
    
private:
    virtual char* serializeData(int* outLength) = 0;
    
};

#endif

