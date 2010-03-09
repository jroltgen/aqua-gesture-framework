/**
 * InputProtocol.h
 * 
 * Provides the interface for implementing an input device protocol.
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
#ifndef _INPUTPROTOCOL_H_
#define _INPUTPROTOCOL_H_
 
#include "../utils/AquaSocket.h" 
#include "../events/Event.h"

#define INPUT_BUFFER_SIZE 65536


class InputProtocol {

// Attributes
protected:
    /**
     * Buffer for incoming data ->incoming data limited by this size.
     */
    char receiveBuffer[INPUT_BUFFER_SIZE];
    
    /**
     * Socket that communicates with the input device.
     */
    AquaSocket _socket;

// Methods
public:
    InputProtocol(AquaSocket socket) {
        _socket = socket;
    }
    
    virtual bool getNextEvent(Event* receivedEvent) = 0;
};
#endif
