/**
 * ClientConnection.h
 * 
 * This is responsible for all communication with the client.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _CLIENTCONNECTION_H_
#define _CLIENTCONNECTION_H_

#include <vector>
#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
// TODO linux xupport
#endif

#include "EventProcessor.h"

class ClientConnection : public EventProcessor {

// Attributes
private:
    #ifdef _WIN32
    SOCKET _clientSocket;
    #else
    // TODO linux support
    #endif

// Methods
public:
    #ifdef _WIN32
    ClientConnection(SOCKET clientSocket);
    #else
    // TODO linux support
    #endif
    
    int getRegionID(Event* e);
    void getRegionGestures(int regionID, std::vector<Gesture*>* gestures);
    void getGlobalGestures(std::vector<Gesture*>* globalGestures);

};

#endif