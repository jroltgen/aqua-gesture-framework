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

//////////////// TODO remove this/
/*
string getType(char type) {
    switch(type) {
    case EVENT_TYPE_DOWN:
        return string("DOWN");
    case EVENT_TYPE_MOVE:
        return string("MOVE");
    case EVENT_TYPE_UP:
        return string("UP");
    case EVENT_TYPE_HOVER:
        return string("OTHER");
    case EVENT_TYPE_OTHER:
        return string("HOVER");
    }
    return string("UNRECOGNIZED");
}
    
    
void printEvent(Event* event) {
    printf("[PrintEventGesture] Received Event.\n");
    printf("\t%s : %s\n", event->getName().c_str(), event->getDesc().c_str());
    printf("\tType: %s ---", getType(event->getType()).c_str());
    printf(" ID: %d\n", event->getID());
    printf("\tX: %f\n\tY: %f\n\tZ: %f\n", event->getX(), event->getY(), 
            event->getZ());
}
*/
////////////////////////////////////

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
    printf("[InputDeviceConnection] ID(%d) Connected\n", _id);
    bool ok;
    do {
        ok = readEvent();
    } while (ok);
    
    // Delete myself, no one has a reference to me.
    printf("[InputDeviceConnection] ID(%d) Disconnected\n", _id);
    delete this;
}

/**
 * Read a single event from the input stream.
 */
#ifdef _WIN32
bool InputDeviceConnection::readEvent() {
    int iResult;
    unsigned short msgLength;
    int remaining;
    // TODO implement 
    // Read event length.
    iResult = recv(_socket, receiveBuffer, 2, 0);
    if (iResult != 2) {
        printf("Error::: %ld\n", WSAGetLastError());
        return false;
    }
    printf("Received length\n");
    
    memcpy(&msgLength, receiveBuffer, 2);
    //msgLength = *receiveBuffer;

    if (EndianConverter::isLittleEndian()) {
        msgLength = EndianConverter::swapShortEndian(msgLength);
        printf("is little endian.\n");
    }
    printf("Message length: %hd\n", msgLength);
    
    remaining = msgLength;
    char* bufferPtr = receiveBuffer;
    while (remaining > 0) {
        iResult = recv(_socket, bufferPtr, remaining, 0);
        remaining -= iResult;
        printf("Data received...length: %d\n", iResult);
        bufferPtr += iResult;
    }
       
    Event* receivedEvent = EventFactory::getInstance()->createEvent(
            string(receiveBuffer), receiveBuffer);
            
    // Add our constant to the eventID to avoid conflicting event IDs.
    receivedEvent->setID(_id * 100000 + receivedEvent->getID());
    
    //printf("Created event...\n");
    //printEvent(receivedEvent);
      
    _server->processEvent(receivedEvent);
    
    delete receivedEvent;
    return true;
}
#else
// TODO linux support
#endif


int InputDeviceConnection::runReadEvents(void* pThis) {
    ((InputDeviceConnection*)pThis)->readEvents();
    return 0;
}