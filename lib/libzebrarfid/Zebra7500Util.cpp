#include "Zebra7500Util.h"

wchar_t boolString[2][7] = {L"False", L"True"};


#define MAX_EVENTS 12

UINT32 goodIndex[2];
void rfidEventCallback(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType);
pthread_mutex_t RfidEventLock = PTHREAD_MUTEX_INITIALIZER;

BOOLEAN stopTesting = FALSE;
#ifndef linux
#define MAX_SEM_COUNT 1
#define THREADCOUNT 12
HANDLE readerEventAwaitingThreadHandle;
HANDLE InventoryStartSemaphore, InventoryCompleteSemaphore, AccessCompleteSemaphore, RfidEventSemaphore;
#else
pthread_t * readerEventAwaitingThreadHandle;
sem_t InventoryStartSemaphore, InventoryCompleteSemaphore, AccessCompleteSemaphore, RfidEventSemaphore;
#endif
std::list<RFID_EVENT_TYPE> rfidEventList;


#define MAX_PREFILTERS 2
static int nPreFilters;

typedef  struct _SINGLE_TAG_ACCESS_INFO
{
	UINT8  epcID[64];
	UINT32 epcIDLength;
}SINGLE_TAG_ACCESS_INFO;

static SINGLE_TAG_ACCESS_INFO singleTagAccessInfo;
static ACCESS_FILTER accessFilter;
static READER_CAPS readerCaps;
ANTENNA_INFO g_antennaInfo;

extern bool g_bUseWin32EventHandling;

// The code will use one or the other of these following : Event Handles for win32 events or Event types for the callbacks
#ifndef linux
HANDLE g_RfidWin32EventHandles[MAX_EVENTS];
#endif
RFID_EVENT_TYPE g_RfidEventTypes[MAX_EVENTS] = 
{
	GPI_EVENT, TAG_READ_EVENT, BUFFER_FULL_EVENT, BUFFER_FULL_WARNING_EVENT, 
	ANTENNA_EVENT, DISCONNECTION_EVENT, 
	INVENTORY_START_EVENT, INVENTORY_STOP_EVENT, ACCESS_START_EVENT, ACCESS_STOP_EVENT, NXP_EAS_ALARM_EVENT, READER_EXCEPTION_EVENT
};



void HandleResult(RFID_HANDLE32 readerHandle, RFID_STATUS rfidStatus)
{
	if(rfidStatus != RFID_API_SUCCESS)
	{
		ERROR_INFO errorInfo;
		RFID_GetLastErrorInfo(readerHandle, &errorInfo);
		wprintf(L"Error: %ls\n\t%ls\n\t%ls", RFID_GetErrorDescription(rfidStatus), errorInfo.statusDesc, errorInfo.vendorMessage);
	}

}
RFID_STATUS  ConnectReader(RFID_HANDLE32 *readerHandle, wchar_t *hostName, int readerPort)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	CONNECTION_INFO connectionInfo;
	connectionInfo.version = RFID_API3_5_1;
	rfidStatus = RFID_Connect(readerHandle, hostName, readerPort, 0,&connectionInfo);
	if(RFID_API_SUCCESS == rfidStatus)
	{
		RFID_SetTraceLevel(*readerHandle, TRACE_LEVEL_OFF);
		rfidStatus = RFID_GetReaderCaps(*readerHandle, &readerCaps); 
		g_antennaInfo.length = readerCaps.numAntennas;
		g_antennaInfo.pAntennaList = new	UINT16[g_antennaInfo.length];
		for(UINT16 nIndex = 0; nIndex < g_antennaInfo.length; nIndex++)
			g_antennaInfo.pAntennaList[nIndex] = nIndex+1;
		g_antennaInfo.pAntennaOpList = NULL;
	}
	HandleResult(*readerHandle, rfidStatus);
	return rfidStatus;
}


RFID_STATUS ReaderCapability(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	wprintf(L"\nFirmwareVersion = %ls", readerCaps.firmWareVersion);
	wprintf(L"\nmodelName = %ls", readerCaps.modelName);
	wprintf(L"\nnumAntennaSupported = %d", readerCaps.numAntennas);
	wprintf(L"\nnumGPI = %d", readerCaps.numGPIs );
	wprintf(L"\nnumGPO = %d", readerCaps.numGPOs);
	wprintf(L"\nhasUTCClockCapability = %ls", boolString[readerCaps.utcClockSupported]);
	wprintf(L"\nblockEraseSupported = %ls", boolString[readerCaps.blockEraseSupported]);
	wprintf(L"\nblockWriteSupported = %ls", boolString[readerCaps.blockWriteSupported]);
	wprintf(L"\ncanDoTagInventoryStateAwareSingulation = %d", readerCaps.stateAwareSingulationSupported);
	wprintf(L"\nmaxNumOperationsInAccessSequence = %d", readerCaps.maxNumOperationsInAccessSequence);
	wprintf(L"\nmaxNumPreFilters = %d", readerCaps.maxNumPreFilters);
	wprintf(L"\ncommunicationStandard = %d", readerCaps.communicationStandard);
	wprintf(L"\ncountryCode = %d", readerCaps.countryCode);
	wprintf(L"\nhoppingEnabled = %ls", boolString[readerCaps.hoppingEnabled]);

	return rfidStatus;
}

RFID_STATUS SingulationControl(RFID_HANDLE32 readerHandle, LPSINGULATION_CONTROL singulationControl)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	int antennaID = 0;

	wprintf(L"\nEnter AntennaId   ");
	scanf("%d", &antennaID);
	rfidStatus = RFID_GetSingulationControl(readerHandle, antennaID, singulationControl);
	if(RFID_API_SUCCESS == rfidStatus)
	{
		wprintf(L"\nSession = %d", singulationControl->session);
		wprintf(L"\nTagPopulation = %d", singulationControl->tagPopulation);
		wprintf(L"\nTagTransitTime = %d milliseconds", singulationControl->tagTransitTimeMilliseconds);
		wprintf(L"\ninventoryState = %d", singulationControl->stateAwareSingulationAction.inventoryState);
		wprintf(L"\nSLFlag = %d", singulationControl->stateAwareSingulationAction.slFlag);
		wprintf(L"\nPerformStateAwareSingulationAction = %ls", boolString[singulationControl->stateAwareSingulationAction.perform]);
	}
	HandleResult(readerHandle, rfidStatus);

	return rfidStatus;
}
RFID_STATUS	RebootReader(RFID_HANDLE32 readerManagementHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	rfidStatus = RFID_Restart(readerManagementHandle);
	if(rfidStatus == RFID_API_SUCCESS)
		wprintf(L"\n\nRebooting the reader...");
	HandleResult(readerManagementHandle, rfidStatus);
	return rfidStatus;
}


RFID_STATUS ConfigureGPO(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	UINT32 portNumber = 0; 
	int option = 0;

	wprintf(L"\n----Command Menu----");
	wprintf(L"\n1. SetGPOState");
	wprintf(L"\n2. GetGPOState\n");
	
    while(1 != scanf("%d", &option))
	{
		wprintf(L"\n Enter a Valid Input");
		clean_stdin();
	}
	wprintf(L"\nEnter portNumber   ");
	scanf("%d", &portNumber);

	switch(option)
	{
	case 1:
		{
			int gpoState = 0; 
			wprintf(L"\nEnter GPOState 0 to disable,1 to enable\n");
			scanf("%d", &gpoState);
			rfidStatus = RFID_SetGPOState(readerHandle, portNumber, (BOOLEAN)gpoState);
			if(rfidStatus == RFID_API_SUCCESS)
				wprintf(L"\n GPO State successfully set..\n");
			break;
		}
	case 2:
		{		
			BOOLEAN gpoState = FALSE; 
			rfidStatus = RFID_GetGPOState(readerHandle, portNumber, &gpoState);
			if(rfidStatus == RFID_API_SUCCESS)
				wprintf(L"\nGPOEnable = %ls", boolString[gpoState]);
			break;
		}
	}
	HandleResult(readerHandle, rfidStatus);
	return rfidStatus;
}

