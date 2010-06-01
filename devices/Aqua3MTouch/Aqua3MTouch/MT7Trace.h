/*******************************************************************************
Copyright 2009 3M. All rights reserved.

Program Name:  MT7Trace
Module Name:   MT7Trace.h

Summary: Application interface for MT 7 Software Trace library

Description:
   This header file defines the application interface for the MT 7 Software
   Trace library. This interface allows an application to enumerate touch
   sensors and obtain traces from the sensors.

Notes:

*******************************************************************************/

#ifndef __MT7TRACE_H__
#define __MT7TRACE_H__

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/
/* I N C L U D E   F I L E S                                                  */

/* Global Includes */

/* Project Includes */

/* Local Includes */

/******************************************************************************/
/* C O N S T A N T S                                                          */

#ifndef MT7TRACE
#if defined (WIN32) || defined (UNDER_CE)
#define MT7TRACE __declspec (dllimport)
#else
#define MT7TRACE
#endif
#endif


/******************************************************************************/
/* G L O B A L   D A T A   T Y P E S                                          */

typedef enum
{
   MT7TraceReason_Arrival = 1,
   MT7TraceReason_Departure = 2,
   MT7TraceReason_NewTrace = 3,
   MT7TraceReason_NewEvent = 4,
} MT7TraceReason;
typedef void (* MT7TraceCallback) (MT7TraceReason eReason, void * hItem,
                                   void * pvUserCode);
typedef enum
{
   MT7TraceEvent_Touchdown = 0,
   MT7TraceEvent_Drag = 1,
   MT7TraceEvent_Liftoff = 2,
} MT7TraceEventType;
typedef struct tagMT7TraceEnum * MT7TraceEnum;
typedef struct tagMT7TraceSensor * MT7TraceSensor;
typedef struct tagMT7TraceTrace * MT7TraceTrace;
typedef struct tagMT7TraceEvent * MT7TraceEvent;

/******************************************************************************/
/* P U B L I C   D A T A                                                      */


/******************************************************************************/
/* G L O B A L   D A T A                                                      */


/******************************************************************************/
/* G L O B A L   F U N C T I O N   P R O T O T Y P E S                        */


/*
** Enumeration API
*/
MT7TRACE MT7TraceEnum MT7TraceCreateEnum (void);
MT7TRACE unsigned int MT7TraceGetEnumID (MT7TraceEnum hEnumerator);
MT7TRACE MT7TraceEnum MT7TraceCloneEnum (MT7TraceEnum hEnumerator);
MT7TRACE MT7TraceSensor MT7TraceGetNextSensor (MT7TraceEnum hEnumerator);
MT7TRACE void MT7TraceReleaseEnum (MT7TraceEnum hEnumerator);

/*
** Sensor API
*/
MT7TRACE unsigned int MT7TraceGetSensorID (MT7TraceSensor hSensor);
MT7TRACE const char * MT7TraceGetSensorName (MT7TraceSensor hSensor);
MT7TRACE MT7TraceSensor MT7TraceCloneSensor (MT7TraceSensor hSensor);
MT7TRACE MT7TraceTrace MT7TraceGetNextTrace (MT7TraceSensor hSensor);
MT7TRACE void MT7TraceSetSensorCallback (MT7TraceSensor hSensor,
                                         MT7TraceCallback fnCallback,
                                         void * pvUserCode);
MT7TRACE void MT7TraceReleaseSensor (MT7TraceSensor hSensor);

/*
** Trace API
*/
MT7TRACE unsigned int MT7TraceGetTraceID (MT7TraceTrace hTrace);
MT7TRACE MT7TraceTrace MT7TraceCloneTrace (MT7TraceTrace hTrace);
MT7TRACE MT7TraceEvent MT7TraceGetNextEvent (MT7TraceTrace hTrace);
MT7TRACE MT7TraceEvent MT7TraceGetPriorEvent (MT7TraceTrace hTrace,
                                              unsigned int nOffset);
MT7TRACE void MT7TraceSetNextEvent (MT7TraceTrace hTrace,
                                    unsigned int nIndex);
MT7TRACE unsigned int MT7TraceGetEventCount (MT7TraceTrace hTrace);
MT7TRACE MT7TraceEvent MT7TraceGetEvent (MT7TraceTrace hTrace,
                                         unsigned int nIndex);
MT7TRACE MT7TraceSensor MT7TraceGetTraceSensor (MT7TraceTrace hTrace);
MT7TRACE void MT7TraceSetTraceCallback (MT7TraceTrace hTrace,
                                        MT7TraceCallback fnCallback,
                                        void * pvUserCode);
MT7TRACE void MT7TraceReleaseTrace (MT7TraceTrace hTrace);

/*
** Event API
*/
MT7TRACE unsigned int MT7TraceGetEventID (MT7TraceEvent hEvent);
MT7TRACE MT7TraceEvent MT7TraceCloneEvent (MT7TraceEvent hEvent);
MT7TRACE MT7TraceEventType MT7TraceGetEventType (MT7TraceEvent hEvent);
MT7TRACE float MT7TraceGetEventX (MT7TraceEvent hEvent);
MT7TRACE float MT7TraceGetEventY (MT7TraceEvent hEvent);
MT7TRACE unsigned int MT7TraceGetEventTime (MT7TraceEvent hEvent);
MT7TRACE MT7TraceTrace MT7TraceGetEventTrace (MT7TraceEvent hEvent);
MT7TRACE void MT7TraceReleaseEvent (MT7TraceEvent hEvent);

/*
** Monitor/Poll API
*/
MT7TRACE int MT7TracePoll (void);
MT7TRACE int MT7TraceStartMonitor (void);
MT7TRACE void MT7TraceStopMonitor (void);
MT7TRACE void MT7TraceSetArrivalCallback (MT7TraceCallback fnCallback,
                                          void * pvUserCode);
MT7TRACE void MT7TraceSetUniversalCallback (MT7TraceCallback fnCallback,
                                            void * pvUserCode);

/******************************************************************************/
/* G L O B A L   M A C R O S                                                  */

#ifdef __cplusplus
}
#endif

#endif /* ifndef __MT7TRACE_H__ */
