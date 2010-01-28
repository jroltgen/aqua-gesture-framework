/**
 * main.cpp
 * 
 * The entry point for Aqua.  This class also contains numerous tests of
 * individual modules which can be compiled in or out using the flags below.
 * 
 * Written as part of the Aqua Universal Gesture Recognition Framework.
 * Licensed under the LGPL.
 * 
 * Jay Roltgen, 2010
 */
#include <stdio.h>
#include <string>

#include "GestureFactory.h"
#include "GestureServer.h"

// Test switches - use these to choose a main method with which to test 
// individual modules.  Only one of these may be uncommented at a time.
#define _GESTURESERVER_TEST_
//#define _GESTUREFACTORY_TEST_
//#define _AQUA_

using namespace std;

#ifdef _AQUA_
int main(int argc, char* argv[]) {
    printf("[AQUA] Hello, World!\n");
    return 0;
}
#endif

#ifdef _GESTURESERVER_TEST_
int main(int argc, char* argv[]) {
    GestureServer s;
    s.run();
    return 0;
}
#endif

#ifdef _GESTUREFACTORY_TEST_
int main(int argc, char* argv[]) {

    Gesture* myGesture;
    
    GestureFactory* myFactory = GestureFactory::getInstance();
    myFactory->loadGestures();
    
    
    myGesture = myFactory->createGesture(string("HelloWorldGesture"), /***/, 1337);
    if (myGesture) {
        myGesture->processEvent(NULL);
        delete myGesture;
    }
    
    myGesture = myFactory->createGesture(string("YouRockGesture"), /***/);
    if (myGesture) {
        myGesture->processEvent(NULL);
        delete myGesture;
    }
    return 0;
}
#endif