/**
 * GestureEngine.cpp
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
#include <stdio.h>

#include "GestureEngine.h"

using namespace std;

GestureEngine::GestureEngine(ClientConnection* c) {
    _client = c;
    _globalLayer = new GlobalGestureLayer(c);
    
    #ifdef _WIN32
    InitializeCriticalSection(&myLock);
    #else
    // TODO ls
    #endif
}

GestureEngine::~GestureEngine() {
    #ifdef _WIN32
    DeleteCriticalSection(&myLock);
    #else
    // TODO ls
    #endif
}

void GestureEngine::init() {
    _client->getTranslators(_gestureTranslators, _globalLayer);
    _globalLayer->init();
}

bool GestureEngine::processEvent(Event* event) {
    unsigned int i;
    bool consumed = false;
    #ifdef _WIN32
    EnterCriticalSection(&myLock);
    #else
    // TODO ls
    #endif
    
    for (i = 0; i < _gestureTranslators.size(); i++) {
        bool result = _gestureTranslators[i]->processEvent(event);
        if (result) consumed = true;
    }
    if (!consumed) {
        _globalLayer->processEvent(event);
    }
    
    #ifdef _WIN32
    LeaveCriticalSection(&myLock);
    #else
    // TODO ls
    #endif
    
    return consumed;
}