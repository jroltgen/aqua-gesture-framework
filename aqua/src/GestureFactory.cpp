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
 
#include "GestureFactory.h"

using namespace std;

// Static member variables
GestureFactory* GestureFactory::_instance;

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
    }
    return _instance;
}


Gesture* GestureFactory::createGesture(string gestureName, 
        EventProcessor* publisher, int regionID) {
    
    Gesture* gesture;
    CreateGestureFunc creator;
    
    if (_gestureMap.find(gestureName) != _gestureMap.end()) {
       creator = _gestureMap[gestureName];
       gesture = creator(publisher, regionID);
       return gesture;
    } else {
        printf("[GestureFactory] Gesture not found in map: %s\n", gestureName.c_str());
        return NULL;
    }
}

#ifdef _WIN32
void GestureFactory::loadGestures(string dir) {
    WIN32_FIND_DATA findData;
    HANDLE          findHandle;
    string          tempString;
    
    HINSTANCE           lib;
    CreateGestureFunc   libFunc;
    
    findHandle = FindFirstFile(dir.c_str(), &findData);
    
    if (findHandle == INVALID_HANDLE_VALUE) {
        printf("[GestureFactory] Error: Invalid Gesture Directory: %\n", dir);
        return;
    }
    
    do {
        //printf("File: %s\n", findData.cFileName);
        tempString = findData.cFileName;
        if (tempString.find(".dll") != string::npos || 
                tempString.find(".DLL") != string::npos) {
            // It's a DLL!
            // TODO add support for using dir instead of just the default dir.
            lib = LoadLibrary(("gestures\\" + tempString).c_str());
            
            if (lib) {
                //printf("Library loaded!!!! woo hoo!!!\n");
                libFunc = (CreateGestureFunc) GetProcAddress(lib, "createGesture");
                if (libFunc) {
                    _gestureMap.insert(pair<string, CreateGestureFunc>(tempString, libFunc));
                } else {
                    printf("[Gesture Factory] Error: Could not load function\n");
                }
            } else {
                printf("[Gesture Factory] Error: Could not find library: %s\n", 
                        findData.cFileName);
            }
        }
    
    } while (FindNextFile(findHandle, &findData) != 0);

}
#else
// TODOlinux support
#endif