/**
 * InputDeviceConnection.h
 * 
 * Communicates with each input device � there should be one 
 * InputDeviceConnection for every input device that is currently connected 
 * to the gesture server.
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
#ifndef _INPUTDEVICECONNECTION_H_
#define _INPUTDEVICECONNECTION_H_

/**
 * The size of the input buffer.  Currently this is 65536, or 65KB.  If the 
 * input device sends an event which is larger than this value, the buffer will 
 * overflow and BAD things will happen.  This input buffer size has been 
 * designed so that it corresponds to the maximum event length, which is an
 * unsigned short int.
 */
#define INPUT_BUFFER_SIZE 65536

#include "GestureServer.h"
#include "utils/AquaSocket.h"

class InputDeviceConnection {

// Attributes
private:
    /**
     * The gesture server, send the events here.
     */
    GestureServer* _server;
    
    /**
     * Buffer for incoming data ->incoming data limited by this size.
     */
    char receiveBuffer[INPUT_BUFFER_SIZE];
    
    /**
     * Socket that communicates with the input device.
     */
    AquaSocket _socket;
    
    /**
     * The input device ID for this device.
     */
    int _id;
    
// Methods
public:
    InputDeviceConnection(AquaSocket theSocket, GestureServer* theServer, int id);
    void run();
    
private:
    void readEvents();
    bool readEvent();

    #ifdef _WIN32
    // Needed for the Win32 thread implementation.
    static int runReadEvents(void* pThis);
    #else
    // TODO linux support (if necessary)
    #endif
};
#endif
