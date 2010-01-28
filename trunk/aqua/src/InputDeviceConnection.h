/**
 * InputDeviceConnection.h
 * 
 * Communicates with each input device – there should be one 
 * InputDeviceConnection for every input device that is currently connected 
 * to the gesture server.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _INPUTDEVICECONNECTION_H_
#define _INPUTDEVICECONNECTION_H_

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

#include "GestureServer.h"

class InputDeviceConnection {

// Attributes
private:
    /**
     * The gesture server, send the events here.
     */
    GestureServer* _server;
    
    /**
     * Socket that communicates with the input device.
     */
    #ifdef _WIN32
    SOCKET _socket;
    #else
    // TODO linux support
    #endif
    
    /**
     * The input device ID for this device.
     */
    int _id;
    
// Methods
public:
    #ifdef _WIN32
    InputDeviceConnection(SOCKET theSocket, GestureServer* theServer, int id);
    #else
    // TODO linux support
    #endif
    
    void run();
    
    
private:
    void readEvents();
    void readEvent();

    #ifdef _WIN32
    // Needed for the Win32 thread implementation.
    static int runReadEvents(void* pThis);
    #else
    // TODO linux support (if necessary
    #endif

};

#endif