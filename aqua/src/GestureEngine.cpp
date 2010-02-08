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
    _globalLayer = new GlobalGestureLayer(c);
}

void GestureEngine::init() {
    _client->getTranslators(_gestureTranslators, _globalLayer);
    _globalLayer->init();
}

bool GestureEngine::processEvent(Event* event) {
    unsigned int i;
    bool consumed = false;
    
    for (i = 0; i < _gestureTranslators.size(); i++) {
        bool result = _gestureTranslators[i]->processEvent(event);
        if (result) consumed = true;
    }
    
    if (consumed) {
        return true;
    } else {
        _globalLayer->processEvent(event);
        return false;
    }
}