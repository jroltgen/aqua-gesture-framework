#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <string>

#include "AquaConnection.h"
#include "UnifiedEvent.h"
#include "UnifiedZoomEvent.h"

#define LEFT_ID 1
#define RIGHT_ID 2

#define ZOOM_IN 1
#define ZOOM_OUT 2


//http://tronche.com/gui/x/xlib/input/XGrabPointer.html

using namespace std;

AquaConnection 	conn;

void sendMove(int buttonID, float* location) {
	string evName("UnifiedEvent");
	string evDesc("Mouse event");
	UnifiedEvent ue(evName, evDesc, EVENT_TYPE_MOVE, buttonID, location);
	conn.sendEvent(&ue);
}

void sendZoom(int direction, float* location) {
	string zoomName("UnifiedZoomEvent");
	string zoomDesc("Mouse Zoom");

	float scale;
    if (direction == ZOOM_IN) {
        scale = 1.05;
    } else {
        scale = 0.95;
    }
	UnifiedZoomEvent ze(zoomName, zoomDesc, EVENT_TYPE_OTHER, 
			0, location, scale, location);
	conn.sendEvent(&ze);
}

void sendButton(int buttonID, int type, float* location) {
	string evName("UnifiedEvent");
	string evDesc("Mouse event");
	UnifiedEvent ue(evName, evDesc, type, buttonID, location);
	conn.sendEvent(&ue);
}


int main(int argc, char* argv[]) {

	Display* 		dis;
	Window   		root;
	XEvent   		ev;
	bool 			leftDown = false;
	bool			rightDown = false;
	int				displayWidth;
	int				displayHeight;

	if (conn.connect() != 0) {
		printf("Could not connect to server.\n");
		return -1;
	}
	
	

	dis = XOpenDisplay(NULL);
	displayWidth = DisplayWidth(dis, DefaultScreen(dis));
	displayHeight = DisplayHeight(dis, DefaultScreen(dis));

	printf("%d x %d\n", displayWidth, displayHeight);
	root = DefaultRootWindow(dis);
	XFlush(dis);

	while (true) {
	    XGrabPointer(dis, root, false, ButtonPressMask | ButtonReleaseMask |
			    ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None,
			    CurrentTime);
		XNextEvent(dis, &ev);
	    XUngrabPointer(dis, CurrentTime);
		float location[3];
		
		location[0] = ev.xbutton.x / (float) displayWidth;
		location[1] = ev.xbutton.y / (float) displayHeight;
		location[2] = 0.0;

		if (ev.type == ButtonPress) {
			switch(ev.xbutton.button) {
			case 1:
				printf("Left button down.\n");
				leftDown = true;
				sendButton(LEFT_ID, EVENT_TYPE_DOWN, location);
				break;
			case 3:
				printf("Right button down.\n");
				rightDown = true;
				sendButton(RIGHT_ID, EVENT_TYPE_DOWN, location);
				break;
			case 4:
				printf("Scroll up.\n");
				sendZoom(ZOOM_IN, location);
				break;
			case 5:
				printf("Scroll down.\n");
				sendZoom(ZOOM_OUT, location);
				break;
			default:
				break;
			}
				
		} else if (ev.type == ButtonRelease) {
			switch(ev.xbutton.button) {
			case 1:
				printf("Left button up.\n");
				leftDown = false;
				sendButton(LEFT_ID, EVENT_TYPE_UP, location);
				break;
			case 3:
				printf("Right button up.\n");
				rightDown = false;
				sendButton(RIGHT_ID, EVENT_TYPE_UP, location);
				break;
			default:
				break;
			}
		} else if (ev.type == MotionNotify) {
            printf("Move.\n");
			if (leftDown) {
				sendMove(LEFT_ID, location);
			}
			if (rightDown) {
				sendMove(RIGHT_ID, location);
			}
		}
	}

	return 0;
}
