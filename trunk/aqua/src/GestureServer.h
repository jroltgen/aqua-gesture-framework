/**
 * GestureServer.h
 * 
 * This is the main class for Aqua.  It listens for incoming connections from 
 * devices.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _GESTURESERVER_H_
#define _GESTURESERVER_H_

#include <vector>

// Connection port to use
#define LISTEN_PORT "3007"

// Connection types TODO - define these somewhere else if we start using
// a custom protocol.
#define INPUT_DEVICE_TYPE   0
#define CLIENT_TYPE         1

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
#include "GestureEngine.h"
#include "gestures/Gesture.h"

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
    #ifdef _WIN32
    SOCKET _listenSocket;
    #else
        // TODO linux support
    #endif
    
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
    void removeGestureEngine(GestureEngine* engineToRemove);
    void run();
    
private:
    void acceptConnections();
    bool initSocket();
    
    #ifdef _WIN32
    void createInputDeviceConnection(SOCKET inputSocket);
    void createClientConnection(SOCKET clientSocket);
    #else
    // TODO linux support
    #endif
    
};

#endif
