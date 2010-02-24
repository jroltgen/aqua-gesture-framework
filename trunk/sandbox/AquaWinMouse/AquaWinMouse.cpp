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
#include "UnifiedEvent.h"

#define SERVER_PORT "3007"

using namespace std;

const char eventName[] = "UnifiedEvent";
const char eventDesc[] = "MouseEvent";
const char eventType = EVENT_TYPE_HOVER;


SOCKET sock;
bool leftDown = false;
bool rightDown = false;

int initSocket();       
LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

int main(int argc, char* argv[]) {
    
    // Connect to gesture server
    if (initSocket() != 0) {
        return 0;
    }

    // Set mouse hook
    HHOOK mouseHook = SetWindowsHookEx(
                  WH_MOUSE_LL,      /* Type of hook */
                  mouseHookProc,    /* Hook process */
                  NULL,             /* Instance */
                  NULL);
    // Wait for user to exit
    MessageBox(NULL, "Aqua mouse driver", "", MB_OK);
    return 0;
}



int initSocket() {
    WSADATA data;
    struct addrinfo *result = NULL;
    struct addrinfo *ptr    = NULL;
    struct addrinfo  hints;
    int iResult;
    char deviceType = 0;
    
    
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

    // Send our device type - 0, input device.
    iResult = send(sock, &deviceType, 1, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
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
    return 0;
}

void send(int id, char type, float* location) {
    Event* ev = new UnifiedEvent(string(eventName), string(eventDesc), type, id, location);
    sendEvent(ev);
    delete ev;
}


LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // Get event information
    PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT) lParam;

    float location[3];

    // Screen resolution
    static float screenWidth = static_cast<float>( GetSystemMetrics(SM_CXSCREEN) );
    static float screenHeight = static_cast<float>( GetSystemMetrics(SM_CYSCREEN) );

    // Mouse position
    location[0] = p->pt.x / screenWidth;
    location[1] = p->pt.y / screenHeight;
    location[2] = 0;
    
    // Type
    switch (wParam) {
    case WM_LBUTTONDOWN:
        printf("Left button down.\n");
        leftDown = true;
        send(1, EVENT_TYPE_DOWN, location);
        break;
    case WM_LBUTTONUP:
        printf("Left button up.\n");
        leftDown = false;
        send(1, EVENT_TYPE_UP, location);
        break;
    case WM_MOUSEMOVE:
        printf("Mouse move.\n");
        if (!leftDown && !rightDown) {
            send(0, EVENT_TYPE_HOVER, location);
        }
        if (leftDown) {
            send(1, EVENT_TYPE_MOVE, location);
        }
        if (rightDown) {
            send(2, EVENT_TYPE_MOVE, location);
        }
        break;
    case WM_MOUSEWHEEL:
        printf("Mouse wheel.\n");
        send(0, EVENT_TYPE_OTHER, location);
        break;
    case WM_RBUTTONDOWN:
        printf("Right button down.\n");
        rightDown = true;
        send(2, EVENT_TYPE_DOWN, location);
        break;
    case WM_RBUTTONUP:
        printf("Right button up.\n");
        rightDown = false;
        send(3, EVENT_TYPE_UP, location);
        break;
    default:
        printf("Other msg.\n");
        send(0, EVENT_TYPE_OTHER, location);
        break;
    }

    return (CallNextHookEx(NULL, nCode, wParam, lParam));
}

