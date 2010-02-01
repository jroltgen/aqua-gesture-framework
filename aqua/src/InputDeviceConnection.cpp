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
#include <string>

#include "events/EventFactory.h"
#include "InputDeviceConnection.h"
#include "utils/EndianConverter.h"

using namespace std;

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
 * Handle a socket error
 */
void InputDeviceConnection::handleError(int sockError) {
    printf("Socket error, bytes received: %d\n", sockError);
    // TODO add better error handling - we need to close this connection.
}

/**
 * Read a single event from the input stream.
 */
#ifdef _WIN32
void InputDeviceConnection::readEvent() {
    int iResult;
    unsigned short msgLength;
    // TODO implement 
    // Read event length.
    iResult = recv(_socket, receiveBuffer, 2, 0);
    if (iResult != 1) {
        handleError(iResult);
    }
    
    msgLength = *receiveBuffer;
    if (EndianConverter::isLittleEndian()) {
        msgLength = EndianConverter::swapShortEndian(msgLength);
    }
    printf("Message length: %d\n", msgLength);
    
    iResult = recv(_socket, receiveBuffer, msgLength, 0);
    
    Event* receivedEvent = EventFactory::getInstance()->createEvent(
            string(receiveBuffer), receiveBuffer);
    
    _server->processEvent(receivedEvent);
    
    delete receivedEvent;
}
#else
// TODO linux support
#endif


int InputDeviceConnection::runReadEvents(void* pThis) {
    ((InputDeviceConnection*)pThis)->readEvents();
    return 0;
}