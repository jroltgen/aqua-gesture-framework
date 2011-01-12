/**
 * HandIDTouchEvent.h
 *
 * This represents a touch event that also has a hand ID associated with it.
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
#ifndef _HANDIDTOUCHEVENT_H_
#define _HANDIDTOUCHEVENT_H_

#define HANDIDTOUCHEVENT_DATA_LENGTH 4

#include "Event.h"

class HandIDTouchEvent : public Event {
    
// Attributes
private:
	int   _handID;
	char  _dataBuffer[HANDIDTOUCHEVENT_DATA_LENGTH];

// Methods
public:
    HandIDTouchEvent(char* data);
    HandIDTouchEvent(std::string& name, std::string& desc, char type, int id, 
            float* location, int handID);
    virtual ~HandIDTouchEvent();

	int getHandID() { return _handID; }

private:
    virtual char* serializeData(short& lengthOut);
};

#endif
