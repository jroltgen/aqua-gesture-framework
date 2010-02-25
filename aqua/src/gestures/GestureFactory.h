/**
 * GestureFactory.h
 * 
 * This class manages dynamic loading of the gestures, which are provided as
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
#include <dlfcn.h>
#endif

#include "../EventProcessor.h"
#include "Gesture.h"

typedef Gesture* (*CreateGestureFunc)(EventProcessor*, int);

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
