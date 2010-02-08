/**
 * GlobalGestureLayer.cpp
 * 
 * GlobalGestureLayer serves as a placeholder for all of the global gestures.
 * Global gestures listen for all events coming in from input devices, however
 * they only publish to the client - they are not allowed to publish back
 * to themselves like the translaters in the GestureEngine are.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
 
#include "GlobalGestureLayer.h"
#include "Region.h"

using namespace std;

GlobalGestureLayer::GlobalGestureLayer(ClientConnection* client) {
    _client = client;
}

void GlobalGestureLayer::init() {
    _client->getGlobalInfo(_globalGestures, _allowedEvents);
}

bool GlobalGestureLayer::processEvent(Event* event) {
    int regionID;
    unsigned int i;
    bool consumed = false;
    Region* r;
    int eventID;
    
    // First send the event to the global gestures.
    for (i = 0; i < _globalGestures.size(); i++) {
        bool result = _globalGestures[i]->processEvent(event);
        if (result) consumed = true;
    }
    
    // Then send the event to the client if it wants it.
    for (i = 0; i < _allowedEvents.size(); i++) {
        if (_allowedEvents[i].compare(event->getName()) == 0) {
            // The client wants this event.  Send it.
            _client->processEvent(event);
            break;
        }
    }
    
    // Then send it to the appropriate region.
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
            r = new Region(regionID, _client);
            // Get the gestures for this region.
            r->init();
            // Add this region.
            _regionIDToRegionMap.insert(pair<int, Region*>(regionID, r));
            // Process the event.
            r->processEvent(event);
        }
        
        // Add this eventID to the eventToRegion map if it's a down event.
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
        
        // TODO implement - we want to remove the region when there are no more
        // events on it, this will involve keeping track of the EventIDs
        // associated with each region.
    }
    return consumed;
}