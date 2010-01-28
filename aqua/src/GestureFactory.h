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
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
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
    /**
     * Maps the gesture name from a string to the function that is exported by 
     * each DLL, the gestureCreator().  It is a function pointer which can be 
     * used to instantiate a new gesture of that type – this is the key to 
     * the dynamic loading of classes.
     */
    std::map<std::string, CreateGestureFunc>    _gestureMap;
    
    /**
     * The single instance of this class
     */
    static GestureFactory*                      _instance;
    
    /**
     * Tells whether the gestures have beeen loaded sucessfully.
     */
    bool                                        _gesturesLoaded;

// Methods
public:
    static GestureFactory* getInstance();
    Gesture* createGesture(std::string &gestureName, EventProcessor &publisher, 
            int regionID = -1);
    void loadGestures();

private:
    GestureFactory();
 
};

#endif