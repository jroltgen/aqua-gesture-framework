/**
 * AquaSocket.cpp
 * 
 * This provides a platform-independent interface for accessing TCP sockets.
 * Currently it only allows for server sockets and those sockets created by 
 * accepting incoming connections, as it does not provide a
 * connect() method.
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
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(_socket);
        returnSocket._valid = false;
    } 
    returnSocket._socket = clientSocket;
  #else
    AquaSocket returnSocket;
    
    socklen_t clientLength;
    int clientSocket;    
    struct sockaddr_in clientAddress;
    clientLength = sizeof(clientAddress);
    
    clientSocket = ::accept(_socket, (struct sockaddr*) &clientAddress, 
            &clientLength);
    
    if (clientSocket < 0) {
        printf("Accept failed: %d\n", clientSocket);
        ::close(clientSocket);
        returnSocket._valid = false;
    }
    returnSocket._socket = clientSocket;
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
    int portNumber, result;
    struct sockaddr_in serverAddress;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        printf("Error creating socket: %d.\n", _socket);
        return AQUASOCKET_RES_ERROR;
    }

    bzero((char*) &serverAddress, sizeof(serverAddress));
    portNumber = atoi(port);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);

    result = ::bind(_socket, (struct sockaddr*) &serverAddress, 
            sizeof(serverAddress));
    if (result < 0) {
        printf("Error binding socket: %d\n", result);
        return AQUASOCKET_RES_ERROR;
    }
    return AQUASOCKET_RES_OK;
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
    if (::listen(_socket, MAX_CONNECTIONS) != 0) {
        printf("Error at listen.\n");
        ::close(_socket);
        return AQUASOCKET_RES_ERROR;
    }
    return AQUASOCKET_RES_OK;
  #endif
}
    

int AquaSocket::send(void* data, int length) {
  #ifdef _WIN32
    int iResult = ::send(_socket, (char*)data, length, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(_socket);
        iResult = AQUASOCKET_RES_ERROR;
    }
    return iResult;
  #else
    int result = ::send(_socket, (char*)data, length, 0);
    if (result == -1) {
        printf("Send failed: %d\n", result);
        ::close(_socket);
        result = AQUASOCKET_RES_ERROR;
    }
    return result;
  #endif
}

int AquaSocket::recv(void* data, int length) {
    #ifdef _WIN32
    int remaining = length;
    while (remaining > 0) {
        int result = ::recv(_socket, &((char*)data)[length - remaining], remaining, 0);
        remaining -= result;
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
            return AQUASOCKET_RES_ERROR;
        }
    }
    return length - remaining;

  #else
    int remaining = length;
    while (remaining = 0) {
        int result = ::recv(_socket, &((char*)data)[length - remaining],
                remaining, 0);
        remaining -= result;
        if (result == 0) {
            printf("[AquaSocket] Connection shutdown by peer.\n");
            ::close(_socket);
            return AQUASOCKET_RES_ERROR;
        } else if (result < 0) {
            printf("[AquaSocket] Error occured in recv.\n");
            ::close(_socket);
            return AQUASOCKET_RES_ERROR;
        }
    }
    return length - remaining;  
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
    // Nothing to do for linux.
    return AQUASOCKET_RES_OK;
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
    // Nothing to do for linux.
    return AQUASOCKET_RES_OK;
  #endif
}
