#include "SparshUtils.h"

SOCKET sock;

void fingerDown(MT7TraceTrace hTrace, MT7TraceEvent hEvent) {
    sparsh_touchpoint tp;
	MT7TraceTrace trace = MT7TraceGetEventTrace(hEvent);
	
	printf("Got down: %d, %f, %f\n", 
			MT7TraceGetTraceID(hTrace),
			MT7TraceGetEventX(hEvent), 1 - MT7TraceGetEventY(hEvent));
	tp._id = swapIntEndian(MT7TraceGetTraceID(hTrace)); 
	tp._x  = swapFloatEndian(MT7TraceGetEventX(hEvent)); 
    tp._y = swapFloatEndian(1 - MT7TraceGetEventY(hEvent));
    tp._type = POINT_BIRTH;
	
    //now send the point over a network
    sendPoint(&tp);       
 }

 void fingerMove(MT7TraceTrace hTrace, MT7TraceEvent hEvent) {
    sparsh_touchpoint tp;
	printf("Got move: %d, %f, %f\n", 
			MT7TraceGetTraceID(hTrace),
			MT7TraceGetEventX(hEvent), 1 - MT7TraceGetEventY(hEvent));
    tp._id = swapIntEndian(MT7TraceGetTraceID(hTrace));
    tp._x  = swapFloatEndian(MT7TraceGetEventX(hEvent));
    tp._y = swapFloatEndian(1 - MT7TraceGetEventY(hEvent));
    tp._type = POINT_MOVE;

    //now send the point over a network
    sendPoint(&tp);
 }

 //! A finger is no longer active..
void fingerUp(MT7TraceTrace hTrace, MT7TraceEvent hEvent) {
    sparsh_touchpoint tp;
	printf("Got up: %d, %f, %f\n", 
			MT7TraceGetTraceID(hTrace),
			MT7TraceGetEventX(hEvent), 1 - MT7TraceGetEventY(hEvent));
    tp._id = swapIntEndian(MT7TraceGetTraceID(hTrace));
    tp._x  = swapFloatEndian(MT7TraceGetEventX(hEvent));
    tp._y = swapFloatEndian(1 - MT7TraceGetEventY(hEvent));
    tp._type = POINT_DEATH;

    //now send the point over a network
    sendPoint(&tp); 
}

void sendPoint(sparsh_touchpoint *tp)
 {
   int struct_size = sizeof(int) + 2 * sizeof(float) + sizeof(char);
   int buffersize = 17;  // Second term for num points.
   char buffer[17];
   char* bufferptr = buffer;
   
   //first copy the size information
   //remember to convert to network endian before copying
   int numPoints = swapIntEndian(1);
   memcpy(bufferptr, &numPoints, sizeof(int)); 
   bufferptr += sizeof(int);
   
   //now copy the id
   memcpy(bufferptr, &(tp->_id), sizeof(int));
   bufferptr += sizeof(int);  
  
   //now copy the X co-ordinate
   memcpy(bufferptr, &(tp->_x), sizeof(float));
   bufferptr += sizeof(float);
 
   //now copy the Y co-ordinate
   memcpy(bufferptr, &(tp->_y), sizeof(float));
   bufferptr += sizeof(float);  

   //now copy the STATE
   memcpy(bufferptr, &(tp->_type), sizeof(char));
   bufferptr += sizeof(char);

   //Your buffer is now ready to be sent over the network
   // Sending the buffer over network
   int sent = send(sock, buffer, buffersize, 0);
   //for (int i = 0; i < 17; i++) {
	//   printf("%d ", buffer[i]);
   //}
   //printf("\n");
}

int initSocket() {
    WSADATA data;
    struct addrinfo *result = NULL;
    struct addrinfo *ptr    = NULL;
    struct addrinfo  hints;
    int iResult;
    char deviceType = 1;
    
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
        printf("Error connecting to Sparsh-UI.\n");
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

    // Send our device type - 1, input device.
    iResult = send(sock, &deviceType, 1, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    return 0;
}