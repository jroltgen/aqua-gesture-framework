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

#include <vector>

#include "events/Event.h"
#include "EventProcessor.h"
#include "gestures/Gesture.h"

class Region : public EventProcessor {

// Attributes
private:
    int                     _regionID;
    std::vector<Gesture*>   _gestures;
    
// Methods
public:
    Region();
    ~Region();

    std::vector<Gesture*>*  getGestures();
    void                    processEvent(Event* e);
    int                     getRegionID();
};

#endif