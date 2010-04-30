#define _WIN32_WINNT 0x0500

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "EndianConverter.h"
#include "Event.h"

#define MSG_GET_REGION_ID 		  0
#define MSG_GET_GLOBAL_INFO 	  1
#define MSG_GET_REGION_INFO 	  2
#define MSG_PROCESS_GLOBAL_EVENT  3
#define MSG_PROCESS_REGION_EVENT  4
#define MSG_GET_EVENT_TRANSLATORS 5

#define SERVER_PORT "3007"

using namespace std;

const char eventName[] = "UnifiedEvent";
const char eventDesc[] = "MouseEvent";
const char eventType = EVENT_TYPE_HOVER;


SOCKET sock;
bool leftDown = false;
bool rightDown = false;
char buffer[512];

int  initSocket();
void readMessages();
void getRegionID();
void getGlobalInfo();
void getRegionInfo();
void processGlobalEvent();
void processRegionEvent();
void getEventTranslators();

void socketError(char* msg);

int main(int argc, char* argv[]) {
    
    // Connect to gesture server
    if (initSocket() != 0) {
        printf("Could not connect to gesture server.\n");
        return 0;
    }

	readMessages();
    return 0;
}

void readMessages() {
    while(true) {
        char msgType;
        if (recv(sock, buffer, 1, NULL) != 1) {
            printf("Error receiving message type.\n");
            return;
        }
        memcpy(&msgType, buffer, 1);

        switch(msgType) {
		case MSG_GET_REGION_ID:
			getRegionID();
			break;
		case MSG_GET_GLOBAL_INFO:
			getGlobalInfo();
			break;
		case MSG_GET_REGION_INFO:
			getRegionInfo();
			break;
		case MSG_PROCESS_GLOBAL_EVENT:
			processGlobalEvent();
			break;
		case MSG_PROCESS_REGION_EVENT:
			processRegionEvent();
			break;
		case MSG_GET_EVENT_TRANSLATORS:
			getEventTranslators();
			break;
		default:
			printf("Message type not recognized: %d\n", msgType);
			exit(-1);
			break;
        }
    }
}

void getRegionInfo() {
	// No region gestures or events.
	int numGestures = 0;
	int numEvents = 0;
    int regionID;

    char buffer[512];

    if (recv(sock, buffer, 4, NULL) != 4) {
        socketError("Error reading regionID in getRegionInfo.\n");
    }
    memcpy(&regionID, buffer, 4);
    if (EndianConverter::isLittleEndian()) {
        regionID = EndianConverter::swapIntEndian(regionID);
    }

	printf("Getting region info for id %d.\n", regionID);

	if (EndianConverter::isLittleEndian()) {
		numGestures = EndianConverter::swapIntEndian(numGestures);
		numEvents = EndianConverter::swapIntEndian(numEvents);
	}

	send(sock, (char*) &numGestures, 4, NULL);
	send(sock, (char*) &numEvents, 4, NULL);
}

void getGlobalInfo() {
	// Write that we want the hello world gesture, no events.
	int numGestures = 1;
	int numEvents = 0;

	char helloWorld[18] = "PrintEventGesture";

	printf("Getting global info.\n");

	if (EndianConverter::isLittleEndian()) {
		numGestures = EndianConverter::swapIntEndian(numGestures);
		numEvents = EndianConverter::swapIntEndian(numEvents);
	}
	send(sock, (char*) &numGestures, 4, NULL);
	send(sock, helloWorld, 18, NULL);

	// Events.
    send(sock, (char*) &numEvents, 4, NULL);
}

void getRegionID() {
    char buffer[512];
	float location[3];

	int length = 12;
	int remaining = 12;
	while (remaining > 0) {
		int bytesRead = recv(sock, &buffer[length - remaining], 
				remaining, NULL);
		if (bytesRead < 0) {
			printf("Bytes read: %d\n", bytesRead);
			socketError("Reading location in getRegionID");
		}

		remaining -= bytesRead;
	}
    memcpy(location, buffer, 12);
	if (EndianConverter::isLittleEndian()) {
		for (int i = 0; i < 3; i++) {
			location[i] = EndianConverter::swapFloatEndian(location[i]);
		}
	}
	// For now, everything will be declared region 2.
	int regionID = 2;
    printf("location: %f, %f\n", location[0], location[1]);

	if (EndianConverter::isLittleEndian()) {
		regionID = EndianConverter::swapIntEndian(regionID);
	}
	send(sock, (char*) &regionID, 4, NULL);
}

void processGlobalEvent() {
	// Length comes in first
	short length;
    printf("Processing global event.\n");
	if (recv(sock, (char*) &length, 2, NULL) != 2) {
		socketError("Reading length in processGlobalEvent");
	}
	if (EndianConverter::isLittleEndian()) {
		length = EndianConverter::swapShortEndian(length);
	}

	// Now read the event.
	char* data = (char*) malloc(length * sizeof(char));

	int remaining = length;
	while (remaining > 0) {
		int bytesRead = recv(sock, (char*) &data[length - remaining], 
				remaining, NULL);
		if (bytesRead < 0) {
			socketError("Reading event data in processGlobalEvent");
		}
		remaining -= bytesRead;
	}

	// Print the event name.
	printf("Received a global event: %s\n", data);

	delete(data);
}

void processRegionEvent() {
	// Region ID comes in first
	int regionID;
    printf("Processing region event.\n");
	if (recv(sock, (char*) &regionID, 4, NULL) != 4) {
		socketError("Reading Region ID in processRegionEvent.");
	}

	// Length comes in second
	short length;
	if (recv(sock, (char*) &length, 2, NULL) != 2) {
		socketError("Reading length in processRegionEvent.");
	}

	// Endian conversion
	if (EndianConverter::isLittleEndian()) {
		length = EndianConverter::swapShortEndian(length);
		regionID = EndianConverter::swapIntEndian(regionID);
	}

	// Now read the event.
	char* data = (char*) malloc(length * sizeof(char));

	int remaining = length;
	while (remaining > 0) {
		int bytesRead = recv(sock, &data[length - remaining], 
				remaining, NULL);
		if (bytesRead < 0) {
			socketError("Reading event data in processRegionEvent");
		}
		remaining -= bytesRead;
	}

	// Print the event name.
	printf("Received a region event for region %d: %s\n", regionID, data);

	delete(data);
}

void getEventTranslators() {
    
    int numGestures = 0;
	printf("Getting event translators.\n");

	if (EndianConverter::isLittleEndian()) {
		numGestures = EndianConverter::swapIntEndian(numGestures);
	}

	send(sock, (char*) &numGestures, 4, NULL);
}

void socketError(char* msg) {
	printf("Error reading from socket: %s, quitting.\n", msg);
	exit(-1);
}


int initSocket() {
    WSADATA data;
    struct addrinfo *result = NULL;
    struct addrinfo *ptr    = NULL;
    struct addrinfo  hints;
    int iResult;
    char deviceType = 3;
    
    
    iResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (iResult != 0) {
        printf("Error starting WSA\n");
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", SERVER_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    sock = INVALID_SOCKET;
    
    // Attempt to connect to the first address returned by addrinfo
    ptr = result;
    sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (sock == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    
    iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("Error connecting to socket.\n");
        closesocket(sock);
        sock = INVALID_SOCKET;
        return 1;
    }

    freeaddrinfo(result);
    if (sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send our device type - 2, input device.
    iResult = send(sock, &deviceType, 1, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    return 0;
}