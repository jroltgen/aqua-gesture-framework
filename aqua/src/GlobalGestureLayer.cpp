/**
 * GlobalGestureLayer.cpp
 * 
 * GlobalGestureLayer serves as a placeholder for all of the global gestures.
 * Global gestures listen for all events coming in from input devices, however
 * they only publish to the client - they are not allowed to publish back
 * to themselves like the translaters in the GestureEngine are.
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010  Jay Roltgen.
 *
 * Aqua is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    //printf("[GGL] Sending event to client.\n");
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
            r = _regionIDToRegionMap[regionID];
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
            printf("[GlobalGestureEngine] Error: Region doesn't exist for ");
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