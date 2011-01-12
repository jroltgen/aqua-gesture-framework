/**
 * UnifiedZoomEvent.h
 *
 * An event to use for sending zoom-related information.
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
#ifndef _UNIFIEDZOOMEVENT_H_
#define _UNIFIEDZOOMEVENT_H_

#define UNIFIEDZOOMEVENT_DATA_LENGTH 16

#include <string>

#include "Event.h"

class UnifiedZoomEvent : public Event {

// Attributes
private:
    float _zoomScale;
    float _zoomCenter[3];
    char  _dataBuffer[UNIFIEDZOOMEVENT_DATA_LENGTH];
    
// Methods
public:
    UnifiedZoomEvent(char *data);
    UnifiedZoomEvent(std::string& name, std::string& desc, char type, int id, 
            float* location, float scale, float* center);
            
    float  getScale();
    float* getCenter();
    
private:
    virtual char* serializeData(short& outLength);
    
};
#endif
