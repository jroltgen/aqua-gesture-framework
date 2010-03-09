/**
 * GestureServer.cpp
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
#include "GestureServer.h"

#include "ClientConnection.h"
#include "input/InputDeviceConnection.h"
#include "utils/AquaException.h"

using namespace std;

GestureServer::GestureServer() {
    _nextInputDeviceID = 0;
}

/**
 * Begins listening for connections from applications / devices.
 */
void GestureServer::run() {
    char *hostName = "localhost";
    char *port = LISTEN_PORT;
    if (_listenSocket.bind(hostName, LISTEN_PORT) != AQUASOCKET_RES_OK) {
        printf("[GestureServer] Error: could not start sockets.\n");
        return;
    }
    acceptConnections();
}

/**
 * Process an event received.  For the gesture server, it just forwards it
 * to all of the gesture engines.  Gesture engines are not allowed to "consume"
 * events themselves, so we don't do anything with the return value.
 */
bool GestureServer::processEvent(Event* event) {
    unsigned int i;
    for (i = 0; i < _gestureEngines.size(); i++) {
        // TODO catch exception and remove.
        try {
            _gestureEngines[i]->processEvent(event);
        } catch (AquaException e) {
            if (e.getType() == AQUA_SOCKET_EXCEPTION) {
                printf("%s\n", e.getMessage().c_str());
                removeGestureEngine(_gestureEngines[i]);
            }
        }
    }
    return false;
}

/**
 * Removes and deletes a gesture engine from this list.
 */
void GestureServer::removeGestureEngine(GestureEngine* engineToRemove) {
    unsigned int i;

    for (i = 0; i < _gestureEngines.size(); i++) {
        if (engineToRemove == _gestureEngines[i]) {
            _gestureEngines.erase(_gestureEngines.begin() + i);
            printf("[GestureServer] App connection destroyed.\n");
            delete engineToRemove;
        }
    }
}

/** 
 * Create a new inputdeviceConnection
 */
void GestureServer::createInputDeviceConnection(string protocolName, 
        AquaSocket inputSocket) {
    InputDeviceConnection* id = new InputDeviceConnection(protocolName, 
            inputSocket, this, _nextInputDeviceID++);
    id->run();
}

/**
 * Create a new ClientConnection, and the asociated GestureEngine.
 */
void GestureServer::createClientConnection(AquaSocket clientSocket) {
    ClientConnection* cc = new ClientConnection(clientSocket);
    GestureEngine* ge = new GestureEngine(cc);
    
    try {
        ge->init();
    } catch (AquaException e) {
        if (e.getType() == AQUA_SOCKET_EXCEPTION) {
            printf("%s\n", e.getMessage().c_str());
            removeGestureEngine(ge);
        }
    }
    _gestureEngines.push_back(ge);
    printf("[GestureServer] New app connection created.\n");
}    

/**
 * Binds the socket
 */
void GestureServer::acceptConnections() {

    AquaSocket  clientSocket;
    int     iResult;
    char    recvType;
    
    string aquaName("Aqua");
    string sparshName("Sparsh");

    printf("[GestureServer] Accepting connections.\n");
    do {  
        if (_listenSocket.listen() == AQUASOCKET_RES_ERROR) {
            printf("[GestureServer] Error in listen.\n");
            break;
        }
        
        // Accept a client socket
        clientSocket = _listenSocket.accept();
        if (!clientSocket.isValid()) {
            printf("[GestureServer] Accept failed.\n");
            break;
        }
        
        iResult = clientSocket.recv(&recvType, 1);
        if (iResult > 0) {
            if (recvType == AQUA_INPUT_DEVICE_TYPE) {
                createInputDeviceConnection(aquaName, clientSocket);
            } else if (recvType == SPARSH_INPUT_DEVICE_TYPE) {
                createInputDeviceConnection(sparshName, clientSocket);
            } else if (recvType == AQUA_CLIENT_TYPE) {
                createClientConnection(clientSocket);
            }
        } else if (iResult = 0) {
            printf("[GestureServer] Connecton closing.\n");
        } else if (iResult == AQUASOCKET_RES_ERROR) {
            printf("[GestureServer] Recv failed.\n");
            return;
        } 
    } while (true);
}
