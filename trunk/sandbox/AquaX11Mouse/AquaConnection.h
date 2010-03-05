/**
 * Aqua Connection.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "Event.h"

class AquaConnection {
	
// Attributes
private:
	int _socket;

// Methods
public:
	int connect() {
	
		struct hostent* 	_hostInfo;
		struct sockaddr_in 	_address;
		long 				_hostAddress;
		char buffer[1];

		buffer[0] = 0;
		
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

	/**
	 * Returns -1 if error, 0 on success.
	 */
	int sendEvent(Event* e) {
	    char outLength[2];
	    short iOutLength, tempLength;
	    int iResult;
	    
	    char* eventData = e->serialize(iOutLength);
	    tempLength = iOutLength;
	    if (EndianConverter::isLittleEndian()) {
	        tempLength = EndianConverter::swapShortEndian(tempLength);
	    }

	    // Send the length of the event
	    memcpy(outLength, &tempLength, 2);
	    iResult = write(_socket, outLength, 2);
	    
		if (iResult == -1) {
	        printf("Send length failed: %d\n", iResult);
	        exit(0);
	    }
	    
	    // Send the event itself.
	    iResult = write(_socket, eventData, iOutLength);
	    if (iResult == -1) {
	        exit(0);
	    }
	    return 0;
	}

};
