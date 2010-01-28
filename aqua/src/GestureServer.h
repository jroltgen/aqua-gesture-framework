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

#ifdef _WIN32
#include <winsock2.h>
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
    SOCKET                          listenSocket;
    #else
        // TODO linux support
    #endif
    
    /**
     * This is a list of the current gesture engines.  Events received by
     * the InputDeviceConnections will be sent to each gesture engine.
     */
    std::vector<GestureEngine*>     gestureEngines;
    
    /**
     * This is incremented every time a new input device connects and is 
     * passed to the input device constructor – the input device connection 
     * adds a constant proportional to this ID so that events coming in with 
     * the same ID from multiple input devices will not be confused.
     */
    int                             nextInputDeviceID;

// Methods
public:
    bool processEvent(Event* e);
    void removeGestureEngine(GestureEngine* engineToRemove);
    void run();
    
private:
    #ifdef _WIN32
    void createInputDeviceConnection(SOCKET* inputSocket);
    void createClientConnection(SOCKET* clientSocket);
    #else
    // TODO linux support
    #endif

};

#endif
