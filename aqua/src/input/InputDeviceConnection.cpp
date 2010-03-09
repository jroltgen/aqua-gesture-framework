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
#include <stdio.h>
#include <stdlib.h>

#include "../events/EventFactory.h"
#include "InputDeviceConnection.h"
#include "../utils/EndianConverter.h"
#include "AquaInputProtocol.h"
#include "SparshInputProtocol.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <pthread.h>
#endif

using namespace std;

/**
 * These are the thread-launching methods required by the threading
 * libraries of Windows & linux.
 */
#ifdef _WIN32
int InputDeviceConnection::runReadEvents(void* pThis) {
    ((InputDeviceConnection*)pThis)->readEvents();
    return 0;
}
#else
extern "C" {
	void *runReadEvents(void* pThis) {
		((InputDeviceConnection*)pThis)->readEvents();
	}
}
#endif

/**
 * Creates a new input device connection object.  This constructor simply
 * initializes the member variables.  If you want your input device to actually
 * do anything, you will have to call run().
 */
InputDeviceConnection::InputDeviceConnection(string protocolName, 
        AquaSocket socket, GestureServer* theServer, int id) {
    _server = theServer;
    _id = id;
    
    if (protocolName.compare("Aqua") == 0) {
        _protocol = new AquaInputProtocol(socket);
    } else if (protocolName.compare("Sparsh") == 0) {
        printf("[InputDeviceConnection] Using SPARSH-UI Protocol.\n");
        _protocol = new SparshInputProtocol(socket);
    } else {
        printf("[InputDeviceConnection] ***Error: Protocol not recognized.\n");
    }
}
    
/**
 * Starts a new thread that will receive events from the input device.
 */
void InputDeviceConnection::run() {
    #ifdef _WIN32
    CreateThread(NULL, 0, 
			(unsigned long (__stdcall *)(void*))this->runReadEvents, 
            (void*) this, 0, NULL);
    #else
	pthread_t myThread;
	int rc;

    rc = pthread_create(&myThread, NULL, runReadEvents, (void*) this);
	if (rc != 0) {
 		printf("[InputDeviceConnection] Error creating pthread.\n");
		exit(-1);
	}
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

    Event* receivedEvent = NULL;
    bool result = _protocol->getNextEvent(receivedEvent);
    
    if (result) {
        if (receivedEvent != NULL) {
            receivedEvent->setID(_id * 100000 + receivedEvent->getID());
            _server->processEvent(receivedEvent);
            delete receivedEvent;
        }
    }
    return result;
}


