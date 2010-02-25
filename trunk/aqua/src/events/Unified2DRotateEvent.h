/**
 * Unified2DRotateEvent.h
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
#ifndef _UNIFIED2DROTATEEVENT_H_
#define _UNIFIED2DROTATEEVENT_H_

#define UNIFIED2DROTATEEVENT_DATA_LENGTH 12

#include <string>

#include "Event.h"

class Unified2DRotateEvent : public Event {

// Attributes
private:
    float _rotateAngle;
    float _rotateCenter[2];
    char  _dataBuffer[UNIFIED2DROTATEEVENT_DATA_LENGTH];
    
// Methods
public:
    Unified2DRotateEvent(char *data);
    Unified2DRotateEvent(std::string& name, std::string& desc, char type, int id, 
            float* location, float angle, float* center);
            
    float  getAngle();
    float* getCenter();
    
private:
    virtual char* serializeData(short& outLength);
    
};
#endif
