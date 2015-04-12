**Note:** I am still working on putting the sample gesture project together, so you will not find the sample gesture project referenced in this document _yet_.

# Aqua Tutorial - Writing a Custom Gesture #

<font face='arial' size='3'>
Allowing developers the ability to write their own custom gestures was one of the most important ideas in developing the design and architecture for Aqua.  Aqua makes it very easy for you as a developer to write your own custom gesture.<br>
<br>
One of the best things about Aqua is that gestures are dynamically loaded through the use of shared libraries.  When you create a gesture, you compile it to a .dll file on Windows or a .so file on Linux.  The sample gesture project which you may download already does this.  The Aqua gesture server recognizes these files and loads your gesture code at run-time.  This means that you don't have to look at a single line of Aqua code to write your own gesture.<br>
<br>
Writing your own gesture involves the following requirements:<br>
<ul><li>Your gesture must subclass the Gesture class defined in Gesture.h.<br>
</li><li>Your gesture class name must exactly match that of the compiled library.  You should define a class “MyGesture” which gets compiled to “MyGesture.dll” or “MyGesture.so”<br>
</li><li>Your gesture class must export a function “createGesture” which is used by Aqua to load your class dynamically.<br>
</li><li>You must override the method “handleEvent” in the Gesture class – this is where your gesture can process incoming events.<br>
</li><li>You must place your compiled library into a place where Aqua can find it, in (Aqua_exe_home)/gestures.  You will find all of the included gestures in this directory.</li></ul>

The sample gesture project shows you the basics of creating a gesture for yourself.  In this tutorial, we will describe the simple “HelloWorldGesture” which is provided in Aqua.<br>
<br>
Here is the header file for this class:<br>
</font>

```
#ifndef _HELLOWORLDGESTURE_H_
#define _HELLOWORLDGESTURE_H_

#include "../events/Event.h"
#include "Gesture.h"  

class HelloWorldGesture : public Gesture {

// Methods
public:
    HelloWorldGesture(EventProcessor* publisher, int regionID = -1) :
            Gesture(publisher, regionID) {};
    virtual bool handleEvent(Event* event);
    
private:
    void printHello();
};
#endif
```

<font face='arial' size='3'>
Notice how short this class is.  It declares the HelloWorldGesture class, which subclasses the Gesture class.  It then declares a constructor which calls the Gesture superclass constructor.  You should provide a constructor with this same signature in your gesture.  The EventProcessor object is the EventProcessor that you will send events that your gesture generates to, using the protected sendEvent method of the Gesture class.  This is described below.<br>
<br>
Additionally, you need to export functions which are used by Aqua to load your class.  These can be placed in the header file or the cpp file.  Here are these functions for the HelloWorldGesture:<br>
</font>

```
#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
        return new HelloWorldGesture(publisher, regionID);
    }
}
#else
extern "C" {
    Gesture* createGesture(EventProcessor* publisher, int regionID) {
        return new HelloWorldGesture(publisher, regionID);
    }
}
#endif
```

<font face='arial' size='3'>
You will have to modify the return statement so that it returns an instance of your new gesture, but otherwise these methods should remain the same.<br>
<br>
Here is the implementation cpp file for HelloWorldGesture:<br>
</font>

```
#include <stdio.h>
#include "HelloWorldGesture.h"

bool HelloWorldGesture::handleEvent(Event* event) {
    printHello();
    publishEvent(event)
    return false;
}

void HelloWorldGesture::printHello() {
    printf("Hello, world! My region ID is: %d\n", _regionID);
}
```

<font face='arial' size='3'>
The cpp file implements only one public method, handleEvent.  This is where all of your gesture processing code belongs.  HandleEvent is called by Aqua whenever a new event is available for your gesture to process.  In this simple gesture, we simply print “Hello, World” along with this gesture's regionID to the console, and publish the same event we received.  However, more complex behavior can easily be defined.  For example, the UnifiedZoomGesture handles events, processes them, creates instances of UnifiiedZoomEvent, and publishes these new events.  Aqua then sends these events to the application.  Please refer to the sample gesture project for more details and examples.<br>
</font>

# What do I do next? #

<font face='arial' size='3'>
That's it!  Now that you've read the tutorial, you are ready to begin developing your own custom gestures.  Download the sample gesture project, study it to gain a feel for how things work, then try customizing it or starting from scratch and creating your own gesture!  Remember, once you can compile your event, the dll or so file that is generated should be placed in (Aqua_exe_home)/gestures.<br>
<br>
If you run into trouble along the way, please go to the google group and post a message detailing the reason for your woes – we'll help as soon as we can.<br>
<br>
Good luck!<br>
</font>