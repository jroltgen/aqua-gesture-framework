/**
 * UnifiedEvent.cpp
 *
 * This class is the base class for most simple events.
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
#include <stdio.h>
 
#include "UnifiedEvent.h"

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Event* createEvent(char* data) {
		return new UnifiedEvent(data);
	}
}
#else
// TODO linux support
#endif

UnifiedEvent::UnifiedEvent(char* data) : Event(data) {
    // Here is where we would use data to initiailize our members, but
    // we don't have any, so we don't have to do anything.
}

UnifiedEvent::UnifiedEvent(std::string& name, std::string& desc, char type, 
        int id, float* location) : Event(name, desc, type, id, location) {
    // Again, we don't have to initialize any of our own members.
}

UnifiedEvent::~UnifiedEvent() {
}

char* UnifiedEvent::serializeData(short &lengthOut) {
    lengthOut = 0;
    return new char[0];
}
