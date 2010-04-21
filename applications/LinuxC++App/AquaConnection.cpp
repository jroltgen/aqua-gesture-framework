#include <pthread.h>

#include "AquaConnection.h"
#include "EndianConverter.h"
#include "Event.h"

// C-style function for using in pthread_create.
extern "C" {
	void *runReadEvents(void* pThis) {
			((AquaConnection*)pThis)->readMessages();
	}
}

int main() {
	AquaConnection c;

	c.connect();
	c.run();
}

int AquaConnection::connect() {
	
	struct hostent* 	_hostInfo;
	struct sockaddr_in 	_address;
	long 				_hostAddress;
	char buffer[1];

	// This is our type - we are a client app connection, 0x03.
	buffer[0] = 3;
		
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (_socket == -1) {
		printf("Could not make a socket.\n");
		return - 1;
	}

	_hostInfo = gethostbyname("localhost");
	memcpy(&_hostAddress, _hostInfo->h_addr, _hostInfo->h_length);

	_address.sin_addr.s_addr = _hostAddress;
	_address.sin_port = htons(3007);
	_address.sin_family = AF_INET;

  	if (::connect(_socket, (struct sockaddr*) &_address, 
			sizeof(_address)) == -1) {
		printf("\nCould not connect to host\n");
		return -1;
    }
	write(_socket, buffer, 1);
	return 0;
}

void AquaConnection::run() {
	
	int 	  rc;
	pthread_t myThread;

	rc = pthread_create(&myThread, NULL, runReadEvents, (void*) this);

	if (rc != 0) {
		printf("[AquaConnection] Error starting read thread.\n");
		exit(-1);
	}

	pthread_join(myThread, NULL);
	
}

void AquaConnection::readMessages() {
	
	char msgType;

	while(true) {
		if (read(_socket, &msgType, 1) != 1) {
			printf("Error reading message type.\n");
			exit(-1);
		}
		
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

		//printf("Done processing message.\n");
	}
}

void AquaConnection::getRegionID() {
    char buffer[512];
	float location[3];

	int length = 12;
	int remaining = 12;
	while (remaining > 0) {
		int bytesRead = read(_socket, (void*) &buffer[length - remaining], 
				remaining);
		printf("Get region id bytes read: %d\n", bytesRead);
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

	if (EndianConverter::isLittleEndian()) {
		regionID = EndianConverter::swapIntEndian(regionID);
	}
	write(_socket, (void*) &regionID, 4);

}

void AquaConnection::getGlobalInfo() {
	// Write that we want the hello world gesture, no events.
	int numGestures = 1;
	int numEvents = 0;

	char helloWorld[18] = "PrintEventGesture";

	printf("Getting global info.\n");

	if (EndianConverter::isLittleEndian()) {
		numGestures = EndianConverter::swapIntEndian(numGestures);
		numEvents = EndianConverter::swapIntEndian(numEvents);
	}
	write(_socket, (void*) &numGestures, 4);
	write(_socket, (void*) helloWorld, 18);

	// Events.
	write(_socket, (void*) &numEvents, 4);
}

	
	
void AquaConnection::getRegionInfo() {
	// No region gestures or events.

	int numGestures = 0;
	int numEvents = 0;

	printf("Getting region info.\n");

	if (EndianConverter::isLittleEndian()) {
		numGestures = EndianConverter::swapIntEndian(numGestures);
		numEvents = EndianConverter::swapIntEndian(numEvents);
	}

	write(_socket, (void*) &numGestures, 4);
	write(_socket, (void*) &numEvents, 4);

}

/**
 * Reads a global event.
 */
void AquaConnection::processGlobalEvent() {
	// Length comes in first
	short length;
	if (read(_socket, (void*) &length, 2) != 2) {
		socketError("Reading length in processGlobalEvent");
	}
	if (EndianConverter::isLittleEndian()) {
		length = EndianConverter::swapShortEndian(length);
	}

	// Now read the event.
	char* data = (char*) malloc(length * sizeof(char));

	int remaining = length;
	while (remaining > 0) {
		int bytesRead = read(_socket, (void*) &data[length - remaining], 
				remaining);
		if (bytesRead < 0) {
			socketError("Reading event data in processGlobalEvent");
		}
		remaining -= bytesRead;
	}

	// Print the event name.
	printf("Received a global event: %s\n", data);

	delete(data);
	
}
void AquaConnection::processRegionEvent() {
	// Region ID comes in first
	int regionID;
	if (read(_socket, (void*) &regionID, 4) != 4) {
		socketError("Reading Region ID in processRegionEvent.");
	}

	// Length comes in second
	short length;
	if (read(_socket, (void*) &length, 2) != 2) {
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
		int bytesRead = read(_socket, (void*) &data[length - remaining], 
				remaining);
		if (bytesRead < 0) {
			socketError("Reading event data in processRegionEvent");
		}
		remaining -= bytesRead;
	}

	// Print the event name.
	printf("Received a region event for region %d: %s\n", regionID, data);

	delete(data);
}

/**
 * Not using any translators.
 */
void AquaConnection::getEventTranslators() {

	int numGestures = 0;

	printf("Getting event translators.\n");

	if (EndianConverter::isLittleEndian()) {
		numGestures = EndianConverter::swapIntEndian(numGestures);
	}

	write(_socket, (void*) &numGestures, 4);
}


void AquaConnection::socketError(char* msg) {
	printf("Error reading from socket: %s, quitting.\n", msg);
	exit(-1);
}




















