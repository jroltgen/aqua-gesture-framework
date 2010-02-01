/**
 * GestureFactory.cpp
 * 
 * This class manages dynamic loading of the gestures, which are provided as
 * shared library files.  In Windows, these are DLLs. 
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include <stdio.h>
#include <vector>

#include "../utils/FileSystem.h" 
#include "GestureFactory.h"

using namespace std;

// Static member variables
GestureFactory* GestureFactory::_instance;

/**
 * Construct a new GestureFactory
 */
GestureFactory::GestureFactory() {
    _gesturesLoaded = false;
    _instance = NULL;
}

/**
 * Returns the instance of this singleton.
 */
GestureFactory* GestureFactory::getInstance() {
    if (_instance == NULL) {
        _instance = new GestureFactory();
        _instance->loadGestures();
    }
    return _instance;
}

/**
 * Creates and returns a new Gesture based on the gesture's name, which is
 * the class name and also the file name of the library (without the
 * extension).
 */
Gesture* GestureFactory::createGesture(string &gestureName, 
        EventProcessor &publisher, int regionID) {
    
    Gesture* gesture;
    CreateGestureFunc creator;
    
    if (!_gesturesLoaded) {
        printf("[GestureFactory] Gestures not loaded.\n");
        return NULL;
    }
    
    // Find the gesture in the map.
    if (_gestureMap.find(gestureName) != _gestureMap.end()) {
       creator = _gestureMap[gestureName];
       gesture = creator(&publisher, regionID);
       return gesture;
    } else {
        printf("[GestureFactory] createGesture() Error: Gesture not ");
        printf("found in map: %s\n", gestureName.c_str());
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
void GestureFactory::loadGestures() {
    
    int                 i;
    vector<string>      libraries;
    
    FileSystem::getSharedLibraryFiles(string("gestures/*"), &libraries);
    
    for (i = 0; i < libraries.size(); i++) {
        HINSTANCE           lib;
        CreateGestureFunc   libFunc;
        string              libName = libraries[i];
        
        lib = LoadLibrary(("gestures/" + libName).c_str());
        
        if (lib) {
            libFunc = (CreateGestureFunc) GetProcAddress(lib, 
                        "createGesture");
            if (libFunc) {
                string tempString = libName.substr(0, libName.length() - 4);
                _gestureMap.insert(pair<string, 
                        CreateGestureFunc>(tempString, libFunc));
                if (!_gesturesLoaded) _gesturesLoaded = true;
            } else {
                printf("[Gesture Factory] Error: Couldn't load function ");
                printf("for library: %s\n", libName);
            }
        } else {
            printf("[Gesture Factory] Error: Could not find library: %s\n", 
                    libName);
        }
    }    
    
}
#else
// TODOlinux support
#endif