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

using namespace std;

ClientConnection::ClientConnection(AquaSocket clientSocket) {
    _clientSocket = clientSocket;
}

/**
 * Returns the regionID associated with this event -> it must go out to the 
 * client and ask for the regionID.  It will probably just use the location 
 * in the event to do this to reduce network overhead.
 */
int ClientConnection::getRegionID(Event* e) {
    // TODO implement
    return -1;
}

/**
 * Is called when a new region is created by the GestureEngine, should add the 
 * gestures to the vector provided.  
 */
void ClientConnection::getRegionInfo(int regionID, vector<Gesture*>& gestures,
        vector<string>& events) {
    // TODO implement
    // Make sure you do set the regionID for these gestures.
}

/**
 * This is called only once by the GestureEngine upon initialization.  It 
 * takes as input a pointer to the engine’s list of global gestures and places 
 * any gestures received/created into that list.  If the client is interested 
 * in the events from any of these global gestures, it must also add ITSELF 
 * to the globalGesture list.  This will be sent in the protocol as a 
 * string “MYSELF.”
 */
void ClientConnection::getGlobalInfo(
        vector<EventProcessor*>& globalGestures, vector<string>& Events) {
    // TODO implement
    // Make sure you do not set the regionID for these gestures.
}

bool ClientConnection::processEvent(Event* e) {
    // TODO implement
    return false;
}

bool ClientConnection::processEvent(Event* e, int regionID) {
    // TODO implement
    return false;
}
