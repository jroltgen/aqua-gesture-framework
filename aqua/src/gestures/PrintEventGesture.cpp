/**
 * PrintEventGesture.cpp
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

#include <stdio.h>

#include "PrintEventGesture.h"

using namespace std;

bool PrintEventGesture::handleEvent(Event* event) {
    printEvent(event);
    return false;
}

string PrintEventGesture::getType(char type) {
    switch(type) {
    case EVENT_TYPE_DOWN:
        return string("DOWN");
    case EVENT_TYPE_MOVE:
        return string("MOVE");
    case EVENT_TYPE_UP:
        return string("UP");
    case EVENT_TYPE_HOVER:
        return string("OTHER");
    case EVENT_TYPE_OTHER:
        return string("HOVER");
    default:
        return string("UNRECOGNIZED TYPE");
    }
}

void PrintEventGesture::printEvent(Event* event) {
    printf("[PrintEventGesture] Received Event.\n");
    printf("\t%s : %s\n", event->getName().c_str(), event->getDesc().c_str());
    printf("\tType: %c ---", getType(event->getType()));
    printf(" ID: %d\n", event->getID());
    printf("\tX: %f\n\tY: %f\n\tZ: %f\n", event->getX(), event->getY(), 
            event->getZ());
}