/**
 * AquaSocket.h
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
// LS
#endif

// Return values
#define AQUASOCKET_RES_OK     0
#define AQUASOCKET_RES_ERROR -1

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
    // LS
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
    
    int        send(char* data, int length);
    int        recv(char* data, int length);
    bool       isValid() { return _valid; };
private:
    int init();
    int cleanup();
    int initSockets();
    int cleanupSockets();
};

#endif
