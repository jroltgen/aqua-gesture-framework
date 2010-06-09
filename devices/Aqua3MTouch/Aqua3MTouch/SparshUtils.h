#ifndef _SPARSHUTILS_H_
#define _SPARSHUTILS_H_

#define SERVER_PORT "3007"
#define _WIN32_WINNT 0x0500
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "MT7Trace.h"
#include "EndianConverter.h"

struct sparsh_touchpoint {
	int _id;    //Integer id which uniquely represent a touch point. *NETWORK ENDIAN *
	float _x;   //normalized value of x-co-ordinate  *NETWORK ENDIAN *
    float _y;   //normalized value of y-co-ordinate  *NETWORK ENDIAN *
    char _type; //this is the touch point state
};

enum Touch_Point_State {
    POINT_BIRTH,
    POINT_DEATH,
    POINT_MOVE
};

int initSocket();

void fingerDown(MT7TraceTrace hTrace, MT7TraceEvent hEvent);
void fingerMove(MT7TraceTrace hTrace, MT7TraceEvent hEvent);
void fingerUp(MT7TraceTrace hTrace, MT7TraceEvent hEvent);
void sendPoint(sparsh_touchpoint *tp);

#endif