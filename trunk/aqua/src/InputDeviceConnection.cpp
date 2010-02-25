/**
 * InputDeviceConnection.cpp
 * 
 * Communicates with each input device – there should be one 
 * InputDeviceConnection for every input device that is currently connected 
 * to the gesture server.
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

#include "events/EventFactory.h"
#include "InputDeviceConnection.h"
#include "utils/EndianConverter.h"

using namespace std;

/**
 * Creates a new input device connection object.  This constructor simply
 * initializes the member variables.  If you want your input device to actually
 * do anything, you will have to call run().
 */
InputDeviceConnection::InputDeviceConnection(AquaSocket theSocket, 
        GestureServer* theServer, int id) {
    _socket = theSocket;
    _server = theServer;
    _id = id;
}
    
/**
 * Starts a new thread that will receive events from the input device.
 */
void InputDeviceConnection::run() {
    #ifdef _WIN32
    CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->runReadEvents, 
            (void *)this, 0, NULL);
    #else
    // LS
    #endif
}

/**
 * Reads events from the input stream.
 */
void InputDeviceConnection::readEvents() {
    printf("[InputDeviceConnection] ID(%d) Connected\n", _id);
    
    bool ok;
    do {
        ok = readEvent();
    } while (ok);
    
    printf("[InputDeviceConnection] ID(%d) Disconnected\n", _id);
    
    // Delete myself, because no one has a reference to me.
    delete this;
}

/**
 * Reads an event from the socket.  First read the length, then proceed by 
 * reading one character at a time until a null character is reached, then 
 * assembling the string.  This is the name of the event.
 */
bool InputDeviceConnection::readEvent() {
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
    Event* receivedEvent = EventFactory::getInstance()->createEvent(
            eventName, receiveBuffer);
            
    // Add our constant to the eventID to avoid conflicting event IDs.
    receivedEvent->setID(_id * 100000 + receivedEvent->getID());
      
    _server->processEvent(receivedEvent);
    
    delete receivedEvent;
    return true;
}

#ifdef _WIN32
int InputDeviceConnection::runReadEvents(void* pThis) {
    ((InputDeviceConnection*)pThis)->readEvents();
    return 0;
}
#else
// LS (if necessary)
#endif
