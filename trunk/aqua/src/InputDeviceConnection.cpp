/**
 * InputDeviceConnection.cpp
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
#include "InputDeviceConnection.h"

#ifdef _WIN32
InputDeviceConnection::InputDeviceConnection(SOCKET theSocket, 
        GestureServer* theServer, int id) {
    _socket = theSocket;
    _server = theServer;
    _id = id;
}
#else
// TODO linux support
#endif
    
    
/**
 * Starts a new thread that will receive events from the input device.
 */
void InputDeviceConnection::run() {

    #ifdef _WIN32
    CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->runReadEvents, 
            (void *)this, 0, NULL);
    #else
    // TODO linux support
    #endif

}

/**
 * Reads events from the input stream.
 */
void InputDeviceConnection::readEvents() {
    printf("[InputDeviceConnection] Connected, receiving events.\n");
    while (true) {
        readEvent();
    }
}

/**
 * Read a single event from the input stream.
 */
void InputDeviceConnection::readEvent() {
    // TODO implement 
}


int InputDeviceConnection::runReadEvents(void* pThis) {
    ((InputDeviceConnection*)pThis)->readEvents();
    return 0;
}