RFID_STATUS	ConfigureGPI(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	BOOLEAN gpiEnable;
	GPI_PORT_STATE gpiState;

	UINT32 portNumber; 
	wchar_t portState[3][10] = {L"LOW", L"HIGH", L"UNKNOWN"};
	gpiEnable = FALSE;
	gpiState = GPI_PORT_STATE_UNKNOWN;

	wprintf(L"\nEnter portNumber   ");
	scanf("%d", &portNumber);	
	rfidStatus = RFID_GetGPIState(readerHandle, portNumber, &gpiEnable, &gpiState);
	if(rfidStatus == RFID_API_SUCCESS)
	{
		wprintf(L"\nGPIEnable = %ls", boolString[gpiEnable]);
		wprintf(L"\nState = %ls", portState[gpiState]);
	}
	HandleResult(readerHandle, rfidStatus);
	return rfidStatus;
}

void rfidEventCallback(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	pthread_mutex_lock(&RfidEventLock);
	rfidEventList.push_back(eventType);
	pthread_mutex_unlock(&RfidEventLock);
#ifndef linux
	ReleaseSemaphore(RfidEventSemaphore, 1, NULL);
#else
	sem_post(&RfidEventSemaphore);
#endif
}

void HandleRfidEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{	
	wchar_t displayBuffer[260]={0,};
	TAG_DATA* pTagData = NULL;

	switch(eventType)
	{
	case GPI_EVENT:
		{
			GPI_EVENT_DATA gpiEventData;
			wchar_t portState[3][10] = {L"LOW", L"HIGH", L"UNKNOWN"};
			if(RFID_API_SUCCESS == RFID_GetEventData(readerHandle, eventType, &gpiEventData))
			{
				wprintf(L"\nSIGNALLED: GPI %d : %ls", gpiEventData.port, portState[gpiEventData.eventInfo]);			
			}
			else
			{
				wprintf(L"\nSIGNALLED: GPI : No event Data");	
			}
		}
		break;
	case TAG_READ_EVENT:
		{
			pTagData = RFID_AllocateTag(readerHandle);
			if(NULL == pTagData)
			{
				// Handle memory allocation failure
				// Optimally, Tag Allocation can be done once and pointer reused till disconnection.
				wprintf(L"RFID_AllocateTag Failed.");
				return;
			}
			while(RFID_API_SUCCESS == RFID_GetReadTag(readerHandle, pTagData))
			{
				printTagDataWithResults(pTagData);
			}
		}
		break;
	case BUFFER_FULL_EVENT:
		{
			pTagData = RFID_AllocateTag(readerHandle);
			if(NULL == pTagData)
			{
				// Handle memory allocation failure
				// Optimally, Tag Allocation can be done once and pointer reused till disconnection.
				wprintf(L"RFID_AllocateTag Failed.");
				return;
			}
			wprintf(L"\nSIGNALLED: BUFFER_FULL_EVENT");
			while(RFID_API_SUCCESS == RFID_GetReadTag(readerHandle, pTagData))
			{
				printTagDataWithResults(pTagData);
			}
		}
		break;
	case BUFFER_FULL_WARNING_EVENT:
		{
			pTagData = RFID_AllocateTag(readerHandle);
			if(NULL == pTagData)
			{
				// Handle memory allocation failure
				// Optimally, Tag Allocation can be done once and pointer reused till disconnection.
				wprintf(L"RFID_AllocateTag Failed.");
				return;
			}
			wprintf(L"\nSIGNALLED: BUFFER_FULL_WARNING_EVENT");
			while(RFID_API_SUCCESS == RFID_GetReadTag(readerHandle, pTagData))
			{
				printTagDataWithResults(pTagData);
			}
		}
		break;
	case ANTENNA_EVENT:
		wprintf(L"\nSIGNALLED: ANTENNA_EVENT");
		break;
	case DISCONNECTION_EVENT:
		DISCONNECTION_EVENT_DATA disconnectionEventData;
		wprintf(L"\nSIGNALLED: DISCONNECTION_EVENT  ");
		RFID_GetEventData(readerHandle,DISCONNECTION_EVENT,(STRUCT_HANDLE)&disconnectionEventData);
		wcscpy(displayBuffer, disconnectionEventData.eventInfo == READER_INITIATED_DISCONNECTION ? 
			L"Reader Initiated Disconnection"  : L"Reader Connection Lost");
		wprintf(displayBuffer);
		wprintf(L"\nQuit the Application and Restart..");
		break;
	case INVENTORY_START_EVENT:
		wprintf(L"\nSIGNALLED: INVENTORY_START_EVENT");
#ifndef linux
		ReleaseSemaphore(InventoryStartSemaphore, 1, NULL);
#else
		sem_post(&InventoryStartSemaphore);
#endif
		break;
	case INVENTORY_STOP_EVENT:
		wprintf(L"\nSIGNALLED: INVENTORY_STOP_EVENT");
#ifndef linux
		ReleaseSemaphore(InventoryCompleteSemaphore, 1, NULL);
#else
		sem_post(&InventoryCompleteSemaphore);
#endif
		break;
	case ACCESS_START_EVENT:
		wprintf(L"\nSIGNALLED: ACCESS_START_EVENT");
		break;
	case ACCESS_STOP_EVENT:
		wprintf(L"\nSIGNALLED: ACCESS_STOP_EVENT");
#ifndef linux
		ReleaseSemaphore(AccessCompleteSemaphore, 1, NULL);
#else
		sem_post(&AccessCompleteSemaphore);
#endif
		break;
	default:
		wprintf(L"\nUnknown/Unhandled event %d signalled", eventType);
		break;
	}
	if(pTagData)
		RFID_DeallocateTag(readerHandle, pTagData);
}

void * ProcessRfidEventsThread(void * pvarg)
{
	RFID_HANDLE32 readerHandle = (RFID_HANDLE32)pvarg;
	RFID_EVENT_TYPE eventType;
	while(!stopTesting)
	{
#ifndef linux
		WaitForSingleObject(RfidEventSemaphore, INFINITE);
#else
		sem_wait(&RfidEventSemaphore);
#endif
		while(!stopTesting && !rfidEventList.empty())
		{
			pthread_mutex_lock(&RfidEventLock);
			eventType = rfidEventList.front();
			rfidEventList.pop_front();
			pthread_mutex_unlock(&RfidEventLock);
			HandleRfidEvent(readerHandle, eventType);
		}
	}
	return NULL;
}

#ifndef linux
// win32 based event handling works only on windows
void * AwaitRfidWin32EventsThread(void * pvarg)
{
	while(!stopTesting)
	{
		DWORD dwStatus = WaitForMultipleObjects(MAX_EVENTS, g_RfidWin32EventHandles, FALSE, INFINITE);
		if(dwStatus >= WAIT_OBJECT_0 && dwStatus <= WAIT_OBJECT_0 + MAX_EVENTS)
		{
			rfidEventList.push_back((RFID_EVENT_TYPE)dwStatus);
			ReleaseSemaphore(RfidEventSemaphore, 1, NULL);
		}
	}
	return NULL;
}
#endif

