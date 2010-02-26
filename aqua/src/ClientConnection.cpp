/**
 * ClientConnection.cpp
 * 
 * This is responsible for all communication with the client.
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
#include "ClientConnection.h"
#include "gestures/GestureFactory.h"
#include "utils/AquaException.h"
#include "utils/EndianConverter.h"

using namespace std;

ClientConnection::ClientConnection(AquaSocket clientSocket) {
    _clientSocket = clientSocket;
    
    #ifdef _WIN32
    InitializeCriticalSection(&myLock);
    #else
    pthread_mutex_init(&myLock, NULL);
    #endif
}

ClientConnection::~ClientConnection() {
    
    #ifdef _WIN32
    DeleteCriticalSection(&myLock);
    #else
    pthread_mutex_destroy(&myLock);
    #endif
}

void ClientConnection::getLock() {
    #ifdef _WIN32
    EnterCriticalSection(&myLock);
    #else
    pthread_mutex_lock(&myLock);
    #endif
}

void ClientConnection::releaseLock() {
    #ifdef _WIN32
    LeaveCriticalSection(&myLock);
    #else
    pthread_mutex_unlock(&myLock);
    #endif
}

/**
 * Returns the regionID associated with this event -> it must go out to the 
 * client and ask for the regionID.  It will probably just use the location 
 * in the event to do this to reduce network overhead.
 */
int ClientConnection::getRegionID(Event* e) {
    char msgType = CLIENT_MSG_REGION_ID;
    int response;
    float tempLocation[3];
    unsigned int i;
    
    getLock();
     
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending regionID message.\n");
    }
    
    // Send the location
    tempLocation[0] = e->getX();
    tempLocation[1] = e->getY();
    tempLocation[2] = e->getZ();
    
    // Handle endianness
    if (EndianConverter::isLittleEndian()) {
        for (i = 0; i < 3; i++) {
            tempLocation[i] = 
					EndianConverter::swapFloatEndian(tempLocation[i]);
        }
    }
    if (_clientSocket.send(&tempLocation, 12) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending location.\n");
    }
    
    // Receive the (integer) response.
    if (_clientSocket.recv(&response, 4) == AQUASOCKET_RES_ERROR) {
        return handleError("Error receiving RegionID response.\n");
    }
    // Handle endianness
    if (EndianConverter::isLittleEndian()) {
        response = EndianConverter::swapIntEndian(response);
    }
    
    releaseLock();
    return response;
}

/**
 * Prints out an error message.
 */
int ClientConnection::handleError(char* msg) {
    printf("[ClientConnection] Socket error: %s\n", msg);
    AquaException ae(AQUA_SOCKET_EXCEPTION, "Socket error");
    throw ae;
}

/**
 * Is called when a new region is created by the GestureEngine, should add the 
 * gestures to the vector provided.  
 */
void ClientConnection::getRegionInfo(int regionID, 
        vector<EventProcessor*>& gestures, vector<string>& events) {
    
    int tempID;
    char msgType = CLIENT_MSG_REGION_INFO;
    char data[MAX_NAME_LENGTH];
    
    getLock();
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending getRegionInfo message.\n");
    }
    
    // Send the region ID (handling endianness)
    tempID = regionID;
    if (EndianConverter::isLittleEndian()) {
        tempID = EndianConverter::swapIntEndian(tempID);
    }
    if (_clientSocket.send(&tempID, 4) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending region ID during getRegionInfo.\n");
    }
    
    receiveGestures(data, gestures, this, regionID);
    receiveEvents(data, events);
    
    releaseLock();
}

/**
 * This is called only once by the GestureEngine upon initialization.  It 
 * takes as input a pointer to the engine’s list of global gestures and places
 * any gestures received/created into that list.  If the client is interested 
 * in the events from any of these global gestures, it must also add ITSELF 
 * to the globalGesture list.  This will be sent in the protocol as a 
 * string “MYSELF.”
 */
void ClientConnection::getGlobalInfo(vector<EventProcessor*>& globalGestures, 
        vector<string>& events) {
    
    char msgType = CLIENT_MSG_GLOBAL_INFO;
    char data[MAX_NAME_LENGTH];
    
    getLock();
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending getRegionInfo message.\n");
    }
    
    // Receive the #gestures, gestures, #events, events.
    receiveGestures(data, globalGestures, this);
    receiveEvents(data, events);
    
    releaseLock();
}

