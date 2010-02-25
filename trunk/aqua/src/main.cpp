/**
 * main.cpp
 * 
 * The entry point for Aqua.  This class also contains numerous tests of
 * individual modules which can be compiled in or out using the flags below.
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
#include <string>

#include "events/EventFactory.h"
#include "events/Event.h"
#include "gestures/GestureFactory.h"
#include "GestureServer.h"

// Test switches - use these to choose a main method with which to test 
// individual modules.  Only one of these may be uncommented at a time.
//#define _GESTURESERVER_TEST_
#define _GESTUREFACTORY_TEST_
//#define _AQUA_
//#define _SOCKET_TEST_
//#define _EVENTFACTORY_TEST_
//#define _UNIFIEDDRAGGESTURE_TEST_

using namespace std;

#ifdef _AQUA_
int main(int argc, char* argv[]) {
    GestureServer s;
    s.run();
}
#endif

#ifdef _UNIFIEDDRAGGESTURE_TEST_
#include "gestures/unifiedGestures/UnifiedDragGesture.h"
#include "events/UnifiedEvent.h"
int main(int argc, char* argv[]) {
	GestureServer s;
	GestureFactory::getInstance()->loadGestures();
	Gesture* g = GestureFactory::getInstance()->createGesture(string("UnifiedDragGesture"), s);
	
	std::string n("TestEvent");
	std::string d("Test");
	float l[3];
    float l2[3];
    float l3[3];
	
    
	l[0] = 0.5;
	l[1] = 0.5;
	l[2] = 0.5;
    
    l2[0] = 0.9;
    l2[1] = 0.7;
    l2[2] = 0.1;
    
    l3[0] = 0.7;
    l3[1] = 0.6;
    l3[2] = 0.3;
	
	UnifiedEvent* e;
	
	e = new UnifiedEvent(n, d, EVENT_TYPE_DOWN, 1, l);
	g->processEvent(e);
	delete e;
    
    e = new UnifiedEvent(n, d, EVENT_TYPE_DOWN, 2, l2);
    g->processEvent(e);
    delete e;
    
    e = new UnifiedEvent(n, d, EVENT_TYPE_MOVE, 2, l3);
    g->processEvent(e);
    delete e;
	
	e = new UnifiedEvent(n, d, EVENT_TYPE_UP, 1, l);
	g->processEvent(e);
	delete e;
    
    e = new UnifiedEvent(n, d, EVENT_TYPE_UP, 2, l3);
    g->processEvent(e);
    delete e;
	
	return 0;
}
	
#endif

#ifdef _GESTURESERVER_TEST_
int main(int argc, char* argv[]) {
    GestureServer s;
	EventFactory::getInstance();
	GestureFactory::getInstance();
    s.run();
    return 0;
}
#endif

#ifdef _EVENTFACTORY_TEST_
int main(int argc, char* argv[]) {
    Event* myEvent;
    char data[20];
    
    data[1] = '\0';
    data[2] = '\0';
    
    EventFactory* myFactory = EventFactory::getInstance();
    
    myFactory->loadEvents();
    printf("Events loaded\n");
    
    
    myEvent = myFactory->createEvent(string("SimpleEvent"), data);
    
    
    return 0;

}
#endif

#ifdef _GESTUREFACTORY_TEST_
int main(int argc, char* argv[]) {

    Gesture* myGesture;
    GestureServer s;
    
    GestureFactory* myFactory = GestureFactory::getInstance();
    myFactory->loadGestures();
    printf("Loaded.\n");
    string name("HelloWorldGesture");
    
    myGesture = myFactory->createGesture(name, s, 1337);
    if (myGesture) {
        myGesture->processEvent(NULL);
        delete myGesture;
    }
    
    name = "YouRockGesture";
    myGesture = myFactory->createGesture(name, s);
    if (myGesture) {
        myGesture->processEvent(NULL);
        delete myGesture;
    }
    return 0;
}
#endif

#ifdef _SOCKET_TEST_
#include "utils/AquaSocket.h"

int main(int argc, char* argv[]) {
    AquaSocket serverSocket;
    AquaSocket clientSocket;
    char* hostName = "localhost";
    char* port = "3007";
    int result;
    bool ok = true;
    bool socketUp = true;
    char buffer = 'A';
    
    if (serverSocket.bind(hostName, port) < AQUASOCKET_RES_OK) {
        printf("Error in bind.\n");
        return 0;
    }
    
    do {
        if (serverSocket.listen() < AQUASOCKET_RES_OK) {
            printf("Error in listen.\n");
            continue;
        }
        
        clientSocket = serverSocket.accept();
        if (!clientSocket.isValid()) {
            printf("Accept failed.\n");
            continue;
        }
        
        do {
            result = clientSocket.recv(&buffer, 1);
            if (result != 1) {
                printf("Receive failed: %d\n", result);
                socketUp = false;
            }
            printf("Received character: %c\n", buffer);
            result = clientSocket.send(&buffer, 1);
            if (result != 1) {
                printf("Send failed %d\n", result);
                socketUp = false;
            }
            buffer += buffer > 255 ? -255 : 1;
            printf("Sent character: %c\n", buffer);
        
        } while (socketUp);

    } while (ok);

}
#endif
