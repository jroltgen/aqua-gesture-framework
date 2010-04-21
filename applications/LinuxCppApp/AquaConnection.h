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

#define MSG_GET_REGION_ID 		  0
#define MSG_GET_GLOBAL_INFO 	  1
#define MSG_GET_REGION_INFO 	  2
#define MSG_PROCESS_GLOBAL_EVENT  3
#define MSG_PROCESS_REGION_EVENT  4
#define MSG_GET_EVENT_TRANSLATORS 5

class AquaConnection {
	
// Attributes
private:
	int _socket;

// Methods
public:
	int  connect();
    void readMessages();
	void run();

private:	
	void getRegionID();
	void getGlobalInfo();
	void getRegionInfo();
	void processGlobalEvent();
	void processRegionEvent();
	void getEventTranslators();

	void socketError(char* msg);

};
