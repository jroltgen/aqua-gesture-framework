/**
 * GestureServer.cpp
 * 
 * This is the main class for Aqua.  It listens for incoming connections from 
 * devices.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include "GestureServer.h"

#include "InputDeviceConnection.h"
#include "ClientConnection.h"

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
        _gestureEngines[i]->processEvent(event);
    }
    return false;
}

/**
 * Removes and deletes a gesture engine from this list.  The caller is still
 * responsible for removing itself (committing suicide) because if we do it
 * here, there will be nowhere for this function to return to.
 */
void GestureServer::removeGestureEngine(GestureEngine* engineToRemove) {
    unsigned int i;

    for (i = 0; i < _gestureEngines.size(); i++) {
        if (engineToRemove == _gestureEngines[i]) {
            _gestureEngines.erase(_gestureEngines.begin() + i);
        }
    }
}

/** 
 * Create a new inputdeviceConnection
 */
void GestureServer::createInputDeviceConnection(AquaSocket inputSocket) {
    InputDeviceConnection* id = new InputDeviceConnection(inputSocket, this, 
            _nextInputDeviceID++);
    id->run();
}

/**
 * Create a new ClientConnection, and the asociated GestureEngine.
 */
void GestureServer::createClientConnection(AquaSocket clientSocket) {
    ClientConnection* cc = new ClientConnection(clientSocket);
    GestureEngine* ge = new GestureEngine(cc);
    ge->init();
    // TODO catch possible exception.
    _gestureEngines.push_back(ge);
}    

/**
 * Binds the socket
 */
void GestureServer::acceptConnections() {

    AquaSocket  clientSocket;
    int     iResult;
    char    recvType;

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
            if (recvType == INPUT_DEVICE_TYPE) {
                createInputDeviceConnection(clientSocket);
            } else if (recvType == CLIENT_TYPE) {
                printf("[GestureServer] Creating client...\n");
                // TODO create client connection.
            }
        } else if (iResult = 0) {
            printf("[GestureServer] Connecton closing.\n");
        } else if (iResult == AQUASOCKET_RES_ERROR) {
            printf("[GestureServer] Recv failed.\n");
            return;
        } 
    } while (iResult > 0);
}
