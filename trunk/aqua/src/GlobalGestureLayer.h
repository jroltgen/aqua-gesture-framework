/**
 * GlobalGestureLayer.h
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
#ifndef _GLOBALGESTURELAYER_H_
#define _GLOBALGESTURELAYER_H_

#include <map>
#include <string>
#include <vector>

#include "ClientConnection.h"
#include "EventProcessor.h"
#include "events/Event.h"
#include "Region.h"

class GlobalGestureLayer : public EventProcessor {

// Attributes
private:
    /**
     * The ClientConnection object that is associated with this Engine.
     */
    ClientConnection* _client;
    
    /**
     * The list of global gestures in this layer.
     */
    std::vector<EventProcessor*> _globalGestures;
    
    /**
     * The list of allowed global events for this client.
     */
    std::vector<std::string> _allowedEvents;
    
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
    
// Methods
public:
    GlobalGestureLayer(ClientConnection* client);
    bool processEvent(Event* event);
    void init();

};

#endif
