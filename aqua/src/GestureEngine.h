/**
 * GestureEngine.h
 * 
 * GestureEngine is the core of the framework.  It is responsible for event 
 * management through the various global and region-centric gestures, 
 * forwarding the events appropriately, and managing the various regions.
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
#ifndef _GESTUREENGINE_H_
#define _GESTUREENGINE_H_

#include <map>
#include <vector>

#include "EventProcessor.h"
#include "GlobalGestureLayer.h"
#include "ClientConnection.h"

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
    
    /**
     * The clientConnection object.
     */
    ClientConnection* _client;
    
    // Thread lock
    #ifdef _WIN32
    CRITICAL_SECTION myLock;
    #else
    // TODO ls
    #endif
    
//Methods
public:
    GestureEngine(ClientConnection* client);
    ~GestureEngine();
    
    void init();
    bool processEvent(Event* event);
    
};
#endif
