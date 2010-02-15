/**
 * ClientConnection.h
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
    
    void  getGlobalInfo(std::vector<EventProcessor*>& globalGestures, 
                  std::vector<std::string>& events);
    int   getRegionID(Event* e);
    void  getRegionInfo(int regionID, std::vector<EventProcessor*>& gestures, 
                  std::vector<std::string>& events);
    void  getTranslators(std::vector<EventProcessor*>& translators, 
                  EventProcessor* globalLayer);
    int   handleError(char* msg);
    bool  processEvent(Event* e);
    bool  processEvent(Event* e, int regionID);
    int   receiveEvents(char* buffer, std::vector<std::string>& events);
    int   receiveGestures(char* buffer, std::vector<EventProcessor*>& gestures, 
                  EventProcessor* publisher, int regionID = -1);
    char* receiveString(char* buffer);
};
#endif
