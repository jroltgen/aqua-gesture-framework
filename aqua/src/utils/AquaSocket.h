/**
 * AquaSocket.h
 * 
 * This provides a platform-independent interface for accessing TCP sockets.
 * Currently it only allows for server sockets and those sockets created by 
 * accepting incoming connections, as it does not provide a
 * connect() method.
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010 Iowa State University.
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
#ifndef _AQUASOCKET_H_
#define _AQUASOCKET_H_

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#endif

// Return values
#define AQUASOCKET_RES_OK     0
#define AQUASOCKET_RES_ERROR -1

#define MAX_CONNECTIONS 128

class AquaSocket {

// Attributes
private:
    /**
     * Maintains a record of the number of sockets alive so cleanup can be
     * performed correctly.
     */
    static int _numSocketsAlive;

    /**
     * Used for returning sockets via the ACCEPT method - if the socket returned
     * has this value false, then there was an accept error.
     */
    bool _valid;
    
    /**
     * The platform-specific socket that we will use.
     */
  #ifdef _WIN32
    SOCKET _socket;
  #else
    int _socket;
  #endif

// Methods
public:
    AquaSocket();   
    ~AquaSocket();
    AquaSocket(const AquaSocket& rhs);
    AquaSocket& operator= (const AquaSocket& rhs);
        
    AquaSocket accept();
    int        bind(char* hostName, char* port);
    int        listen();
    
    int        send(void* data, int length);
    int        recv(void* data, int length);
    bool       isValid() { return _valid; };
private:
    int init();
    int cleanup();
    int initSockets();
    int cleanupSockets();
};

#endif
