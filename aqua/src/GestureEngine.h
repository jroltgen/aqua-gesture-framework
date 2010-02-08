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

#include "EventProcessor.h"
#include "GlobalGestureLayer.h"

class GestureEngine : public EventProcessor {

// Attributes
private: 
    /**
     * This is the list of global gestures - the gestures that we will ask the
     * client to send us on initialization.
     */
    std::vector<EventProcessor*> _gestureTranslators;
    
    /**
     * This is the assocated GlobalGestureLayer which handles the global
     * gestures and also fowards events to the regions.
     */
    GlobalGestureLayer* _globalLayer;
    
//Methods
public:
    GestureEngine(ClientConnection* client);
    bool processEvent(Event* event);
    void init();
    
};
#endif
