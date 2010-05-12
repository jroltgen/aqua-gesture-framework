/**
 * HelloWorldGesture.cpp
 * 
 * Prints "Hello World" when an event is received.
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
#ifndef _SAMPLEGESTURE_H_
#define _SAMPLEGESTURE_H_

#include <map>
#include <queue>

#include "events/Event.h"
#include "Gesture.h"  
#include "TouchData.h"

#define QUEUE_SIZE 5
#define MILLIS_THRESH 20

typedef struct {
    void* gestureInstance; //this is the gesture instance we need for threading. 
    int touchID;
    double x;
    double y;
    double xVel;
    double yVel;
} kineticInfo_t;

class UnifiedKineticGesture : public Gesture {

// Attributes
private:
    // Maps unified event ids to queues which hold the last 5 events.
    std::map<int, std::queue<TouchData*>*> _idmap;

// Methods
public:
    UnifiedKineticGesture(EventProcessor* publisher, int regionID = -1) :
            Gesture(publisher, regionID) {};
    virtual bool handleEvent(Event* event);
    
    void handleBirth(Event* e);
    void handleMove(Event* e);
    bool handleDeath(Event* e);
    TouchData* createTouchData(Event* e);
    void processKinetics(kineticInfo_t* info);
    
private:
    bool isFlick(double xVel, double yVel, double xAcc, double yAcc);
};

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
		return new UnifiedKineticGesture(publisher, regionID);
	}
}
#else
extern "C" {
    Gesture* createGesture(EventProcessor* publisher, int regionID) {
        return new SampleGesture(publisher, regionID);
    }
}
#endif



#endif
