/**
 * EventFactory.cpp
 * 
 * This class manages dynamic loading of the events, which are provided as
 * shared library files.  In Windows, these are DLLs. 
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
#include <vector>

#include "../utils/FileSystem.h" 
#include "EventFactory.h"

using namespace std;

// Static member variables
EventFactory* EventFactory::_instance;

/**
 * Construct a new EventFactory
 */
EventFactory::EventFactory() {
    _eventsLoaded = false;
    _instance = NULL;
}

/**
 * Returns the instance of this singleton.
 */
EventFactory* EventFactory::getInstance() {
    if (_instance == NULL) {
        _instance = new EventFactory();
        _instance->loadEvents();
    }
    return _instance;
}

/**
 * Creates and returns a new Gesture based on the gesture's name, which is
 * the class name and also the file name of the library (without the
 * extension).
 */
Event* EventFactory::createEvent(string &eventName, char* data) {
    
    Event* event;
    CreateEventFunc creator;
    
    if (!_eventsLoaded) {
        printf("[EventFactory] Events not loaded.\n");
        return NULL;
    }
    
    // Find the gesture in the map.
    if (_eventMap.find(eventName) != _eventMap.end()) {
       creator = _eventMap[eventName];
       event = creator(data);
       return event;
    } else {
        printf("[EventFactory] createGesture() Error: Event not ");
        printf("found in map: %s\n", eventName.c_str());
        return NULL;
    }
}

#ifdef _WIN32
/**
 * Loads the gestures for the Windows environment from DLLs.  Each gestures
 * is contained in its own shared library and the class name exactly matches
 * the DLL file name (without the extension).  The gestures are loaded from
 * the ./gestures sub-directory of the directory the executable sits in.
 */
void EventFactory::loadEvents() {
    
    int                 i;
    vector<string>      libraries;
    
    FileSystem::getSharedLibraryFiles(string("events/*"), &libraries);
    
    for (i = 0; i < libraries.size(); i++) {
        HINSTANCE           lib;
        CreateEventFunc     libFunc;
        string              libName = libraries[i];
        
        lib = LoadLibrary(("events/" + libName).c_str());
        
        if (lib) {
            libFunc = (CreateEventFunc) GetProcAddress(lib, 
                        "createEvent");
            if (libFunc) {
                string tempString = libName.substr(0, libName.length() - 4);
                _eventMap.insert(pair<string, 
                        CreateEventFunc>(tempString, libFunc));
                if (!_eventsLoaded) _eventsLoaded = true;
            } else {
                printf("[Event Factory] Error: Couldn't load function ");
                printf("for library: %s\n", libName.c_str());
            }
        } else {
            printf("[Event Factory] Error: Could not find library: %s\n", 
                    libName.c_str());
        }
    }    
    
}
#else
void EventFactory::loadEvents() {
    
    int                 i;
    vector<string>      libraries;
    
    FileSystem::getSharedLibraryFiles(string("./events"), &libraries);
    
    for (i = 0; i < libraries.size(); i++) {
        void*               lib;
        CreateEventFunc     libFunc;
        string              libName = libraries[i];
        
        lib = dlopen(("./events/" + libName).c_str(), RTLD_NOW);
        
        if (lib) {
            libFunc = (CreateEventFunc) dlsym(lib, "createEvent");
            if (libFunc) {
                string tempString = libName.substr(3, libName.length() - 6);
                _eventMap.insert(pair<string, 
                        CreateEventFunc>(tempString, libFunc));
                if (!_eventsLoaded) _eventsLoaded = true;
            } else {
                printf("[Event Factory] Error: Couldn't load function ");
                printf("for library: %s\n", libName.c_str());
            }
        } else {
            printf("[Event Factory] Error: Could not find library: %s\n", 
                    libName.c_str());
        }
    }    
    
}
#endif
