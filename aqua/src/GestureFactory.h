/**
 * GestureFactory.h
 * 
 * This class manages dynamic loading of the gestures, which are provided as
 * shared library files.  In Windows, these are DLLs. 
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _GESTUREFACTORY_H_
#define _GESTUREFACTORY_H_

#include <map>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
//TODOlinux support
#endif

#include "EventProcessor.h"
#include "gestures/Gesture.h"

#ifdef _WIN32
typedef Gesture* (*CreateGestureFunc)(EventProcessor*, int);
#else
//TODOlinux support
#endif


class GestureFactory {

// Attributes
private:
    std::map<std::string, CreateGestureFunc>    _gestureMap;
    static GestureFactory*                      _instance;
    bool                                        _gesturesLoaded;

// Methods
public:
    static GestureFactory* getInstance();
    Gesture* createGesture(std::string gestureName, EventProcessor*  publisher, 
            int regionID = -1);
    void loadGestures(std::string directory);

private:
    GestureFactory();
 
};

#endif