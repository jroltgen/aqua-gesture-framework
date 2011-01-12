/**
 * SampleEvent.cpp
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
#include "SampleEvent.h"

/**
 * First and foremost, I need to export the functions which allow AQUA-G to
 * load my event dynamically.  You must change the constructors contained
 * here when you change the event name so that they refer to your event's 
 * name instead of "SampleEvent."  The preprocessor-directive _WIN32 exists
 * so that you can compile this event for Linux as well as Windows, if you
 * so desire.
 */
#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Event* createEvent(char* data) {
        return new SampleEvent(data);
    }
}
#else
extern "C" {
    Event* createEvent(char* data) {
        return new SampleEvent(data);
    }
}
#endif

/**
 * I provide a constructor which takes as input a char array.  This is the
 * serialized data that comes over the network - I need to unserialize it
 * in my implementation.
 *
 * First, my event calls the super constructor by calling Event(data).
 */
SampleEvent::SampleEvent(char* data) : Event(data) {

    /**
     * This code exists because char* data contains all of the data in the
     * event, not just our data.  So we need to get the index of our custom 
     * data by bypassing all of the data that is part of the superclass.
     */
    int dataPos = (_name.length() + _description.length() + 2 + 17);

    /**
     * Now we want to un-serialize the data into our event's fields.  We use
     * memcpy to do this.
     */
    memcpy(&_myEventType, &data[dataPos], 1);
    dataPos += 1;
    memcpy(&_myEventInteger, &data[dataPos], 4);
    dataPos += 4;

    /**
     * This code handles endianness - data coming in over the network will
     * be in big-endian form.  If this machine is little-endian, then we need
     * to convert it so it will be handled appropriately.
     */
    if (EndianConverter::isLittleEndian()) {
        _myEventInteger = EndianConverter::swapIntEndian(_myEventInteger);
    }
}

/**
 * This is the normal constructor this event which gestures can use.  Notice
 * again, the first thing we do is call the event base class's constructor.
 */
SampleEvent::SampleEvent(std::string& name, std::string& desc, char type, int id, 
        float* location, char myEventType, int myEventInteger) : 
        Event(name, desc, type, id, location) {

    _myEventType = myEventType;
    _myEventInteger = myEventInteger;
        
}

/**
 * Provide the destructor.
 */
SampleEvent::~SampleEvent() {}

/**
 * This method serializes our custom data by placing it into the data array
 * and returning it.
 */
char* SampleEvent::serializeData(short &outLength) {
    
    outLength = SAMPLE_EVENT_DATA_LENGTH;

    /**
     * Again, handle the endianness - in serialized form, the data must
     * be in big-endian form.
     */
    int tempInt = _myEventInteger;
    if (EndianConverter::isLittleEndian()) {
        tempInt = EndianConverter::swapIntEndian(tempInt);
    }

    memcpy(_dataBuffer + 0, &_myEventType, 1);
    memcpy(_dataBuffer + 1, &tempInt     , 4);

    return _dataBuffer;

}

char SampleEvent::getMyEventType() {
    return _myEventType;
}


int SampleEvent::getMyEventInteger() {
    return _myEventInteger;
}

