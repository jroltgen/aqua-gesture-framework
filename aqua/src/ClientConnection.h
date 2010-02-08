/**
 * ClientConnection.h
 * 
 * This is responsible for all communication with the client.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#ifndef _CLIENTCONNECTION_H_
#define _CLIENTCONNECTION_H_

#include <string>
#include <vector>

#include "EventProcessor.h"
#include "events/Event.h"
#include "gestures/Gesture.h"
#include "utils/AquaSocket.h"

// Message types
#define CLIENT_MSG_REGION_ID            0
#define CLIENT_MSG_GLOBAL_INFO          1
#define CLIENT_MSG_REGION_INFO          2
#define CLIENT_MSG_PROCESS_GLOBAL_EVENT 3
#define CLIENT_MSG_PROCESS_REGION_EVENT 4
#define CLIENT_MSG_TRANSLATORS          5

#define MAX_NAME_LENGTH 64

class ClientConnection : public EventProcessor {

// Attributes
private:
    AquaSocket _clientSocket;

// Methods
public:
    ClientConnection(AquaSocket clientSocket);
    
    int getRegionID(Event* e);
    void getRegionInfo(int regionID, std::vector<EventProcessor*>& gestures, 
            std::vector<std::string>& events);
    void getGlobalInfo(std::vector<EventProcessor*>& globalGestures, 
            std::vector<std::string>& events);
    void getTranslators(std::vector<EventProcessor*>& translators, 
            EventProcessor* globalLayer);
    bool processEvent(Event* e);
    bool processEvent(Event* e, int regionID);
    int handleError(char* msg);
    char* receiveString(char* buffer);
    int receiveGestures(char* buffer, std::vector<EventProcessor*>& gestures, 
            EventProcessor* publisher, int regionID = -1);
    int receiveEvents(char* buffer, std::vector<std::string>& events);

};

#endif