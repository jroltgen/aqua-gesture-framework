#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

//http://tronche.com/gui/x/xlib/input/XGrabPointer.html

int main(int argc, char* argv[]) {

	Display* dis;
	Window   root;
	XEvent   ev;


	dis = XOpenDisplay(NULL);

	root = DefaultRootWindow(dis);
	XFlush(dis);
	XGrabPointer(dis, root, false, ButtonPressMask | ButtonReleaseMask |
			ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None,
			CurrentTime);
	while (true) {
		XNextEvent(dis, &ev);
		if (ev.type == ButtonPress) {
			printf("Got press event: %d!\n", ev.xbutton.button);
		} else if (ev.type == ButtonRelease) {
			printf("Got press event: %d!\n", ev.xbutton.button);
		} else if (ev.type == MotionNotify) {
			printf("Got move event: %d, %d\n", ev.xmotion.x, ev.xmotion.y);
		}
	}

	XUngrabPointer(dis, CurrentTime);
	//XUngrabButton(dis, CurrentTime, );
	return 0;
}
