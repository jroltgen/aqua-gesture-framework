/**
 * GestureEngine.h
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
#ifndef _GESTUREENGINE_H_
#define _GESTUREENGINE_H_

#include <map>
#include <vector>

#include "ClientConnection.h"
#include "GestureServer.h"
#include "EventProcessor.h"

class GestureEngine : public EventProcessor {

// Attributes
private:
    /**
     * The Gesture server - we need this reference to tell the gesture server
     * when we die.
     */
    GestureServer* _server;
    
    /**
     * The ClientConnection object that is associated with this Engine.
     */
    ClientConnection* _client;
    
    /**
     * This is the list of global gestures - the gestures that we will ask the
     * client to send us on initialization.
     */
    std::vector<EventProcessor*> _globalProcessors;
    
    /**
     * This map is here to provide a more optimized algorithm for locating the 
     * regions.  In the event that the client may have hundreds or even 
     * thousands of regions, the map will provide access to the regions in 
     * logarithmic time, rather than constant time.
     */
    std::map<int, Region*> _regionIDToRegionMap;
    
    /**
     * This exists to map eventIDs to regionIDs to lessen the load on the 
     * client – when an DOWN event is received, the regionID is obtained.  
     * When MOVE events are received, they look in this map first; if there 
     * is no key in this map that matches, it gets the regionID from the 
     * client, otherwise, it uses this regionID.  When an UP event is received, 
     * we will remove the mapping.
     */
    std::map<int, Region*> _eventToRegionMap;
    
//Methods
public:
    GestureEngine(ClientConnection* c, GestureServer* s);
    bool processEvent(Event* event);
    
};

#endif