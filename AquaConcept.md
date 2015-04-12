## Concept ##
AQUA-G is a universal input device and gesture handling system.  It has a unified event architecture that allows for a great deal of flexibility in gesture recognition algorithms, input devices, and client applications.

AQUA-G is designed with developers in mind, and utilizes shared libraries to dynamically load gestures and events.  Developers can easily add new events and gestures to the system, because it is not necessary to recompile the entire system to add a new event, gesture, or input device.

> ![http://www.vrac.iastate.edu/~jroltgen/AquaConcept.png](http://www.vrac.iastate.edu/~jroltgen/AquaConcept.png)

Since AQUA-G is designed to be compatible with as many input devices and programming languages is possible, communication to AQUA-G is performed through TCP sockets. AQUA-G will be compatible with Windows, Linux and MAC OS, though currently I'm just working on the Windows and Linux versions.

## Why should you use AQUA-G? ##
AQUA-G will be open source software (LGPL), so you are free to modify the code as you choose.  Additionally, it will be compatible with a variety of input devices.  Ideally, as the project grows, the community will contribute input device drivers for AQUA-G, increasing the probability that your input device will work with Aqua out-of-the-box.  Examples include multitouch screens, mice & keyboard input, 3D time-of-flight cameras, Wiimotes, game controllers, etc.

AQUA-G allows for the possibility of multi-modal gestural interaction, because you may connect more than one input device at the same time.  Tangible UIs will also fit nicely into this architecture.  It also allows multiple simultaneous users, each with his or her own set of input devices.

Because AQUA-G has a unified event architecture, many gestures will work with a variety of input devices out-of-the-box.  You configure your input device to provide the appropriate events for the gestures you desire to receive, and you will instantly receive gesture events.

Finally, AQUA-G is designed to allow for easy integration of existing gesture recognition algorithms.  I hope that developers will contribute many and varied forms of gesture recognition that are already available, increasing the usefulness of AQUA-G to software developers wishing to easily integrate gestural interaction into their new or existing software applications.