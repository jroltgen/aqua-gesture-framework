/**
 * SampleGesture.cpp
 * 
 * Makes locations negative instead of positive and publishee teh resulting event.
 * This sample project is provided to demonstrate how to write a simple AQUA-G
 * gesture.
 * 
 * This file is part of Aqua Universal Gesture Recognition Framework.
 * Copyright (C) 2010 Iowa State University.
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
 
#include "SampleGesture.h"

bool SampleGesture::handleEvent(Event* e) {
    printf("Upon receiving event: %s, %f, %f, %f\n", e->getName().c_str(), e->getX(), e->getY(), e->getZ());
    invertEvent(e);
    return false;
}

void SampleGesture::invertEvent(Event* e) {
    float inv[3];
    //Event* newEvent;

    std::string desc = "Inverted Event";
    
    // Invert x, y and z.
    printf("Original event: %f, %f, %f\n", e->getX(), e->getY(), e->getZ());
    inv[0] = 0 - e->getX();
    inv[1] = 0 - e->getY();
    inv[2] = 0 - e->getZ();
    printf("Inverted location: %f, %f, %f\n", inv[0], inv[1], inv[2]);

    //newEvent = new UnifiedEvent(e->getName(), desc, e->getType(), e->getID(), inv);

    //publishEvent(newEvent);
    //delete(newEvent);

}