void CreateEventThread(RFID_HANDLE32 readerHandle)
{
#ifndef linux
	DWORD dwThreadID = 0;
#endif
	if(g_bUseWin32EventHandling)
	{
#ifndef linux
		g_RfidWin32EventHandles[GPI_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_RfidWin32EventHandles[TAG_READ_EVENT] = CreateEvent(NULL, TRUE, FALSE, NULL);
		g_RfidWin32EventHandles[BUFFER_FULL_WARNING_EVENT] = CreateEvent(NULL, TRUE, FALSE, NULL);
		g_RfidWin32EventHandles[ANTENNA_EVENT] = CreateEvent(NULL, TRUE, FALSE, NULL);
		g_RfidWin32EventHandles[INVENTORY_START_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_RfidWin32EventHandles[INVENTORY_STOP_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_RfidWin32EventHandles[ACCESS_START_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_RfidWin32EventHandles[ACCESS_STOP_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_RfidWin32EventHandles[DISCONNECTION_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_RfidWin32EventHandles[BUFFER_FULL_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_RfidWin32EventHandles[NXP_EAS_ALARM_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);		
		g_RfidWin32EventHandles[READER_EXCEPTION_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);

		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, GPI_EVENT, g_RfidWin32EventHandles[GPI_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, TAG_READ_EVENT, g_RfidWin32EventHandles[TAG_READ_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, BUFFER_FULL_WARNING_EVENT, g_RfidWin32EventHandles[BUFFER_FULL_WARNING_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, BUFFER_FULL_EVENT, g_RfidWin32EventHandles[BUFFER_FULL_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, ANTENNA_EVENT, g_RfidWin32EventHandles[ANTENNA_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, INVENTORY_START_EVENT, g_RfidWin32EventHandles[INVENTORY_START_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, INVENTORY_STOP_EVENT, g_RfidWin32EventHandles[INVENTORY_STOP_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, ACCESS_START_EVENT, g_RfidWin32EventHandles[ACCESS_START_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, ACCESS_STOP_EVENT, g_RfidWin32EventHandles[ACCESS_STOP_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, DISCONNECTION_EVENT, g_RfidWin32EventHandles[DISCONNECTION_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, READER_EXCEPTION_EVENT, g_RfidWin32EventHandles[READER_EXCEPTION_EVENT]));
		HandleResult(readerHandle, RFID_RegisterEventNotification(readerHandle, NXP_EAS_ALARM_EVENT, g_RfidWin32EventHandles[NXP_EAS_ALARM_EVENT]));


		// we need a thread that waits on all these events.
		readerEventAwaitingThreadHandle = CreateThread(NULL, 0, (unsigned long (WINAPI *)(void *))AwaitRfidWin32EventsThread,  (void *)readerHandle, 0, &dwThreadID);
#endif
	}
	else
	{
		HandleResult(readerHandle, RFID_RegisterEventNotificationCallback(readerHandle, g_RfidEventTypes,  MAX_EVENTS, (RfidEventCallbackFunction) rfidEventCallback, NULL, NULL));
	}
#ifndef linux
	RfidEventSemaphore = CreateSemaphore( NULL, 0, MAX_SEM_COUNT, NULL);    
	InventoryStartSemaphore = CreateSemaphore( NULL, 0, MAX_SEM_COUNT, NULL);    
	InventoryCompleteSemaphore = CreateSemaphore( NULL, 0, MAX_SEM_COUNT, NULL);    
	AccessCompleteSemaphore = CreateSemaphore( NULL, 0, MAX_SEM_COUNT, NULL);
	readerEventAwaitingThreadHandle = CreateThread(NULL, 0, (unsigned long (WINAPI *)(void *))ProcessRfidEventsThread,  (void *)readerHandle, 0, &dwThreadID);
#else
    /*
	readerEventAwaitingThreadHandle  = new pthread_t;
	sem_init(&RfidEventSemaphore, 0, 0);
	sem_init(&InventoryStartSemaphore, 0, 0);
	sem_init(&InventoryCompleteSemaphore, 0, 0);
	sem_init(&AccessCompleteSemaphore, 0, 0);
	pthread_create(readerEventAwaitingThreadHandle, NULL, ProcessRfidEventsThread, (void *)(readerHandle));
    */
#endif
}

void KillEventThread()
{
	wprintf(L"\nKilling Threads.. ");
	stopTesting = TRUE;
	// Release allocated resources
#ifndef linux
	ReleaseSemaphore(RfidEventSemaphore, 1, NULL);
	WaitForSingleObject(readerEventAwaitingThreadHandle, INFINITE);
	CloseHandle(readerEventAwaitingThreadHandle);
	CloseHandle(RfidEventSemaphore);
	CloseHandle(InventoryStartSemaphore);
	CloseHandle(AccessCompleteSemaphore);
	CloseHandle(AccessCompleteSemaphore);
#else
	sem_post(&RfidEventSemaphore);
	pthread_join(*readerEventAwaitingThreadHandle, NULL );
	delete readerEventAwaitingThreadHandle;
	sem_destroy(&RfidEventSemaphore);
	sem_destroy(&InventoryStartSemaphore);
	sem_destroy(&InventoryCompleteSemaphore);
	sem_destroy(&AccessCompleteSemaphore);
	
#endif
	wprintf(L"Killing Threads.. Done");
	readerEventAwaitingThreadHandle = NULL;
}

RFID_STATUS	ConfigureAntenna(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT16 antennaID;
	UINT16 receiveSensitivityIndex; 
	UINT16 transmitPowerIndex; 
	UINT16 transmitFrequencyIndex;

	int option = 0;

	wprintf(L"\nEnter AntennaID   ");
	scanf("%hu", &antennaID);

	wprintf(L"\n----Command Menu----");
	wprintf(L"\n1. SetConfigureAntenna");
	wprintf(L"\n2. GetConfigureAntenna");
	
	while(1 != scanf("%d", &option))
	{
		wprintf(L"\nEnter a valid input:");
		clean_stdin();
	}
	switch(option)
	{
	case 1:
		{
			UINT16 frequencyIndex = 0;
			READER_CAPS readerCaps;
			RFID_GetReaderCaps(readerHandle, &readerCaps);
			wprintf(L"\nEnter ReceiveSensitivityIndex  value range 0-%d   ", readerCaps.receiveSensitivtyTable.numValues-1);
			scanf("%hu", &receiveSensitivityIndex);

			if(readerCaps.hoppingEnabled)
			{
				frequencyIndex = readerCaps.freqHopInfo.numTables;
			}
			else
			{
				frequencyIndex = readerCaps.fixedFreqInfo.numFreq;
			}
			wprintf(L"\nEnter TransmitFrequencyIndex value 1-%d   ", frequencyIndex);
			scanf("%hu", &transmitFrequencyIndex);

			wprintf(L"\nEnter TransmitPowerIndex  value 0-%d   ", readerCaps.transmitPowerLevelTable.numValues-1);
			scanf("%hu", &transmitPowerIndex);

			rfidStatus = RFID_SetAntennaConfig(readerHandle, antennaID, 
				receiveSensitivityIndex, transmitPowerIndex, transmitFrequencyIndex);
			if(rfidStatus == RFID_API_SUCCESS)
				wprintf(L"\n Antenna Configuration successfully set...\n");
		}
		break;
	case 2:
		rfidStatus = RFID_GetAntennaConfig(readerHandle, antennaID, &receiveSensitivityIndex, 
			&transmitPowerIndex, &transmitFrequencyIndex);
		if(rfidStatus == RFID_API_SUCCESS)
		{
			wprintf(L"\nReceiveSensitivityIndex = %d", receiveSensitivityIndex);
			wprintf(L"\nTransmitPowerIndex = %d", transmitPowerIndex);
			wprintf(L"\nTransmitFrequencyIndex = %d", transmitFrequencyIndex);
		}
		break;
	}
	HandleResult(readerHandle, rfidStatus);
	return rfidStatus;
}


RFID_STATUS	ConfigureRFMode(RFID_HANDLE32 readerHandle)
{	
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	int option = 0;
	UINT16 antennaID; 
	UINT32 rfModeTableIndex;
	UINT32 tari;

	wprintf(L"\nEnter AntennaID   ");
	scanf("%hu", &antennaID);

	wprintf(L"\n 1.Get RFMode");
	wprintf(L"\n 2.Set RFMode\n");
	while(1 != scanf("%d", &option))
	{
		wprintf(L"\nEnter a valid input:");
		clean_stdin();
	}
	switch(option)
	{
	case 1:
		rfidStatus = RFID_GetRFMode(readerHandle, antennaID, &rfModeTableIndex, &tari);
		if(rfidStatus == RFID_API_SUCCESS)
		{
			wprintf(L"\nRfModeTableIndex = %d", rfModeTableIndex);
			wprintf(L"\nTari = %d", tari);
		}
		break;
	case 2:
		wprintf(L"\n Enter RFMode Table Index   ");
		scanf("%d",&rfModeTableIndex);
		wprintf(L"\n Enter Tari Value   ");
		scanf("%d",&tari);
		rfidStatus = RFID_SetRFMode(readerHandle,antennaID,rfModeTableIndex,tari);
		if(rfidStatus == RFID_API_SUCCESS)
		{  
			wprintf(L"\nRF Mode successfully set..");
		}
		break;
	default: break;
	} 
	HandleResult(readerHandle, rfidStatus);

	return rfidStatus;

}


RFID_STATUS ConfigureAntennaMode(RFID_HANDLE32 readerManagementHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	ANTENNA_MODE antennaMode;
	int option = 0;

	wprintf(L"\n1. SetConfigureAntennaMode");
	wprintf(L"\n2. GetConfigureAntennaMode\n");
	while(1 != scanf("%d", &option))
	{
		wprintf(L"\nEnter a valid input:");
		clean_stdin();
	}
	switch(option)
	{
	case 1:
		{
			wprintf(L"\nEnter ConfigureAntennaMode, 0 for MonoStatic and  1 for Bistatic ");
			scanf("%d", &antennaMode);
			rfidStatus = RFID_SetAntennaMode(readerManagementHandle, antennaMode);
			if(rfidStatus == RFID_API_SUCCESS)
				wprintf(L"\nAntenna Mode successfully set..");
		}
		break;
	case 2:
		{
			rfidStatus = RFID_GetAntennaMode(readerManagementHandle, &antennaMode);
			if(rfidStatus == RFID_API_SUCCESS)
				wprintf(L"\n  The Antenna Mode is %ls ",antennaMode?"Bistatic":"Monostatic");
		}
		break;
	}
	HandleResult(readerManagementHandle, rfidStatus);

	return rfidStatus;
}

RFID_STATUS ConfigureReadPointStatus(RFID_HANDLE32 readerManagementHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	UINT16 antennaID; 
	BOOLEAN status;
	wprintf(L"\nEnter AntennaID");
	scanf("%d", &antennaID);

	int option = 0;

	wprintf(L"\n1. SetConfigureAntennaMode");
	wprintf(L"\n2. GetConfigureAntennaMode\n");
	while(1 != scanf("%d", &option))
	{
		wprintf(L"\nEnter a valid input:");
		clean_stdin();
	}
	switch(option)
	{
	case 1:
		{
			wprintf(L"\nEnter Status 0 to disable and 1 to enable");
			scanf("%d", &status);
			rfidStatus = RFID_EnableReadPoint(readerManagementHandle, antennaID, status);
			if(rfidStatus == RFID_API_SUCCESS)
				wprintf(L"\n Read Point %ls",status>0?"Enabled":"Disabled");
			break;
		}
	case 2:
		rfidStatus = RFID_GetReadPointStatus(readerManagementHandle, antennaID, &status);
		if(rfidStatus == RFID_API_SUCCESS)
			wprintf(L"\nStatus = %ls", boolString[status]);
		break;
	}

	HandleResult(readerManagementHandle, rfidStatus);
	return rfidStatus;
}

RFID_STATUS SimpleInventory(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	rfidStatus = RFID_PerformInventory(readerHandle, NULL, NULL, NULL, NULL);
	if(RFID_API_SUCCESS != rfidStatus)
	{
		HandleResult(readerHandle, rfidStatus);
		return rfidStatus;
	}

	wprintf(L"Waiting for sometime so that some tags are reported from the Reader");
#ifdef linux
    sleep(1);
#else
	Sleep(1000);
#endif
    rfidStatus = RFID_StopInventory(readerHandle);

	HandleResult(readerHandle, rfidStatus);
	return rfidStatus;
}

RFID_STATUS PeriodicInventory(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	TRIGGER_INFO triggerInfo;
	TAG_DATA* pTagData = NULL;

	pTagData = RFID_AllocateTag(readerHandle);


	triggerInfo.tagReportTrigger = 1;
	triggerInfo.startTrigger.type = START_TRIGGER_TYPE_PERIODIC;
	triggerInfo.startTrigger.value.periodic.periodMilliseconds = 10000; /*perform inventory every 10 sec*/
	SYSTEMTIME startTime;
	GetLocalTime(&startTime);
	startTime.wSecond += 2;
	triggerInfo.startTrigger.value.periodic.startTime = &startTime;

	triggerInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	triggerInfo.stopTrigger.value.duration = 5000;/*perform inventory for 5 seconds*/

	triggerInfo.lpTagEventReportInfo = NULL;
	while(RFID_API_SUCCESS == RFID_GetReadTag(readerHandle, pTagData));

	rfidStatus = RFID_PerformInventory(readerHandle, NULL, &g_antennaInfo, &triggerInfo, NULL);
	if(RFID_API_SUCCESS == rfidStatus)
	{
		wprintf(L"\nPeriodic Inventory starting in 2 seconds");
		for(int nIndex = 0; nIndex < 2; nIndex++)
		{
#ifndef linux
			if(WAIT_OBJECT_0 == WaitForSingleObject(InventoryStartSemaphore, INFINITE))
#else
			sem_wait(&InventoryStartSemaphore);
#endif
			{
				wprintf(L"\nInventory successfully started");
			}
#ifndef linux
			if(WAIT_OBJECT_0 == WaitForSingleObject(InventoryCompleteSemaphore, INFINITE))
#else
			sem_wait(&InventoryCompleteSemaphore);
#endif
			{
				wprintf(L"\nInventory successfully stopped");
			}
		}
		RFID_StopInventory(readerHandle);
	}
	HandleResult(readerHandle, rfidStatus);
	if(pTagData)
		RFID_DeallocateTag(readerHandle, pTagData);
	return rfidStatus;
}


RFID_STATUS AddPreFilter(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	PRE_FILTER filter[MAX_PREFILTERS];
	UINT8 filterMask0[32], filterMask1[32];
	char str[5];
	char pBytes[64]; 
	char *pBuf; 
	UINT32 Index;
	UINT16 temp;

	filter[0].pTagPattern = filterMask0;
	filter[1].pTagPattern = filterMask1;
	memset(str, 0, sizeof(str));
	memset(filter[0].pTagPattern, 0, sizeof(filter[0].pTagPattern));
	memset(filter[1].pTagPattern, 0, sizeof(filter[1].pTagPattern));
	memset(pBytes, 0, 64);

	UINT16 AntennaID = 0;
	memset(goodIndex, 0, sizeof(UINT32));
	if(nPreFilters!=MAX_PREFILTERS)
		wprintf(L"\n \nU can add %d more pre filters..\n",MAX_PREFILTERS-nPreFilters);
	else
	{
		wprintf(L"\n SampleApp supports only %d PreFilters in all",MAX_PREFILTERS);
		return rfidStatus;
	}
	for(int i = 0;i<(MAX_PREFILTERS-nPreFilters);i++)
	{
		if (i == 1)
		{
			wprintf(L"\nDo u want to add another Y/N");
			scanf("%s", str);
			if(str[0] == 'N' || str[0] == 'n')
				break;
		}
		wprintf(L"\n----Command Menu----");
		wprintf(L"\nEnter AntennaID   ");
		scanf("%hu", &AntennaID);
		wprintf(L"\nEnter Memorybank   ");
		wprintf(L"\n 1 for EPC  ");
		wprintf(L"\n 2 for TID  ");
		wprintf(L"\n 3 for USER\n");
		scanf("%d", &filter[i].memoryBank);
		wprintf(L"\n Enter TagPattern   ");
		scanf("%s", pBytes);
		wprintf(L"\n Enter tagPattern length in Bits   ");
		scanf("%d", &filter[i].tagPatternBitCount);
		pBuf = pBytes;
		for(int j = 0;j<(filter[i].tagPatternBitCount+7)/8;j++)
		{
			if(sscanf(pBuf, "%2x", &temp) == 1)
			{
				filter[i].pTagPattern[j] = (unsigned char)temp;
				pBuf  = pBuf + 2; 
			};
		}
		wprintf(L"\n Enter OffSet in bits   ");
		scanf("%d", &filter[i].bitOffset);
		wprintf(L"\n Enter FilterAction ");
		wprintf(L"\n 0 for FILTER_ACTION_DEFAULT");
		wprintf(L"\n 1 for FILTER_ACTION_STATE_AWARE");
		wprintf(L"\n 2 for FILTER_ACTION_STATE_UNAWARE  \n");
		scanf("%d", &filter[i].filterAction);

		switch(filter[i].filterAction)
		{
		case 0 :
			filter[i].filterAction = FILTER_ACTION_DEFAULT;
			break;
		case  1:
			filter[i].filterAction = FILTER_ACTION_STATE_AWARE;
			wprintf(L"\n Enter Target");
			wprintf(L"\n0 for TARGET_SL ");    
			wprintf(L"\n1 for TARGET_INVENTORIED_STATE_S0");    
			wprintf(L"\n2 for TARGET_INVENTORIED_STATE_S1" );    
			wprintf(L"\n3 for TARGET_INVENTORIED_STATE_S2 ");    
			wprintf(L"\n4 for TARGET_INVENTORIED_STATE_S3 \n");
			scanf("%d", &filter[i].filterActionParams.stateAwareParams.target);
			wprintf(L"\n Enter StateAwareAction");
			wprintf(L"\n0 for STATE_AWARE_ACTION_INV_A_NOT_INV_B");
			wprintf(L"\n0 for STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL");
			wprintf(L"\n1 for STATE_AWARE_ACTION_INV_A");
			wprintf(L"\n1 for STATE_AWARE_ACTION_ASRT_SL");
			wprintf(L"\n2 for STATE_AWARE_ACTION_NOT_INV_B ");
			wprintf(L"\n2 for STATE_AWARE_ACTION_NOT_DSRT_SL");
			wprintf(L"\n3 for STATE_AWARE_ACTION_INV_A2BB2A_NOT_INV_A");
			wprintf(L"\n3 for STATE_AWARE_ACTION_NEG_SL_NOT_ASRT_SL");
			wprintf(L"\n4 for STATE_AWARE_ACTION_INV_B_NOT_INV_A");
			wprintf(L"\n4 for STATE_AWARE_ACTION_DSRT_SL_NOT_ASRT_SL");
			wprintf(L"\n5 for STATE_AWARE_ACTION_INV_B ");
			wprintf(L"\n5 for STATE_AWARE_ACTION_DSRT_SL");
			wprintf(L"\n6 for STATE_AWARE_ACTION_NOT_INV_A");
			wprintf(L"\n6 for STATE_AWARE_ACTION_NOT_ASRT_SL");
			wprintf(L"\n7 for STATE_AWARE_ACTION_NOT_INV_A2BB2A");
			wprintf(L"\n7 for STATE_AWARE_ACTION_NOT_NEG_SL   \n");
			scanf("%d", &filter[i].filterActionParams.stateAwareParams.stateAwareAction);
			break;
		case 2 :
			wprintf(L"\nEnter stateUnawareAction");
			wprintf(L"\n0 for STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT ") ;
			wprintf(L"\n1 for STATE_UNAWARE_ACTION_SELECT ") ;
			wprintf(L"\n2 for STATE_UNAWARE_ACTION_NOT_UNSELECT ") ;
			wprintf(L"\n3 for STATE_UNAWARE_ACTION_UNSELECT ") ;
			wprintf(L"\n4 for STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT ") ;
			wprintf(L"\n5 for STATE_UNAWARE_ACTION_NOT_SELECT \n") ;
			scanf("%d", &filter[i].filterActionParams.stateUnawareAction);
			break;
		default:
			break;
		}

		rfidStatus = RFID_AddPreFilter(readerHandle, AntennaID, &filter[i], &Index);
		if (rfidStatus!= RFID_API_SUCCESS)
		{   
			break;
		}
		HandleResult(readerHandle, rfidStatus);
		nPreFilters++;
		wprintf(L"\n Filter Successfully Added with Index = %d \n", Index) ;
		goodIndex[i] = Index;
	}
	return rfidStatus;
}
RFID_STATUS RemovePrefilter(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	UINT16 AntennaID = 0;
	UINT32 filterIndex;
	wprintf(L"\nEnter AntennaID   ");
	scanf("%d", &AntennaID);	
	wprintf(L"\nEnter filterIndex   ");
	scanf("%d", &filterIndex);

	rfidStatus = RFID_DeletePreFilter(readerHandle, AntennaID, filterIndex);
	if (rfidStatus!= RFID_API_SUCCESS)
	{
		wprintf(L"\n Error Deleting the prefilter\n", filterIndex) ;
		HandleResult(readerHandle, rfidStatus);
	}
	else
		nPreFilters--;
	return rfidStatus;
}

RFID_STATUS PerformSingleTagAccess(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	char pBytes[256] = {0,}; 
	char *pBuf; 
	UINT32 temp = 0;

	memset(singleTagAccessInfo.epcID, 0, 64);
	wprintf(L"\n Enter TagID   :");
	scanf("%s", pBytes);

	//wprintf(L"\n Enter TagIDLength   ");
	//scanf("%d", &singleTagAccessInfo.epcIDLength);

	singleTagAccessInfo.epcIDLength = strlen(pBytes)/2;

	wprintf(L"\n Length is %d: ", singleTagAccessInfo.epcIDLength);
	pBuf = pBytes;
	for(UINT32 nIndex = 0; nIndex < singleTagAccessInfo.epcIDLength; nIndex++)
	{
		if(sscanf(pBuf, "%2x", &temp) == 1)
		{
			singleTagAccessInfo.epcID[nIndex] = (unsigned char)temp;
			pBuf  = pBuf + 2; 
			wprintf(L"%x", temp);
		};
	};
	rfidStatus = SingleTagAccess(readerHandle);
	return rfidStatus;

}
RFID_STATUS MultipleTagAccess(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	char pBytes[64] = {0, }; 
	char *pBuf; 
	UINT16 temp = 0;
	TAG_PATTERN tagPatternA;
	UINT8 patternData[64], patternMask[64];

	accessFilter.lpTagPatternA = &tagPatternA;
	accessFilter.lpTagPatternA->pTagPattern = patternData;
	accessFilter.lpTagPatternA->pTagMask = patternMask;
	accessFilter.lpTagPatternB = NULL;
	accessFilter.matchPattern = A_AND_B;
	memset(pBytes, 0, 64);
	memset(accessFilter.lpTagPatternA->pTagPattern, 0, 64);
	memset(accessFilter.lpTagPatternA->pTagMask, 0, 64);

	wprintf(L"\n----Access Filter ----");
	wprintf(L"\nEnter Memorybank   ");
	wprintf(L"\n 0 for RESERVED ");
	wprintf(L"\n 1 for EPC  ");
	wprintf(L"\n 2 for TID  ");
	wprintf(L"\n 3 for USER\n");
	scanf("%d", &accessFilter.lpTagPatternA->memoryBank);
	wprintf(L"\nEnter offset in bits   ");
	scanf("%d", &accessFilter.lpTagPatternA->bitOffset);
	wprintf(L"\nEnter Tag Pattern   ");
	scanf("%s", pBytes);

	wprintf(L"\nEnter Tag Pattern length in bytes\t");
	scanf("%d", &accessFilter.lpTagPatternA->tagPatternBitCount);

	pBuf = pBytes;
	for(int j = 0;j<accessFilter.lpTagPatternA->tagPatternBitCount ;j++)
	{
		if(sscanf(pBuf, "%2x", &temp) == 1)
		{
			accessFilter.lpTagPatternA->pTagPattern[j] = (unsigned char)temp;
			pBuf  = pBuf + 2; 
		};
	};
	accessFilter.lpTagPatternA->tagPatternBitCount = accessFilter.lpTagPatternA->tagPatternBitCount * 8;

	memset(pBytes, 0, 64);
	wprintf(L"\nEnter TagMask\t");
	scanf("%s", pBytes);
	wprintf(L"\nEnter tagMaskLength in bytes\t");
	scanf("%d", &accessFilter.lpTagPatternA->tagMaskBitCount);
	pBuf = pBytes;
	for(int j = 0;j<accessFilter.lpTagPatternA->tagMaskBitCount ;j++)
	{
		if(sscanf(pBuf, "%2x", &temp) == 1)
		{
			accessFilter.lpTagPatternA->pTagMask[j] = (unsigned char)temp;
			pBuf  = pBuf + 2; 
		};
	};
	accessFilter.lpTagPatternA->tagMaskBitCount = accessFilter.lpTagPatternA->tagMaskBitCount * 8;

	rfidStatus = PerformMultipleTagAccess(readerHandle);

	return rfidStatus;

}

RFID_STATUS SingleTagAccess(RFID_HANDLE32  readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	UINT16 option;
	wprintf(L"\n----Command Menu----");
	wprintf(L"\n1.Read Tag");
	wprintf(L"\n2.Write Tag");
	wprintf(L"\n3.Lock Tag");
	wprintf(L"\n4.Kill Tag\n");
	while(1 != scanf("%d", &option))
	{
		wprintf(L"\nEnter a valid input:");
		clean_stdin();
	}
	switch(option)
	{
	case 1:
		ReadAccessSingleTag(readerHandle);
		break;
	case 2:
		WriteAccessSingleTag(readerHandle);
		break;
	case 3:
		LockAccessSingleTag(readerHandle);
		break;
	case 4:
		KillAccessSingleTag(readerHandle);
		break;
	}

	return rfidStatus;

}


RFID_STATUS PerformMultipleTagAccess(RFID_HANDLE32  readerHandle)
{

	RFID_STATUS rfidStatus = RFID_API_SUCCESS;
	UINT16 option;
	wprintf(L"\n----Command Menu----");
	wprintf(L"\n1.Read Tag");
	wprintf(L"\n2.Write Tag");
	wprintf(L"\n3.Lock Tag");
	wprintf(L"\n4.Kill Tag\n");
	while(1 != scanf("%d", &option))
	{
		wprintf(L"\nEnter a valid input:");
		clean_stdin();
	}
	switch(option)
	{
	case 1:
		ReadAccessMultipleTags(readerHandle);
		break;
	case 2:
		WriteAccessMultipleTags(readerHandle);
		break;
	case 3:
		LockAccessMultipleTags(readerHandle);
		break;
	case 4:
		KillAccessMultipleTags(readerHandle);
		break;
	}

	return rfidStatus;

}


RFID_STATUS ReadAccessSingleTag(RFID_HANDLE32  readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	UINT8* pTagID;
	UINT32 tagIDLength;
	LPTAG_DATA pTagData = NULL;
	READ_ACCESS_PARAMS readAccessParams;
	char AccessPassword[10];
	UINT32 password;

	memset(AccessPassword, 0, 10);
	pTagID = singleTagAccessInfo.epcID;
	tagIDLength = singleTagAccessInfo.epcIDLength;

	wprintf(L"\nEnter accessPassword\t");
	scanf("%s", AccessPassword);
	sscanf(AccessPassword, "%x", &password);
	wprintf(L"\nEnter Memorybank   ");
	wprintf(L"\n 0 for RESERVED ");
	wprintf(L"\n 1 for EPC  ");
	wprintf(L"\n 2 for TID  ");
	wprintf(L"\n 3 for USER\n");
	scanf("%d", &readAccessParams.memoryBank);
	wprintf(L"\nEnter offset in bytes   ");
	scanf("%d", &readAccessParams.byteOffset);	
	wprintf(L"\nEnter length in bytes   ");
	scanf("%d", &readAccessParams.byteCount);
	readAccessParams.accessPassword = password;

	pTagData = RFID_AllocateTag(readerHandle);
	if(NULL == pTagData)
	{
		return RFID_API_CANNOT_ALLOC_MEM;
	}
	rfidStatus = RFID_Read(readerHandle, pTagID, tagIDLength, &readAccessParams, NULL, NULL, pTagData, NULL);	
	accessSuccessCount = accessFailureCount = 0;
	RFID_GetLastAccessResult(readerHandle, &accessSuccessCount, &accessFailureCount);
	HandleResult(readerHandle, rfidStatus);

	if(RFID_API_SUCCESS == rfidStatus &&  1 == accessSuccessCount && 0 == accessFailureCount)
	{
		printTagDataWithResults(pTagData);
	}
	if(pTagData)
		RFID_DeallocateTag(readerHandle, pTagData);
	return rfidStatus;

}

RFID_STATUS WriteAccessSingleTag(RFID_HANDLE32  readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	UINT8* pTagID;
	UINT32 tagIDLength;
	LPACCESS_FILTER lpAccessFilter;
	WRITE_ACCESS_PARAMS writeAccessParams;
	UINT8 writeData[64];

	writeAccessParams.pWriteData = writeData ;
	char pBytes[256]; 
	char *pBuf; 
	UINT16 temp = 0;
	char AccessPassword[10];
	memset(AccessPassword, 0, 10);


	memset(writeAccessParams.pWriteData, 0,64);
	memset(AccessPassword, 0, 10);


	pTagID = singleTagAccessInfo.epcID;
	tagIDLength = singleTagAccessInfo.epcIDLength;
	lpAccessFilter = NULL;


	wprintf(L"\nEnter accessPassword");
	scanf("%s", AccessPassword);
	sscanf(AccessPassword, "%x", &writeAccessParams.accessPassword);
	wprintf(L"\nEnter Memorybank   ");
	wprintf(L"\n 0 for RESERVED ");
	wprintf(L"\n 1 for EPC  ");
	wprintf(L"\n 2 for TID  ");
	wprintf(L"\n 3 for USER\n");
	scanf("%d", &writeAccessParams.memoryBank);
	wprintf(L"\nEnter length in bytes   ");
	scanf("%hu", &writeAccessParams.writeDataLength);
	wprintf(L"\nEnter offset in bytes   ");
	scanf("%hu", &writeAccessParams.byteOffset);
	wprintf(L"\nEnter data to be written   ");
	scanf("%s", pBytes);


	pBuf = pBytes;
	for(int j = 0;j<writeAccessParams.writeDataLength ;j++)
	{  
		if(1 == sscanf(pBuf, "%2x", &temp))
		{
			writeAccessParams.pWriteData[j] = (unsigned char)temp;
			pBuf  = pBuf + 2; 
		};
	};


	rfidStatus = RFID_Write(readerHandle, pTagID, tagIDLength, &writeAccessParams, lpAccessFilter, NULL, NULL);	
	accessSuccessCount = accessFailureCount = 0;
	ERROR_INFO errorInfo;
	if(RFID_API_SUCCESS!= rfidStatus)
	{
		rfidStatus = RFID_GetLastErrorInfo(readerHandle, &errorInfo);
	}
	RFID_GetLastAccessResult(readerHandle, 
		&accessSuccessCount, 
		&accessFailureCount);
	if(RFID_API_SUCCESS == rfidStatus &&  1 == accessSuccessCount && 0 == accessFailureCount)
	{
		rfidStatus = RFID_API_SUCCESS;
		wprintf(L"\nWrite Operation done successfully..");
	}
	HandleResult(readerHandle, rfidStatus);
	return rfidStatus;
}


RFID_STATUS LockAccessSingleTag(RFID_HANDLE32  readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	UINT8* pTagID;
	UINT32 tagIDLength;
	LOCK_ACCESS_PARAMS lockAccessParams;

	char AccessPassword[10];
	memset(AccessPassword, 0, 10);

	wchar_t lockDataField[NUM_LOCK_DATA_FIELDS][32] = {	
		{L"Kill Password"}, 
		{L"Access Password"}, 
		{L"EPC Memory"}, 
		{L"TID Memory"}, 
		{L"User Memory"}
	};
	memset(AccessPassword, 0, 10);

	pTagID = singleTagAccessInfo.epcID;
	tagIDLength = singleTagAccessInfo.epcIDLength;

	wprintf(L"\nEnter accessPassword   ");
	scanf("%s", AccessPassword);
	sscanf(AccessPassword, "%x", &lockAccessParams.accessPassword);


	for (int i = 0;i<5;i++)
	{
		wprintf(L"\nEnter the lock priviledge for the memory bank %ls", lockDataField[i]);
		wprintf(L"\n0 for None");
		wprintf(L"\n1 for Read-write");
		wprintf(L"\n2 for Permanent Lock");
		wprintf(L"\n3 for Permanent unlock");
		wprintf(L"\n4 for Unlock");
		scanf("%d", &lockAccessParams.privilege[i]);

	}

	rfidStatus = RFID_Lock(readerHandle, pTagID, tagIDLength, &lockAccessParams,NULL,&g_antennaInfo, NULL);	
	accessSuccessCount = accessFailureCount = 0;
	RFID_GetLastAccessResult(readerHandle, 
		&accessSuccessCount, 
		&accessFailureCount);
	if(RFID_API_SUCCESS!= rfidStatus)
	{
		ERROR_INFO errorInfo;
		rfidStatus = RFID_GetLastErrorInfo(readerHandle, &errorInfo);
		wprintf(L"\nLock failed. Reason: %ls",errorInfo.vendorMessage);
	}

	if(RFID_API_SUCCESS == rfidStatus &&  1 == accessSuccessCount && 0 == accessFailureCount)
	{
		rfidStatus = RFID_API_SUCCESS;
		wprintf(L"Lock Operation done successfully");
	}
	HandleResult(readerHandle, rfidStatus);
	return rfidStatus;
}


RFID_STATUS KillAccessSingleTag(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	UINT8* pTagID;
	UINT32 tagIDLength;
	LPACCESS_FILTER lpAccessFilter;
	KILL_ACCESS_PARAMS killAccessParams;

	char KillPassword[10];
	memset(KillPassword, 0,10);

	pTagID = singleTagAccessInfo.epcID;
	tagIDLength = singleTagAccessInfo.epcIDLength;
	lpAccessFilter = NULL;

	wprintf(L"\nEnter KillPassword   ");
	scanf("%s", KillPassword);
	sscanf(KillPassword, "%x", &killAccessParams.killPassword);
	rfidStatus = RFID_Kill(readerHandle, pTagID, tagIDLength, &killAccessParams, NULL,&g_antennaInfo, NULL);	

	accessSuccessCount = accessFailureCount = 0;
	RFID_GetLastAccessResult(readerHandle, 
		&accessSuccessCount, 
		&accessFailureCount);
	if(RFID_API_SUCCESS == rfidStatus &&  1 == accessSuccessCount && 0 == accessFailureCount)
	{
		rfidStatus = RFID_API_SUCCESS;
		wprintf(L"Lock Operation done successfully");
	}
	HandleResult(readerHandle, rfidStatus);

	return rfidStatus;
}


RFID_STATUS ReadAccessMultipleTags(RFID_HANDLE32  readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	LPTAG_DATA pTagData = NULL;
	READ_ACCESS_PARAMS readAccessParams;
	char AccessPassword[10];
	memset(AccessPassword, 0, 10);

	wprintf(L"\nEnter accessPassword   ");
	scanf("%s", AccessPassword);
	sscanf(AccessPassword, "%x", &readAccessParams.accessPassword);

	wprintf(L"\nEnter Memorybank   ");
	wprintf(L"\n 0 for RESERVED ");
	wprintf(L"\n 1 for EPC  ");
	wprintf(L"\n 2 for TID  ");
	wprintf(L"\n 3 for USER\n");
	scanf("%d", &readAccessParams.memoryBank);

	wprintf(L"\nEnter offset in bytes   ");
	scanf("%d", &readAccessParams.byteOffset);
	wprintf(L"\nEnter length in bytes   ");
	scanf("%d", &readAccessParams.byteCount);

	pTagData = RFID_AllocateTag(readerHandle);
	if(NULL == pTagData)
	{
		return RFID_API_CANNOT_ALLOC_MEM;
	}

	rfidStatus = RFID_Read(readerHandle, NULL, 0, &readAccessParams, &accessFilter,&g_antennaInfo, pTagData, NULL);
	if(rfidStatus == RFID_API_SUCCESS)
	{
#ifndef linux
		if(WAIT_OBJECT_0 == WaitForSingleObject(AccessCompleteSemaphore, INFINITE))
#else
		sem_wait(&AccessCompleteSemaphore);
#endif
		while(RFID_API_SUCCESS == RFID_GetReadTag(readerHandle, pTagData))
		{
			printTagDataWithResults(pTagData);
		}
		accessSuccessCount = accessFailureCount = 0;
		RFID_GetLastAccessResult(readerHandle, &accessSuccessCount, &accessFailureCount);
		wprintf(L"\n Read succeeded on %d tags and failed on %d tags ",accessSuccessCount,accessFailureCount);
	}
	HandleResult(readerHandle, rfidStatus);
	if(pTagData)
		RFID_DeallocateTag(readerHandle, pTagData);

	return rfidStatus;

}

RFID_STATUS WriteAccessMultipleTags(RFID_HANDLE32  readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	WRITE_ACCESS_PARAMS writeAccessParams;
	UINT8 writeData[64];
	writeAccessParams.pWriteData = writeData;
	char pBytes[64]; 
	char *pBuf; 
	UINT16 temp = 0;

	char AccessPassword[10];
	memset(AccessPassword, 0, 10);

	memset(writeAccessParams.pWriteData, 0,64);


	wprintf(L"\nEnter accessPassword   ");

	scanf("%s", AccessPassword);
	sscanf(AccessPassword, "%x", &writeAccessParams.accessPassword);
	wprintf(L"\nEnter Memorybank   ");
	wprintf(L"\n 0 for RESERVED ");
	wprintf(L"\n 1 for EPC  ");
	wprintf(L"\n 2 for TID  ");
	wprintf(L"\n 3 for USER \n");
	scanf("%d", &writeAccessParams.memoryBank);
	wprintf(L"\nEnter length in bytes   ");
	scanf("%d", &writeAccessParams.writeDataLength);
	wprintf(L"\nEnter offset in bytes   ");
	scanf("%d", &writeAccessParams.byteOffset);
	wprintf(L"\nEnter data to be written   ");
	scanf("%s", pBytes);

	pBuf = pBytes;
	for(int j = 0;j<writeAccessParams.writeDataLength ;j++)
	{
	   	if(1 == sscanf(pBuf, "%2x", &temp))
		{
			writeAccessParams.pWriteData[j] = (unsigned char)temp;
			pBuf  = pBuf + 2; 
		};
	};

	rfidStatus = RFID_Write(readerHandle, NULL, 0, &writeAccessParams, &accessFilter,&g_antennaInfo, NULL);	

	if(rfidStatus == RFID_API_SUCCESS)
	{
#ifndef linux
		if(WAIT_OBJECT_0 == WaitForSingleObject(AccessCompleteSemaphore, INFINITE))
#else
		sem_wait(&AccessCompleteSemaphore);
#endif
		accessSuccessCount = accessFailureCount = 0;
		RFID_GetLastAccessResult(readerHandle, &accessSuccessCount, &accessFailureCount);
		wprintf(L"\n Write succeeded on %d tags and failed on %d tags ",accessSuccessCount,accessFailureCount);

	}
	HandleResult(readerHandle, rfidStatus);

	return rfidStatus;
}


RFID_STATUS LockAccessMultipleTags(RFID_HANDLE32  readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	UINT8* pTagID;
	UINT32 tagIDLength;
	LOCK_ACCESS_PARAMS lockAccessParams;
	char AccessPassword[10];

	memset(AccessPassword, 0, 10);


	char lockDataField[NUM_LOCK_DATA_FIELDS][32] = {	{"Kill Password"}, 
	{"Access Password"}, 
	{"EPC Memory"}, 
	{"TID Memory"}, 
	{"User Memory"}
	};

	pTagID = NULL;
	tagIDLength = 0;

	wprintf(L"\nEnter accessPassword   ");

	scanf("%s", AccessPassword);
	sscanf(AccessPassword, "%x", &lockAccessParams.accessPassword);
	for (int i = 0;i<5;i++)
	{
		wprintf(L"\nEnter the lock priviledge for the memory bank %ls", lockDataField[i]);
		wprintf(L"\n0 for None");
		wprintf(L"\n1 for Read-write");
		wprintf(L"\n2 for Permanent Lock");
		wprintf(L"\n3 for Permanent unlock");
		wprintf(L"\n4 for Unlock");
		scanf("%d", &lockAccessParams.privilege[i]);

	}
	rfidStatus = RFID_Lock(readerHandle, pTagID, tagIDLength, &lockAccessParams, &accessFilter, &g_antennaInfo, NULL);	
	accessSuccessCount = accessFailureCount = 0;
#ifndef linux
	if(WAIT_OBJECT_0 == WaitForSingleObject(AccessCompleteSemaphore, INFINITE))
#else
	sem_wait(&AccessCompleteSemaphore);
#endif
	RFID_GetLastAccessResult(readerHandle, &accessSuccessCount, &accessFailureCount);
	if(accessSuccessCount> 0  && rfidStatus == RFID_API_SUCCESS)
	{
		rfidStatus = RFID_API_SUCCESS;
		wprintf(L"Lock Operation done successfully on %d tags and failed on %d tags",accessSuccessCount,accessFailureCount);
	}
	HandleResult(readerHandle, rfidStatus);

	return rfidStatus;
}


RFID_STATUS KillAccessMultipleTags(RFID_HANDLE32 readerHandle)
{
	RFID_STATUS rfidStatus = RFID_API_SUCCESS;

	UINT32 accessSuccessCount = 0;
	UINT32 accessFailureCount = 0;		
	UINT8* pTagID;
	UINT32 tagIDLength;
	KILL_ACCESS_PARAMS killAccessParams;

	pTagID = NULL;
	tagIDLength = 0;

	wprintf(L"\nEnter KillPassword   ");
	scanf("%d", &killAccessParams.killPassword);

	rfidStatus = RFID_Kill(readerHandle, pTagID, tagIDLength, &killAccessParams, &accessFilter,&g_antennaInfo, NULL);	

	accessSuccessCount = 0;
	accessFailureCount = 0;
#ifndef linux
	if(WAIT_OBJECT_0 == WaitForSingleObject(AccessCompleteSemaphore, INFINITE))
#else
	sem_wait(&AccessCompleteSemaphore);
#endif
	RFID_GetLastAccessResult(readerHandle, &accessSuccessCount, &accessFailureCount);
	if(accessSuccessCount>0 && rfidStatus == RFID_API_SUCCESS)
	{
		rfidStatus = RFID_API_SUCCESS;
		wprintf(L"Kill Operation done successfully on %d tags and failed on %d tags",accessSuccessCount,accessSuccessCount);
	}
	HandleResult(readerHandle, rfidStatus);

	return rfidStatus;
}

void printTagDataWithResults(TAG_DATA *pTagData)
{
	wchar_t resultBuffer[2048] = {0,};
	UINT8*pTagID = pTagData->pTagID;
	UINT8*pTagMBData = pTagData->pMemoryBankData;
	UINT32 epcLength =  pTagData->tagIDLength;
#ifdef WIN32
	TIME_ZONE_INFORMATION tzInfo;
#endif
	SYSTEMTIME localTime;

	wchar_t  memoryBankBuffer[128] = {0,};
	wchar_t  accessMBBuffer[128] = {0,};
	wchar_t  AccessResultBuffer[128] = {0,};
	wchar_t  tagEventBuffer[128] = {0,};
	wchar_t  tagLocationBuffer[128] = {0,};
	wchar_t  AccessOpBuffer[128] = {0,};
	wchar_t tagBuffer[260] = {0,};
	wchar_t* pTagReportData = tagBuffer;
	unsigned int   index = 0;
#ifndef linux
	GetTimeZoneInformation(&tzInfo);
#endif
	for(index = 0; index < epcLength; index++)
	{
		if(0 < index && index%2 == 0)
		{
			//*pTagReportData++ = L'-';
		}
		rfid_swprintf(pTagReportData, (260-index), L"%02X", pTagID[index]);
		while(*pTagReportData) pTagReportData++;
	}

	if(pTagData->lpLocation)
	{
		rfid_swprintf(tagLocationBuffer, 128, L"Relative-Distance = %d", pTagData->lpLocation->relativeDistance);
	}

	if(pTagData->tagEvent == NONE)
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->seenTime.utcTime.firstSeenTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->seenTime.utcTime.firstSeenTimeStamp, sizeof(SYSTEMTIME));
#endif
	}
	else
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->tagEventTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->tagEventTimeStamp, sizeof(SYSTEMTIME));
#endif
	}

	switch(pTagData->tagEvent)
	{
	case NONE:
		break;
	case UNKNOWN_STATE:
		break;
	case NEW_TAG_VISIBLE:
		rfid_swprintf(tagEventBuffer, 128, L"NEW\t");
		break;
	case TAG_BACK_TO_VISIBILITY:
		rfid_swprintf(tagEventBuffer, 128, L"BACK\t");
		break;
	case TAG_NOT_VISIBLE:
		rfid_swprintf(tagEventBuffer, 128, L"GONE\t");
		break;
	default:
		rfid_swprintf(tagEventBuffer, 128, L"ERROR!!!!!!!!!");
		break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE)
		switch(pTagData->opCode)
	{
		case  ACCESS_OPERATION_READ:
			rfid_swprintf(AccessOpBuffer, 128, L"Read");
			break;
		case ACCESS_OPERATION_WRITE:
			rfid_swprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_LOCK:
			rfid_swprintf(AccessOpBuffer, 128, L"Lock");
			break;
		case ACCESS_OPERATION_KILL:
			rfid_swprintf(AccessOpBuffer, 128, L"Kill");
			break;
		case ACCESS_OPERATION_BLOCK_WRITE:
			rfid_swprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_BLOCK_ERASE:
			rfid_swprintf(AccessOpBuffer, 128, L"Erase");
			break;
		case ACCESS_OPERATION_RECOMMISSION:
			rfid_swprintf(AccessOpBuffer, 128, L"Recommision");
			break;
		case ACCESS_OPERATION_BLOCK_PERMALOCK:
			rfid_swprintf(AccessOpBuffer, 128, L"Blk-Permalock");
			break;
		case ACCESS_OPERATION_NONE:
			rfid_swprintf(AccessOpBuffer, 128, L"None");
			break;
		default:
			rfid_swprintf(AccessOpBuffer, 128, L"ERROR!!!!!!!!!");
			break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE)
		switch(pTagData->opStatus)
	{
		case ACCESS_SUCCESS:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Success", AccessOpBuffer);
			break;
		case ACCESS_TAG_NON_SPECIFIC_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag non-specific error", AccessOpBuffer);
			break;
		case ACCESS_READER_NON_SPECIFIC_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Reader non-specific error", AccessOpBuffer);
			break;
		case ACCESS_NO_RESPONSE_FROM_TAG:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: No response from Tag", AccessOpBuffer);
			break;
		case ACCESS_INSUFFICENT_POWER:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Insufficient power", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_LOCKED_ERROR	:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag memory locked", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_OVERRUN_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag memory overrun", AccessOpBuffer);
			break;
		case ACCESS_ZERO_KILL_PASSWORD_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Zero kill password error", AccessOpBuffer);
			break;
		default:
			rfid_swprintf(AccessResultBuffer, 128, L"ERROR!!!!!!!!!");
			break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE && ACCESS_SUCCESS == pTagData->opStatus)
	{

		switch(pTagData->memoryBank)
		{
		case MEMORY_BANK_RESERVED:
			rfid_swprintf(memoryBankBuffer, 128, L"-RSD-");
			break;
		case MEMORY_BANK_EPC:
			rfid_swprintf(memoryBankBuffer, 128, L"-EPC-");
			break;
		case MEMORY_BANK_TID:
			rfid_swprintf(memoryBankBuffer, 128, L"-TID-");
			break;
		case MEMORY_BANK_USER:
			rfid_swprintf(memoryBankBuffer, 128, L"-USR-");
			break;
		}

		wchar_t* pTagReportData = accessMBBuffer;
		for(index = 0; index < pTagData->memoryBankDataLength; index++)
		{
			if(0 < index && index%2 == 0)
			{
				//*pTagReportData++ = L'-';
			}
			rfid_swprintf(pTagReportData, 128, L"%02X", pTagMBData[index]);
			while(*pTagReportData) pTagReportData++;
		}
	}	
	rfid_swprintf(resultBuffer, 2048, L"%2d/%2d/%2d %02d:%02d:%2d:%03d\t%ls\tAntenna:%2d\tPC:%5x\tXPC:%5x\tRSSI:%04d\t%ls %ls %ls %ls %ls\n",
		localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
		tagEventBuffer, pTagData->antennaID, pTagData->PC,
		pTagData->XPC, pTagData->peakRSSI, tagBuffer,AccessResultBuffer, memoryBankBuffer, accessMBBuffer, tagLocationBuffer);
	wprintf(L"%ls", resultBuffer);
}
#ifdef linux
tm SYSTEMTIME2tm(SYSTEMTIME *s)
{
	tm t;

	t.tm_year  = s->wYear - 1900;
	t.tm_mon   = s->wMonth- 1;
	t.tm_wday  = s->wDayOfWeek;
	t.tm_mday  = s->wDay;
	t.tm_yday  = 0;
	t.tm_hour  = s->wHour;
	t.tm_min   = s->wMinute;
	t.tm_sec   = s->wSecond;
	t.tm_isdst = 0;

	return t;
}


static SYSTEMTIME tm2SYSTEMTIME(tm *t,int milliSecs)
{
	SYSTEMTIME s;
	s.wYear      = t->tm_year + 1900;
	s.wMonth     = t->tm_mon  + 1;
	s.wDayOfWeek = t->tm_wday;
	s.wDay       = t->tm_mday;
	s.wHour      = t->tm_hour;
	s.wMinute    = t->tm_min;
	s.wSecond    = t->tm_sec;
	s.wMilliseconds = milliSecs;
	return s;
}

// returns as UTC time
void GetLocalTime(SYSTEMTIME * pSystemTime)
{
	time_t rawtime;
	struct tm * ptm;

	time ( &rawtime );

	ptm = gmtime ( &rawtime );
	*pSystemTime = tm2SYSTEMTIME(ptm,0);
}

#endif
void clean_stdin(void)
{
	int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
