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
    _listenSocket = INVALID_SOCKET;
    _nextInputDeviceID = 0;
}

/**
 * Begins listening for connections from applications / devices.
 */
void GestureServer::run() {
    if (!initSocket()) {
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
void GestureServer::createInputDeviceConnection(SOCKET inputSocket) {
    InputDeviceConnection* id = new InputDeviceConnection(inputSocket, this, 
            _nextInputDeviceID++);
    id->run();
}

/**
 * Create a new ClientConnection, and the asociated GestureEngine.
 */
void GestureServer::createClientConnection(SOCKET clientSocket) {
    ClientConnection* cc = new ClientConnection(clientSocket);
    GestureEngine* ge = new GestureEngine(cc, this);
    _gestureEngines.push_back(ge);
}    


#ifdef _WIN32
/**
 * Binds the socket
 */
void GestureServer::acceptConnections() {

    SOCKET  clientSocket;
    int     iResult;
    char    recvType;

    printf("[GestureServer] Accepting connections.\n");
    do {  
        // Listen on the socket
        if (listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
            printf("Error at listen(): %ld\n", WSAGetLastError());
            closesocket(_listenSocket);
            WSACleanup();
            break;
        }
        
        // Accept a client socket
        clientSocket = accept(_listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(_listenSocket);
            WSACleanup();
            break;
        }
        
        iResult = recv(clientSocket, &recvType, 1, 0);
        if (iResult > 0) {
            if (recvType == INPUT_DEVICE_TYPE) {
                createInputDeviceConnection(clientSocket);
            } else if (recvType == CLIENT_TYPE) {
                printf("Creating client...\n");
            }
        } else if (iResult = 0) {
            printf("Connecton closing.\n");
        } else {
            printf("Recv failed: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
         
    } while (iResult > 0);

}

/**
 * Initializes the socket library
 */
bool GestureServer::initSocket() {
    WSADATA          data;
    int              iResult;
    struct addrinfo  hints; 
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;   
    // Initialize Winsock
    
    
    iResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (iResult != 0) {
        printf("Error starting WSA\n");
        return false;
    }
    
    // Initialize the socket info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    
    // Resolve the local address and port.
    iResult = getaddrinfo(NULL, LISTEN_PORT, &hints, &result);
    if (iResult != 0) {
        printf("Get addrinfo failed: %d\n", iResult);
        WSACleanup();
        return false;
    }
    
    _listenSocket = socket(result->ai_family, result->ai_socktype, 
            result->ai_protocol);
    if (_listenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    
    // Steup the TCP listening socket.
    iResult = bind(_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("Bind Failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(_listenSocket);
        WSACleanup();
        return false;
    }
    
    return true;
    
}
#else
// TODO linux support
#endif
