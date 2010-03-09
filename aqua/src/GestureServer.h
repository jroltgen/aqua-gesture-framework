/**
 * GestureServer.h
 * 
 * This is the main class for Aqua.  It listens for incoming connections from 
 * devices.
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
#ifndef _GESTURESERVER_H_
#define _GESTURESERVER_H_

#include <vector>
#include <string>
// Connection port to use
#define LISTEN_PORT "3007"

// Connection types TODO - define these somewhere else if we start using
// a custom protocol.
#define SPARSH_CLIENT_TYPE		  0
#define SPARSH_INPUT_DEVICE_TYPE  1
#define AQUA_INPUT_DEVICE_TYPE    2
#define AQUA_CLIENT_TYPE          3

#include "EventProcessor.h"
#include "GestureEngine.h"
#include "gestures/Gesture.h"
#include "utils/AquaSocket.h"

class GestureServer : public EventProcessor {

// Attributes
private:
    /**
     * This is the server socket that accepts connections from devices.  
     * Client connections and input device connections are made to the same 
     * server socket, and the socket that is created as a result of the 
     * connection attempt is passed to either the InputDeviceConnection or 
     * ClientConnection as a pointer.
     */
    AquaSocket _listenSocket;
    
    /**
     * This is a list of the current gesture engines.  Events received by
     * the InputDeviceConnections will be sent to each gesture engine.
     */
    std::vector<GestureEngine*> _gestureEngines;
    
    /**
     * This is incremented every time a new input device connects and is 
     * passed to the input device constructor – the input device connection 
     * adds a constant proportional to this ID so that events coming in with 
     * the same ID from multiple input devices will not be confused.
     */
    int _nextInputDeviceID;

// Methods
public:
    GestureServer();
    bool processEvent(Event* e);
    void run();
    
private:
    void acceptConnections();
    void createClientConnection(AquaSocket clientSocket);
    void createInputDeviceConnection(std::string protocolname, 
            AquaSocket inputSocket);
    bool initSocket();
    void removeGestureEngine(GestureEngine* engineToRemove);
    
};
#endif
