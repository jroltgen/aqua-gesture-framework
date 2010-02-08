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

/**
 * Creates a new input device connection object.  This constructor simply
 * initializes the member variables.  If you want your input device to actually
 * do anything, you will have to call run().
 */
InputDeviceConnection::InputDeviceConnection(AquaSocket theSocket, 
        GestureServer* theServer, int id) {
    _socket = theSocket;
    _server = theServer;
    _id = id;
}
    
/**
 * Starts a new thread that will receive events from the input device.
 */
void InputDeviceConnection::run() {
    #ifdef _WIN32
    CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->runReadEvents, 
            (void *)this, 0, NULL);
    #else
    // LS
    #endif
}

/**
 * Reads events from the input stream.
 */
void InputDeviceConnection::readEvents() {
    printf("[InputDeviceConnection] ID(%d) Connected\n", _id);
    
    bool ok;
    do {
        ok = readEvent();
    } while (ok);
    
    printf("[InputDeviceConnection] ID(%d) Disconnected\n", _id);
    
    // Delete myself, because no one has a reference to me.
    delete this;
}

/**
 * Reads an event from the socket.  First read the length, then proceed by 
 * reading one character at a time until a null character is reached, then 
 * assembling the string.  This is the name of the event.
 */
bool InputDeviceConnection::readEvent() {
    int             iResult;
    unsigned short  msgLength;
    int             remaining;
    
    // Read event length.
    iResult = _socket.recv(receiveBuffer, 2);
    if (iResult != 2) {
        printf("[InputDeviceConnection] Socket receive error.\n");
        return false;
    }
    
    memcpy(&msgLength, receiveBuffer, 2);

    if (EndianConverter::isLittleEndian()) {
        msgLength = EndianConverter::swapShortEndian(msgLength);
    }
    
    remaining = msgLength;
    char* bufferPtr = receiveBuffer;
    while (remaining > 0) {
        iResult = _socket.recv(bufferPtr, remaining);
        if (iResult == AQUASOCKET_RES_ERROR) return false;
        remaining -= iResult;
        bufferPtr += iResult;
    }
       
    Event* receivedEvent = EventFactory::getInstance()->createEvent(
            string(receiveBuffer), receiveBuffer);
            
    // Add our constant to the eventID to avoid conflicting event IDs.
    receivedEvent->setID(_id * 100000 + receivedEvent->getID());
      
    _server->processEvent(receivedEvent);
    
    delete receivedEvent;
    return true;
}

#ifdef _WIN32
int InputDeviceConnection::runReadEvents(void* pThis) {
    ((InputDeviceConnection*)pThis)->readEvents();
    return 0;
}
#else
// LS (if necessary)
#endif
