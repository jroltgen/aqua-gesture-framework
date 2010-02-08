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

class ClientConnection : public EventProcessor {

// Attributes
private:
    AquaSocket _clientSocket;

// Methods
public:
    ClientConnection(AquaSocket clientSocket);
    
    int getRegionID(Event* e);
    void getRegionInfo(int regionID, std::vector<Gesture*>& gestures, 
            std::vector<std::string>& events);
    void getGlobalInfo(std::vector<EventProcessor*>& globalGestures, 
            std::vector<std::string>& events);
    bool processEvent(Event* e);
    bool processEvent(Event* e, int regionID);

};

#endif