void ClientConnection::getTranslators(vector<EventProcessor*>& translators,
        EventProcessor* globalLayer) {
    char msgType = CLIENT_MSG_TRANSLATORS;
    char data[MAX_NAME_LENGTH];
    
    getLock();
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending getRegionInfo message.\n"); 
    }
    
    // Receive the translators.
    receiveGestures(data, translators, globalLayer);
    
    releaseLock();
}

int ClientConnection::receiveGestures(char* buffer, 
        vector<EventProcessor*>& gestures, EventProcessor* publisher, 
        int regionID) {
    
    unsigned int i;
    int numStrings;
    Gesture* newGesture;
    string receivedString;
    
    
    // #gestures
    if (_clientSocket.recv(&numStrings, 4) == AQUASOCKET_RES_ERROR) {
        handleError("Error receiving the number of gestures.\n");
    }
    if (EndianConverter::isLittleEndian()) {
        numStrings = EndianConverter::swapIntEndian(numStrings);
    }
    
    // gestures
    for (i = 0; i < numStrings; i++) {
        if (receiveString(buffer) == NULL) {
            return -1;
        }
        receivedString = buffer;
        
        newGesture = GestureFactory::getInstance()->createGesture(
                receivedString, *publisher, regionID);
        if (newGesture == NULL) {
            printf("Error creating gesture: %s!n", buffer);
            return -1;
        } else {
            gestures.push_back(newGesture);
        }        
        
    }
    return 0;
}

int ClientConnection::receiveEvents(char* buffer, vector<string>& events) {
    unsigned int i;
    int numStrings;
    string receivedString;

    if (_clientSocket.recv(&numStrings, 4) == AQUASOCKET_RES_ERROR) {
        return handleError("Error receiving the number of gestures.\n");
    }
    if (EndianConverter::isLittleEndian()) {
        numStrings = EndianConverter::swapIntEndian(numStrings);
    }   

    for (i = 0; i < numStrings; i++) {
        if (receiveString(buffer) == NULL) {
            printf("Error in receiveString.\n");
            return -1;
        }
        receivedString = buffer;
        events.push_back(receivedString);
    }
    return 0;
}

char* ClientConnection::receiveString(char* buffer) {
    int recvIndex = 0;
    while (true) {
        if (_clientSocket.recv(&buffer[recvIndex], 1) == AQUASOCKET_RES_ERROR) {
            handleError("Error receiving gesture character.\n");
            return NULL;
        }
        if (buffer[recvIndex] == '\0') {
            return buffer;
        } else {
            recvIndex++;
        }
    }
    return buffer;
}



bool ClientConnection::processEvent(Event* e) {
    char msgType = CLIENT_MSG_PROCESS_GLOBAL_EVENT;
    short int length;
    short int networkLength;
    char* data;
    
    getLock();
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending processGlobalEvent message.\n");
    }
    // Send the event 
    data = e->serialize(length);
    networkLength = length;
    if (EndianConverter::isLittleEndian()) {
        networkLength = EndianConverter::swapShortEndian(length);
    }

    if (_clientSocket.send(&networkLength, 2) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending process Global Event msg.\n");
    }

    if (_clientSocket.send(data, length) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending process Global event msg data.\n");
    }

    delete[] data;
    releaseLock();
    return false;
}

bool ClientConnection::processEvent(Event* e, int regionID) {
    char msgType = CLIENT_MSG_PROCESS_REGION_EVENT;
    short int length;
    short int networkLength;
    int tempID;
    char* data;
    
    //printf("In processRegionEvent.\n");
    getLock();
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending processRegionEvent message.\n");
    }
    
    // Send the regionID.
    tempID = regionID;
    if (EndianConverter::isLittleEndian()) {
        tempID = EndianConverter::swapIntEndian(tempID);
    }
    if (_clientSocket.send(&tempID, 4) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending regionID in regionEvent msg.\n");
    }
    
    // Send the event 
    data = e->serialize(length);
    networkLength = length;
    if (EndianConverter::isLittleEndian()) {
        networkLength = EndianConverter::swapShortEndian(networkLength);
    }

    if (_clientSocket.send(&networkLength, 2) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending process Global Event msg.\n");
    }

    if (_clientSocket.send(data, length) == AQUASOCKET_RES_ERROR) {
        return handleError("Error sending process Global event msg data.\n");
    }

    delete[] data;
    
    releaseLock();
    
    return false;
}
