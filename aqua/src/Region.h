/**
 * Region.h
 * 
 * This exists as a parallel to the logical idea of a “region” in the client.  
 * The idea is that some gestures will be “region-centric” and the client will 
 * inform us where to pass events.
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
