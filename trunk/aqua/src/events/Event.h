/**
 * Event.h
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
protected:
    std::string _name;
    std::string _description;
    char        _type;
    int         _id;
    float       _location[3];

// Methods
public:
    Event(char* eventData);    
    Event(std::string& name, std::string& desc, char type, 
            int id, float* location);
    virtual ~Event();
    
    char* serialize(short& outLength);
    
    void        setID(int id);
    
    std::string getName();
    std::string getDesc();
    float*      getLocation();
    float       getX();
    float       getY();
    float       getZ();
    char        getType();
    int         getID();
    
private:
    virtual char* serializeData(short &lengthOut) = 0;
    
};

#endif
