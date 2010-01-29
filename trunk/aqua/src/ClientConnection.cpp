/**
 * ClientConnection.cpp
 * 
 * This is responsible for all communication with the client.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include "ClientConnection.h"

ClientConnection::ClientConnection(SOCKET clientSocket) {
    _clientSocket = clientSocket;
}

/**
 * Returns the regionID associated with this event -> it must go out to the 
 * client and ask for the regionID.  It will probably just use the location 
 * in the event to do this to reduce network overhead.
 */
int ClientConnection::getRegionID(Event* e) {
    // TODO implement
}

/**
 * Is called when a new region is created by the GestureEngine, should add the 
 * gestures to the vector provided.  
 */
void ClientConnection::getRegionGestures(int regionID, 
        std::vector<Gesture*>* gestures) {
    // TODO implement
}

/**
 * This is called only once by the GestureEngine upon initialization.  It 
 * takes as input a pointer to the engine’s list of global gestures and places 
 * any gestures received/created into that list.  If the client is interested 
 * in the events from any of these global gestures, it must also add ITSELF 
 * to the globalGesture list.  This will be sent in the protocol as a 
 * string “MYSELF.”
 */
void ClientConnection::getGlobalGestures(
        std::vector<Gesture*>* globalGestures) {
    // TODO implement
}