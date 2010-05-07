/**
 * UnifiedKineticGesture.cpp
 * 
 * Transforms unified events into "kinetic events" wherein the events
 * continue past the death point and have "physics".
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
 
#include "UnifiedKineticGesture.h"

using namespace std;

bool UnifiedKineticGesture::handleEvent(Event* e) {
    
    switch(e->getType()) {
    case EVENT_TYPE_DOWN:
        handleBirth(e);
        return false;
    case EVENT_TYPE_MOVE:
        handleMove(e);
        return false;
    case EVENT_TYPE_UP:
        return handleDeath(e);
    default:
        return false;
    }
}

void UnifiedKineticGesture::handleBirth(Event* e) {
    if (_idmap.find(e->getID()) != _idmap.end()) {
        // Exists in map?? error.
        printf("[UnifiedKineticGesture] ERROR.... birth id %d found in map.", e->getID());
        exit(0);
    }
    TouchData* t = createTouchData(e);
    _idmap[e->getID()] = new queue<TouchData*>();
    _idmap[e->getID()]->push(t);
}

#ifdef _WIN32
void UnifiedKineticGesture::handleMove(Event* e) {
    if (_idmap.find(e->getID()) == _idmap.end()) {
        // Exists in map?? error.
        printf("[UnifiedKineticGesture] ERROR.... move id %d not found in map.", e->getID());
        exit(0);
    }
    TouchData* t = createTouchData(e);
    
    int prevms = _idmap[e->getID()]->back()->getTime().wMilliseconds;
    int currms = t->getTime().wMilliseconds;
    
    // Handle wrap-around errors.
    while (currms < prevms) {
        currms += 1000;
    }
    // If this isn't greater than our thresh, throw it out.
    if (currms - prevms > MILLIS_THRESH) {
        _idmap[e->getID()]->push(t);
    } else {
        delete t;
        return;
    }
    
    if (_idmap[e->getID()]->size() > QUEUE_SIZE) {
        // Pop the first element.
        TouchData* t = _idmap[e->getID()]->front();
        _idmap[e->getID()]->pop();
        delete t;
    }
}

bool UnifiedKineticGesture::handleDeath(Event* e) {
    handleMove(e);
    
    double lastXVelocity = 0;
    double sumXVelocity = 0;
    double sumXAcceleration = 0;
    
    double lastYVelocity = 0;
    double sumYVelocity = 0;
    double sumYAcceleration = 0;
    
    float lastX = 0, lastY = 0;
    int lastMS = 0;
    int count = 0;
    
    // If less than 3 touches, can't evaluate curvature, so don't consume the death.
    queue<TouchData*>* q = _idmap[e->getID()];
    if (q->size() < 3) {
        delete q;
        _idmap.erase(e->getID());
        return false;
    }
    
    // We want to evaluate the average velocity and curvature.
    while (!q->empty()) {
        
        double xVelocity, yVelocity, xAcceleration, yAcceleration;
    
        TouchData* t = q->front();
        
        float x = t->getX();
        float y = t->getY();
        int ms  = t->getTime().wMilliseconds;
        
        int dt = ms - lastMS;
        // Handle wrap-around
        while (dt < 0) {
            dt += 1000;
        }
        
        xVelocity = (x - lastX) / dt;
        yVelocity = (y - lastY) / dt;
        xAcceleration = (xVelocity - lastXVelocity) / dt;
        yAcceleration = (yVelocity - lastYVelocity) / dt;
        
        printf("Touch ms: %d, x: %g, y: %g, z: %g\n", t->getTime().wSecond * 1000 + t->getTime().wMilliseconds, t->getX(), t->getY(), t->getZ());
        
        
        if (count == 0) {
            // This is the first point, can't do anything
        }
        if (count > 0) {
            // Calculate velocity
            sumXVelocity += xVelocity;
            sumYVelocity += yVelocity;
        }
        if (count > 1) {
            // Calculate acceleration.
            sumXAcceleration += xAcceleration;
            sumYAcceleration += yAcceleration;
        }
        
        lastX = x;
        lastY = y;
        lastMS = ms;
        lastXVelocity = xVelocity;
        lastYVelocity = yVelocity;
        
        delete t;
        q->pop();
        count++;
    }
    
    float avgXVelocity = sumXVelocity / (count - 1);
    float avgYVelocity = sumYVelocity / (count - 1);
    float avgXAcceleration = sumXAcceleration / (count - 2);
    float avgYAcceleration = sumYAcceleration / (count - 2);
    printf("V: %f, %f   ---- A: %f, %f\n", avgXVelocity, avgYVelocity,
            avgXAcceleration, avgYAcceleration);
            
    delete _idmap[e->getID()];
    _idmap.erase(e->getID());
    
    return false;
}

TouchData* UnifiedKineticGesture::createTouchData(Event* e) {
    SYSTEMTIME currentTime;
    TouchData* t;
    
    GetSystemTime(&currentTime);
    t = new TouchData(e->getLocation(), currentTime);
    return t;
}
#endif