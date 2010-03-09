/**
 * AquaInputProtocol.cpp
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
 
#include "AquaInputProtocol.h"
#include "../events/EventFactory.h"

using namespace std;
 
AquaInputProtocol::AquaInputProtocol(AquaSocket socket) : 
        InputProtocol(socket) {
}


bool AquaInputProtocol::getNextEvent(Event* &receivedEvent) {
 
    int             iResult;
    unsigned short  msgLength;
    int             remaining;
    string          eventName;
    
    // Read event length.
    iResult = _socket.recv(receiveBuffer, 2);
    if (iResult != 2) {
        printf("[InputDeviceConnection] Socket receive error.\n");
        return false;
    }
    memcpy(&msgLength, receiveBuffer, 2);
    if (EndianConverter::isLittleEndian()) {
        msgLength = EndianConverter::swapShortEndian(msgLength);
    }
    
    iResult = _socket.recv(receiveBuffer, msgLength);
    if (iResult == AQUASOCKET_RES_ERROR) {
		printf("[InputDeviceConnection] Socket receive error.\n");
		return false;
	}
	
    eventName = string(receiveBuffer);
    receivedEvent = EventFactory::getInstance()->createEvent(
            eventName, receiveBuffer);
  
    return true;
}
