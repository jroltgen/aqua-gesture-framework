/**
 * ClientConnection.cpp
 * 
 * This is responsible for all communication with the client.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include "ClientConnection.h"
#include "gestures/GestureFactory.h"
#include "utils/EndianConverter.h"

using namespace std;

ClientConnection::ClientConnection(AquaSocket clientSocket) {
    _clientSocket = clientSocket;
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
            tempLocation[i] = EndianConverter::swapFloatEndian(tempLocation[i]);
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
    return response;
}

/**
 * Prints out an error message.
 */
int ClientConnection::handleError(char* msg) {
    printf("[ClientConnection] Socket error: %s\n", msg);
    return -1;
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
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending getRegionInfo message.\n");
    }
    
    // Send the region ID (handling endianness
    tempID = regionID;
    if (EndianConverter::isLittleEndian()) {
        tempID = EndianConverter::swapIntEndian(tempID);
    }
    if (_clientSocket.send(&tempID, 4) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending region ID during getRegionInfo.\n");
    }
    
    receiveGestures(data, gestures, this, regionID);
    receiveEvents(data, events);
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
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending getRegionInfo message.\n");
    }
    
    // Receive the #gestures, gestures, #events, events.
    receiveGestures(data, globalGestures, this);
    receiveEvents(data, events);
}

void ClientConnection::getTranslators(vector<EventProcessor*>& translators,
        EventProcessor* globalLayer) {
    char msgType = CLIENT_MSG_TRANSLATORS;
    char data[MAX_NAME_LENGTH];
    
    // Send the request.
    if (_clientSocket.send(&msgType, 1) == AQUASOCKET_RES_ERROR) {
        handleError("Error sending getRegionInfo message.\n");
    }
    
    // Receive the translators.
    receiveGestures(data, translators, globalLayer);
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
        return handleError("Error receiving the number of gestures.\n");
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
        if (_clientSocket.recv(&buffer[recvIndex], 1) == AQUASOCKET_RES_ERROR){
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
    
    return false;
}

bool ClientConnection::processEvent(Event* e, int regionID) {
    char msgType = CLIENT_MSG_PROCESS_REGION_EVENT;
    short int length;
    short int networkLength;
    int tempID;
    char* data;
    
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
    return false;
}
