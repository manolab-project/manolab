/*                Copyrights ©2010-2020 Zebra Technologies Corp.
 *                          All rights reserved
 * 
 *  
 *****************************************************************************
 */

/**
 *****************************************************************************
 ** 
 ** @file  rfidapiTypes.h
 ** 
 ** @brief <b> Data types </b> 
 ** 
 ** 
 ****************************************************************************/
#ifndef RFIDAPI_TYPES_H
#define RFIDAPI_TYPES_H

#if defined(__cplusplus)

extern "C"
{
#endif

#define MAX_TAGS_IN_FILTER	256

// Max number of extra triggers is 7, LLRP can support total 8 triggers, which are matched by priority value in ROSpec is 0~7.
#define MAX_NUM_EXTRA_TRIGGER	7

typedef signed   char       INT8;
typedef unsigned char       UINT8;
typedef signed   short      INT16;
typedef unsigned short      UINT16;
typedef signed   int	    INT32;
typedef unsigned int        UINT32;
typedef float               FLOAT;
typedef unsigned long       ULONG;

#ifdef WIN32
typedef signed   __int64    INT64;
typedef unsigned __int64    UINT64;
typedef unsigned __int64	RFID_HANDLE64;
typedef unsigned __int64	SOCKET_HANDLE;
#else
#include <inttypes.h> 
#include <wchar.h>
typedef wchar_t WCHAR;
#define MAX_PATH          260
typedef void *LPVOID;
typedef char CHAR;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef BYTE				BOOLEAN;
#define WINAPI //__attribute__((stdcall))
	typedef int64_t    INT64;
	typedef uint64_t	RFID_HANDLE64;
	typedef uint64_t    UINT64;
	typedef uint64_t	SOCKET_HANDLE;
		#ifndef FALSE
		#define FALSE               0
	#endif
	#ifndef TRUE
		#define TRUE                1
	#endif


	//Time related
	typedef struct _SYSTEMTIME {
		WORD wYear;
		WORD wMonth;
		WORD wDayOfWeek;
		WORD wDay;
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
		WORD wMilliseconds;
	} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;


#endif
typedef void *				RFID_HANDLE32;
typedef void *				EVENT_HANDLE;
typedef void *				STRUCT_HANDLE;

#ifdef __cplusplus
}
#endif

#endif //RFIDAPI_TYPES_H
