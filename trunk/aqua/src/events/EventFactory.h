/**
 * EventFactory.h
 * 
 * This class manages dynamic loading of the events, which are provided as
 * shared library files.  In Windows, these are DLLs. 
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _EVENTFACTORY_H_
#define _EVENTFACTORY_H_

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

#include "../EventProcessor.h"
#include "Event.h"

#ifdef _WIN32
typedef Event* (*CreateEventFunc)(char*);
#else
//TODOlinux support
#endif

class EventFactory {

// Attributes
private:
    /**
     * Maps the gesture name from a string to the function that is exported by 
     * each DLL, the gestureCreator().  It is a function pointer which can be 
     * used to instantiate a new gesture of that type – this is the key to 
     * the dynamic loading of classes.
     */
    std::map<std::string, CreateEventFunc>    _eventMap;
    
    /**
     * The single instance of this class
     */
    static EventFactory*                      _instance;
    
    /**
     * Tells whether the gestures have beeen loaded sucessfully.
     */
    bool                                        _eventsLoaded;

// Methods
public:
    static EventFactory* getInstance();
    Event* createEvent(std::string &eventName, char* data);
    void loadEvents();

private:
    EventFactory();
 
};


#endif