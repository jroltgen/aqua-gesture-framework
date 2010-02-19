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
//#define _GESTUREFACTORY_TEST_
#define _AQUA_
//#define _SOCKET_TEST_
//#define _EVENTFACTORY_TEST_

using namespace std;

#ifdef _AQUA_
int main(int argc, char* argv[]) {
    GestureServer s;
    s.run();
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
    
    myGesture = myFactory->createGesture(string("HelloWorldGesture"), s, 1337);
    if (myGesture) {
        myGesture->processEvent(NULL);
        delete myGesture;
    }
    
    myGesture = myFactory->createGesture(string("YouRockGesture"), s);
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