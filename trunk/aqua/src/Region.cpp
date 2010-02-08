/**
 * Region.cpp
 * 
 * This exists as a parallel to the logical idea of a �region� in the client.  
 * The idea is that some gestures will be �region-centric� and the client will 
 * inform us where to pass events.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include "Region.h"

using namespace std;

Region::Region(int regionID, ClientConnection* client) {
    _regionID = regionID;
    _client = client;
}

/**
 * The destructor must de-allocate the gestures because the Region "owns" these
 * gestures - it is the only object that knows about them.
 */
Region::~Region() {
    unsigned int i;
    for (i = 0; i < _gestures.size(); i++) {
        delete _gestures[i];
    }
}

/**
 * Initializes this region by communicating with the client.
 */
void Region::init() {
    _client->getRegionInfo(_regionID, _gestures, _allowedEvents);
}

/**
 * Here we just send the event along to all of our associated gestures.
 */
bool Region::processEvent(Event* event) {
    unsigned int i;
   
    for (i = 0; i < _gestures.size(); i++) {
        _gestures[i]->processEvent(event);
    }
    // TODO we might want to do something with the "consumed" here.
    
    // If the client wants this event, send it.
    _client->processEvent(event, _regionID);
    return false;
}

int Region::getRegionID() {
    return _regionID;
}
