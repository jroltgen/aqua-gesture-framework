/**
 * SimpleEvent.h
 *
 * This class is the base class for most simple events.
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
#ifndef SIMPLEEVENT_H_
#define SIMPLEEVENT_H_

#include "Event.h"

class SimpleEvent : public Event {
    
// Attributes

// Methods
public:
    SimpleEvent(char* data);
    SimpleEvent(std::string& name, std::string& desc, char type, int id, 
            float* location);
    virtual ~SimpleEvent();

private:
    virtual char* serializeData(short& lengthOut);
};

#endif
