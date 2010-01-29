/**
 * GestureEngine.cpp
 * 
 * GestureEngine is the core of the framework.  It is responsible for event 
 * management through the various global and region-centric gestures, 
 * forwarding the events appropriately, and managing the various regions.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include <stdio.h>

#include "GestureEngine.h"

using namespace std;

GestureEngine::GestureEngine(ClientConnection* c) {
    _client = c;
}

void GestureEngine::init() {
    _client->getGlobalGestures(&_globalProcessors);
    // TODO maybe get interested events as well?
}

bool GestureEngine::processEvent(Event* event) {

    int regionID;
    unsigned int i;
    bool consumed = false;
    Region* r;
    int eventID;
    
    // First send the event to the global processors.
    for (i = 0; i < _globalProcessors.size(); i++) {
        bool result = _globalProcessors[i]->processEvent(event);
        if (result) consumed = true;
    }
    
    // If someone consumed this event, do not forward to the regions or to 
    // the client.
    if (consumed) return false;
    
    // Forward this event directly to the client
    // TODO we may end up changing this - the client might need to send its
    // list of interested events as well.
    _client->processEvent(event);

    if (event->getType() == EVENT_TYPE_DOWN || event->getType() == 
            EVENT_TYPE_OTHER || event->getType() == EVENT_TYPE_HOVER) {
        // For these events we will always get the regionID.
        // We need to ask the client to get it for us.
        regionID = _client->getRegionID(event);
        
        // Check if this region already exists.
        if (_regionIDToRegionMap.find(regionID) != _regionIDToRegionMap.end()) {
            // Region already exists, send this event to that region.
            _regionIDToRegionMap[regionID]->processEvent(event);
        } else {
            // Region does not exist.  Create it!
            r = new Region(regionID);
            // Get the gestures for this region.
            _client->getRegionGestures(regionID, r->getGestures());
            r->processEvent(event);
        }
        
        // Add this eventID to the eventToRegion map (only if it's a down event)
        if (event->getType() == EVENT_TYPE_DOWN) {
            _eventToRegionMap.insert(pair<int, Region*>(event->getID(), r));
        }
        
    } else if (event->getType() == EVENT_TYPE_MOVE || 
            event->getType() == EVENT_TYPE_UP) {
        
        // Get the event ID
        eventID = event->getID();
        
        // Get regionID from map.
        if (_eventToRegionMap.find(eventID) != _eventToRegionMap.end()) {
            // This region exists.
            _eventToRegionMap[eventID]->processEvent(event);
         } else {
            // This region doesn't exist - this is an error.
            printf("[GestureEngine] Error: Region doesn't exist for ");
            printf("eventID: %d\n", eventID);
        }
            
        // If this is an up, remove it from the event mapping.
        if (event->getType() == EVENT_TYPE_UP) {
            _eventToRegionMap.erase(eventID);
        }
    }
        
    return false;
}