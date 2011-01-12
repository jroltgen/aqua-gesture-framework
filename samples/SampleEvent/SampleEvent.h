/**
 * SampleEvent.h
 *
 * This class demonstrates how to create a simple AQUA-G event.  Additional
 * comments are added to help describe what is necessary to create an
 * event which will be used by AQUA-G.
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
#ifndef _SAMPLEEVENT_H_
#define _SAMPLEEVENT_H_

/**
 * Here I define my sample event's data length, in bytes.
 * I use this define during serialization, and also to specify
 * the size of my data array.
 */
#define SAMPLE_EVENT_DATA_LENGTH 5

#include "Event.h"

class SampleEvent : public Event {

// Attributes
private:
    /**
     * Here is my event's custom data.
     */
    char _myEventType;
    int  _myEventInteger;

    /**
     * This is the buffer to store my serialized data.
     */
    char _dataBuffer[SAMPLE_EVENT_DATA_LENGTH];


// Methods
public:
    /**
     * Refer to the .cpp file for method comments.
     */
    SampleEvent(char* data);
    SampleEvent(std::string& name, std::string& desc, char type, int id, 
        float* location, char myEventType, int myEventInteger);
    virtual ~SampleEvent(void);

    char getMyEventType();
    int  getMyEventInteger();

protected:
    virtual char* serializeData(short& outLength);
};
#endif
