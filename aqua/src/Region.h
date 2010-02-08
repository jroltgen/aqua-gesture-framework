/**
 * Region.h
 * 
 * This exists as a parallel to the logical idea of a “region” in the client.  
 * The idea is that some gestures will be “region-centric” and the client will 
 * inform us where to pass events.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _REGION_H_
#define _REGION_H_

#include <string>
#include <vector>

#include "ClientConnection.h"
#include "EventProcessor.h"
#include "events/Event.h"
#include "gestures/Gesture.h"

class Region : public EventProcessor {

// Attributes
private:
    ClientConnection*               _client;
    int                             _regionID;
    std::vector<EventProcessor*>    _gestures;
    std::vector<std::string>        _allowedEvents;
    
// Methods
public:
    Region(int regionID, ClientConnection* c);
    ~Region();

    void init();
    bool processEvent(Event* e);
    int  getRegionID();
};

#endif
