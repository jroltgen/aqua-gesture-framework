/**
 * SparshInputProtocol.h
 * 
 * Implements the communication protocol for talking with SparshUI-enabled
 * input devices.
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
#ifndef _SPARSHINPUTPROTOCOL_H_
#define _SPARSHINPUTPROTOCOL_H_

#include <queue>

#include "InputProtocol.h"

class SparshInputProtocol : public InputProtocol {

// Attributes
private:
    std::queue<Event*> _eventQueue;

// Methods
public:
    SparshInputProtocol(AquaSocket socket);
    virtual bool getNextEvent(Event* &receivedEvent);
};
#endif
