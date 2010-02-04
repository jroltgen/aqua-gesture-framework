/**
 * AquaSocket.cpp
 * 
 * This provides a platform-independent interface for accessing TCP sockets.
 * Currently it only allows for server sockets and those sockets created by 
 * accepting incoming connections, as it does not provide a
 * connect() method.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include <stdio.h>
#include <stdlib.h>
 
#include "AquaSocket.h"

// Static variable declaration - number of sockets alive.
int AquaSocket::_numSocketsAlive;

/**
 * Create a new socket.  This method inits the sockets library if necessary
 * and increments the number of socket objects that are currently alive.
 */
AquaSocket::AquaSocket() {
    _valid = true;
    init();
}

AquaSocket::AquaSocket(const AquaSocket& rhs) {
    init();
    _socket = rhs._socket;
    _valid  = rhs._valid;
}

/**
 * Overloaded equals operator for constructing a new object using return 
 * statements and assignments.  This is used when we call accept().
 */
AquaSocket& AquaSocket::operator= (const AquaSocket& rhs) {
    init();
    if (this != &rhs) {
        _socket = rhs._socket;
        _valid  = rhs._valid;
    }
    // Clean up other copy.
    _numSocketsAlive--;
    return *this;
}
    
/**
 * Clean up resources - specifically clean up the sockets library if necessary.
 */
AquaSocket::~AquaSocket() {
    cleanup();
}

/**
 * Initializes sockets.
 */
int AquaSocket::init() {
    if (_numSocketsAlive++ == 0) {
        return initSockets();
    } else {
        return AQUASOCKET_RES_OK;
    }
}

/**
 * Cleans up sockets.
 */
int AquaSocket::cleanup() {
    _numSocketsAlive--;
    if (_numSocketsAlive < 1) {
        cleanupSockets();
    }
    return AQUASOCKET_RES_OK;
}

/**
 * This accepts a new connection - right now, this method does not return
 * address information about the client because we generally don't require it.
 */
AquaSocket AquaSocket::accept() {
    #ifdef _WIN32
    AquaSocket returnSocket;
    
    // Accept a client socket
    SOCKET clientSocket = ::accept(_socket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(_socket);
        returnSocket._valid = false;
    } 
    returnSocket._socket = clientSocket;
    #else
    // LS
    #endif
    
    return returnSocket;
}

/**
 * Intiializes the socket structure and information.  For simplicity, this
 * is done in the constructor.
 */
int AquaSocket::bind(char* hostName, char* port) {
    #ifdef _WIN32
    // Initialize the socket info
    int              iResult;
    struct addrinfo  hints; 
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;  
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    
    // Resolve the local address and port.
    iResult = getaddrinfo(hostName, port, &hints, &result);
    if (iResult != 0) {
        printf("Get addrinfo failed: %d\n", iResult);
        return AQUASOCKET_RES_ERROR;
    }
    
    _socket = socket(result->ai_family, result->ai_socktype, 
            result->ai_protocol);
    if (_socket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        return AQUASOCKET_RES_ERROR;
    }
    
    // Steup the TCP listening socket.
    iResult = ::bind(_socket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("Bind Failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(_socket);
        return AQUASOCKET_RES_ERROR;
    }
    return AQUASOCKET_RES_OK;
    #else
    // LS
    #endif
}

int AquaSocket::listen() {
    #ifdef _WIN32
    // Listen on the socket
    if (::listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Error at listen(): %ld\n", WSAGetLastError());
        closesocket(_socket);
        return AQUASOCKET_RES_ERROR;
    }
    return AQUASOCKET_RES_OK;
    #else
    // LS
    #endif
}
    

int AquaSocket::send(char* data, int length) {
    #ifdef _WIN32
    int iResult = ::send(_socket, data, length, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(_socket);
        iResult = AQUASOCKET_RES_ERROR;
    }
    return iResult;
    #else
    // LS
    #endif
}

int AquaSocket::recv(char* data, int length) {
    #ifdef _WIN32
    int result = ::recv(_socket, data, length, 0);
    // Simplify error handling.
    if (result < 0) {
        int errorCode = WSAGetLastError();
        if (errorCode = 10054) {
            printf("[AquaSocket] Connection reset by peer.\n");
            closesocket(_socket);
        } else if (errorCode = 10038) {
            printf("[AquaSocket] Socket operation on non-socket.  Assuming socket was shut");
            printf(" down improperly or something, so not closing socket..?\n");
        } else {
            printf("[AquaSocket] Unknown Windows Sockets Error: %d ", errorCode);
            printf("Shutting down....\n");
            exit(0);
        }
        result = AQUASOCKET_RES_ERROR;
    }
    return result;
    #else
    // LS
    #endif
}

/**
 * Initiailizes the platform-specific sockets library.
 */
int AquaSocket::initSockets() {
    #ifdef _WIN32
    WSADATA          data;
    int              iResult;
    
    iResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (iResult != 0) {
        printf("Error starting WSA.\n");
        return AQUASOCKET_RES_ERROR;
    }
    return AQUASOCKET_RES_OK;
    #else
    // LS
    #endif
}

/**
 * Cleans up the platform-specific sockets library.
 */
int AquaSocket::cleanupSockets() {
    #ifdef _WIN32
    WSACleanup();
    return AQUASOCKET_RES_OK;
    #else
    // LS
    #endif
}
