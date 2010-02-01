/**
 * EventFactory.cpp
 * 
 * This class manages dynamic loading of the events, which are provided as
 * shared library files.  In Windows, these are DLLs. 
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include <stdio.h>
#include <vector>

#include "utils/FileSystem.h" 
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
    
    if (!_eventsLoaded) return NULL;
    
    // Find the gesture in the map.
    if (_eventMap.find(eventName) != _eventMap.end()) {
       creator = _eventMap[eventName];
       event = creator(data);
       return event;
    } else {
        printf("[EventFactory] createGesture() Error: Gesture not ");
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
                printf("for library: %s\n", libName);
            }
        } else {
            printf("[Event Factory] Error: Could not find library: %s\n", 
                    libName);
        }
    }    
    
}
#else
// TODOlinux support
#endif