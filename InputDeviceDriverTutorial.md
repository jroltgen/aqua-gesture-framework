**Note:** I am still working on putting the sample input device project together.

# Aqua Tutorial - Writing an Input Device Driver #

<font face='arial' size='3'>
Aqua is designed to communicate with a limitless variety of input devices.  In order for it to do this, it communicates with input devices using a custom serialization protocol on a TCP/IP internet socket.  Writing a new input device driver is made easier, however, by using the Event classes already provided with AQUA.  These classes already have built-in serialization functionality which you may use in your device driver.<br>
<br>
As stated above, Aqua communicates with input devices via TCP/IP sockets.  Therefore, in order to communicate with Aqua, your input device driver must create a TCP socket.  Aqua listens for connections on port 3007.  Then, you must send a single byte with value 0x02, which will identify you as an input device Here is code to do this in C++ using windows sockets:<br>
</font>

```
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET sock;

WSADATA data;
struct addrinfo *result = NULL;
struct addrinfo *ptr    = NULL;
struct addrinfo  hints;
int iResult;
char deviceType = 2;
    
    
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
```

<font face='arial' size='3'>
Once this initialization is complete, it is quite easy to send events to Aqua.  In the Event class, a single method is defined:<br>
</font>

```
char* serialize(int& outLength)
```

<font face='arial' size='3'>
This method returns a pointer to an array of bytes which contains the event data, as well as an integer “outLength” which contains the length of this array.<br>
<br>
To send the event to Aqua, simply send a short int containing this length, followed by the data.  Below is example code which demonstrates how to do this.<br>
</font>

```
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
    iResult = send(sock, outLength, 2, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send length failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(0);
        return -1;
    }
    
    // Send the event itself.
    iResult = send(sock, eventData, iOutLength, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send data failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(0);
        return -1;
    }
    
    delete[] eventData;
    return 0;
}
```

# What do I do next? #

<font face='arial' size='3'>
In order to develop an input device driver, it will be beneficial to review example code.  There is a seed project for creating a new input device driver in both Windows and Linux available for download on the “Downloads” page.  The example project is the Windows Mouse input device driver.  Other example device drivers may be found by browsing the source code.<br>
</font>