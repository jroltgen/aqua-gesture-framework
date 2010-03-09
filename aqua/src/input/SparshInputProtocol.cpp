/**
 * SparshInputProtocol.cpp
 * 
 * Implements the communication protocol for talking with Aqua-enabled
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
#include <string>
 
#include "SparshInputProtocol.h"
#include "../events/EventFactory.h"
#include "../utils/EndianConverter.h"
#include "../events/Event.h"
#include "../events/UnifiedEvent.h"

#define SPARSH_BIRTH 0
#define SPARSH_DEATH 1
#define SPARSH_MOVE  2

using namespace std;
 
SparshInputProtocol::SparshInputProtocol(AquaSocket socket) : 
        InputProtocol(socket) {
}


bool SparshInputProtocol::getNextEvent(Event* &receivedEvent) {
 
    // If the queue is empty, then read from the input device.  Otherwise,
    // we just pop the next element out of the queue and return it.
    if (_eventQueue.empty()) {
        
        int             iResult;
        int             i;
        int             numTouchPoints;
        unsigned short  msgLength;
        int             remaining;
        string          eventName;
        int             id;
        float           location[3];
        char            touchState;
        string          unifiedEventName("UnifiedEvent");
        string          unifiedEventDesc("SparshTouchEvent");
        
        // Read the number of events.
        iResult = _socket.recv(receiveBuffer, 4);
        if (iResult != 4) {
            printf("[InputDeviceConnection] Socket receive error.\n");
            return false;
        }
        memcpy(&numTouchPoints, receiveBuffer, 4);
        if (EndianConverter::isLittleEndian()) {
            numTouchPoints = EndianConverter::swapIntEndian(numTouchPoints);
        }
 
        // Iterate through the events
        for (i = 0; i < numTouchPoints; i++) {
            // Get all data
            iResult = _socket.recv(receiveBuffer, 13);
            if (iResult != 13) {
                printf("[InputDeviceConnection] Socket receive error.\n");
                return false;
            }
            
            // Set ID, x, y, type
            memcpy(&id,          &receiveBuffer[0],  4);
            memcpy(&location[0], &receiveBuffer[4],  4);
            memcpy(&location[1], &receiveBuffer[8],  4);
            memcpy(&touchState,  &receiveBuffer[12], 1);
            
            // Swap endianness
            if (EndianConverter::isLittleEndian()) {
                id = EndianConverter::swapIntEndian(id);
                location[0]  = EndianConverter::swapFloatEndian(location[0]);
                location[1] = EndianConverter::swapFloatEndian(location[1]);
            }
            location[2] = 0;
            
            // Set touchState
            switch(touchState) {
            case SPARSH_BIRTH:
                touchState = EVENT_TYPE_DOWN;
                break;
            case SPARSH_DEATH:
                touchState = EVENT_TYPE_UP;
                break;
            case SPARSH_MOVE:
                touchState = EVENT_TYPE_MOVE;
                break;
            default:
                printf("[InputDeviceConnection] Unrecognized type: %d\n", 
                        touchState);
                return false;
            }
            Event* newEvent = new UnifiedEvent(unifiedEventName,
					unifiedEventDesc, touchState, id, location);
                    
            _eventQueue.push(newEvent);
        }
    }
    
    // Retrive the next element from the queue.
    receivedEvent = _eventQueue.front();
    _eventQueue.pop();
    return true;
}
