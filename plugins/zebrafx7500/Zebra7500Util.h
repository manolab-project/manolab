// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef linux
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//#include "rfidapiTestApp.h"
//#include "rfidTestInterface.h"
//#include "logger.h"
//extern CLogger *testLogger;
//#ifdef UNICODE
//#define LOG_MSG(...) {testLogger->logWriteW(LOG_LEVEL_INFO, __VA_ARGS__); /*assert(0);*/ }
//#else

//#endif
// TODO: reference additional headers your program requires here
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")


// TODO: reference additional headers your program requires here
#else
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include <stdarg.h>
#include <list>
#include "rfidapi.h"


#ifdef WIN32
#define sleep(s) Sleep(s)
#endif


RFID_STATUS ConnectReader(RFID_HANDLE32 *readerHandle,wchar_t *hostName,int readerPort);
RFID_STATUS ReaderCapability(RFID_HANDLE32);
RFID_STATUS SingulationControl(RFID_HANDLE32,LPSINGULATION_CONTROL);
RFID_STATUS	RebootReader(RFID_HANDLE32);
RFID_STATUS ConfigureAntennaMode(RFID_HANDLE32);
RFID_STATUS ConfigureReadPointStatus(RFID_HANDLE32);
RFID_STATUS ConfigureGPO(RFID_HANDLE32);
RFID_STATUS ConfigureGPI(RFID_HANDLE32);
RFID_STATUS ConfigureAntenna(RFID_HANDLE32);
RFID_STATUS ConfigureRFMode(RFID_HANDLE32);
RFID_STATUS SimpleInventory(RFID_HANDLE32);
RFID_STATUS PeriodicInventory(RFID_HANDLE32);
RFID_STATUS AddPreFilter(RFID_HANDLE32);
RFID_STATUS RemovePrefilter(RFID_HANDLE32);
RFID_STATUS PerformSingleTagAccess(RFID_HANDLE32);
RFID_STATUS MultipleTagAccess(RFID_HANDLE32);
RFID_STATUS SingleTagAccess(RFID_HANDLE32);
RFID_STATUS PerformMultipleTagAccess(RFID_HANDLE32);
RFID_STATUS ReadAccessSingleTag(RFID_HANDLE32);
RFID_STATUS WriteAccessSingleTag(RFID_HANDLE32);
RFID_STATUS LockAccessSingleTag(RFID_HANDLE32);
RFID_STATUS KillAccessSingleTag(RFID_HANDLE32);
RFID_STATUS ReadAccessMultipleTags(RFID_HANDLE32);
RFID_STATUS WriteAccessMultipleTags(RFID_HANDLE32);
RFID_STATUS LockAccessMultipleTags(RFID_HANDLE32);
RFID_STATUS KillAccessMultipleTags(RFID_HANDLE32);

void * ProcessRfidEventsThread(void * pvarg);
void * AwaitRfidWin32EventsThread(void * pvarg);


void printTagDataWithResults(TAG_DATA *pTagData);
void CreateEventThread(RFID_HANDLE32 readerHandle);
void KillEventThread();
tm SYSTEMTIME2tm(SYSTEMTIME *s);
void GetLocalTime(SYSTEMTIME * pSystemTime);
void clean_stdin(void);
void HandleResult(RFID_HANDLE32 readerHandle, RFID_STATUS rfidStatus);

#ifdef linux
#define rfid_swprintf swprintf
#else
#define rfid_swprintf(x, y, z, ...) swprintf(x, z, __VA_ARGS__)
#endif
