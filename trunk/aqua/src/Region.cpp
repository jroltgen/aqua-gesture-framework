/**
 * Region.cpp
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
#include "Region.h"

using namespace std;

Region::Region(int regionID, ClientConnection* client) {
    _regionID = regionID;
    _client = client;
}

/**
 * The destructor must de-allocate the gestures because the Region "owns" these
 * gestures - it is the only object that knows about them.
 */
Region::~Region() {
    unsigned int i;
    for (i = 0; i < _gestures.size(); i++) {
        delete _gestures[i];
    }
}

/**
 * Initializes this region by communicating with the client.
 */
void Region::init() {
    _client->getRegionInfo(_regionID, _gestures, _allowedEvents);
}

/**
 * Here we just send the event along to all of our associated gestures.
 */
bool Region::processEvent(Event* event) {
    unsigned int i;
   
    for (i = 0; i < _gestures.size(); i++) {
        _gestures[i]->processEvent(event);
    }
    // TODO we might want to do something with the "consumed" here.
    
    // If the client wants this event, send it.
    for (i = 0; i < _allowedEvents.size(); i++) {
        if (_allowedEvents[i].compare(event->getName()) == 0) {
            _client->processEvent(event, _regionID);
            break;
        }
    }
    return false;
}

int Region::getRegionID() {
    return _regionID;
}
