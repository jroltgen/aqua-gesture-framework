#include "GestureFactory.h"

// Main function - used for testing
int main(int argc, char* argv[]) {

    Gesture* myGesture;
    
    GestureFactory* myFactory = GestureFactory::getInstance();
    myFactory->loadGestures(argv[1]);
    
    myGesture = myFactory->createGesture("HelloWorldGesture.dll", NULL, 1337);
    if (myGesture) {
        myGesture->processEvent(NULL);
        delete myGesture;
    }
    
    myGesture = myFactory->createGesture("YouRockGesture.dll", NULL);
    if (myGesture) {
        myGesture->processEvent(NULL);
        delete myGesture;
    }
    return 0;
}