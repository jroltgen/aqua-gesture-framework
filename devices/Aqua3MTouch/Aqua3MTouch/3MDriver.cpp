#include <stdio.h>
#include <map>

#include "MT7Trace.h"
#include "SparshUtils.h"

std::map<int, int> traceIDtoEventCount;

void handleEvent(MT7TraceTrace hTrace, MT7TraceEvent hEvent);
void eventCallback(MT7TraceReason eReason, void* hItem, void* pvUserCode);


int main(int argc, char* argv[]) {
	initSocket();
	MT7TraceSetUniversalCallback(eventCallback, NULL);
	printf("Connection made to touchscreen.\n");
	MT7TraceStartMonitor();
	while (true) {
		Sleep(1000);
	}
	return 1;
}

void handleEvent(MT7TraceTrace hTrace, MT7TraceEvent hEvent) {
	MT7TraceEventType type = MT7TraceGetEventType(hEvent);
	traceIDtoEventCount[MT7TraceGetTraceID(hTrace)]++;
	switch(type) {
		case MT7TraceEvent_Touchdown:
			fingerDown(hTrace, hEvent);
			break;
		case MT7TraceEvent_Drag:
			fingerMove(hTrace, hEvent);
			break;
		case MT7TraceEvent_Liftoff:
			printf("Number of events: %d\n", traceIDtoEventCount[MT7TraceGetTraceID(hTrace)]);
			fingerUp(hTrace, hEvent);
			break;
	}

}

void eventCallback(MT7TraceReason eReason, void* hItem, void* pvUserCode) {
	MT7TraceTrace hTrace;
	MT7TraceEvent hEvent;

	switch (eReason) {
		// We don't care about sensor arrival or departure
		case MT7TraceReason_Arrival:
		case MT7TraceReason_Departure:
			MT7TraceReleaseSensor((MT7TraceSensor)hItem);
			break;

		// Have new traces on a sensor; call GetNextTrace and GetNextEvent
		// repeatedly
		case MT7TraceReason_NewTrace:
			printf("Got a new trace.\n");
			while ((hTrace = MT7TraceGetNextTrace((MT7TraceSensor)hItem)) != NULL) {
				// Insert 0 into the event count - it's a new trace.
				int traceID = MT7TraceGetTraceID(hTrace);
				traceIDtoEventCount.insert(std::pair<int,int>(traceID, 0));

				while ((hEvent = MT7TraceGetEvent(hTrace, traceIDtoEventCount[traceID])) != NULL) {
					handleEvent(hTrace, hEvent);
					MT7TraceReleaseEvent(hEvent);
				}

				//printf("Num events: %d\n", MT7TraceGetEventCount(hTrace));
				//MT7TraceSetNextEvent(hTrace, MT7TraceGetEventCount(hTrace)-1);
				MT7TraceReleaseTrace(hTrace);
			}
			MT7TraceReleaseSensor((MT7TraceSensor)hItem);
			break;
			
		// Have new events on a trace; call GetNextEvent repeatedly
		case MT7TraceReason_NewEvent:
			hTrace = (MT7TraceTrace)hItem;
			int traceID = MT7TraceGetTraceID(hTrace);

			while ((hEvent = MT7TraceGetEvent(hTrace, traceIDtoEventCount[traceID])) != NULL) {
				handleEvent(hTrace, hEvent);
				MT7TraceReleaseEvent(hEvent);
			}
			//printf("Num events: %d\n", MT7TraceGetEventCount((MT7TraceTrace)hItem));
			//MT7TraceSetNextEvent((MT7TraceTrace)hItem, MT7TraceGetEventCount((MT7TraceTrace)hItem)-1);
			break;
	}
}



