#include "Zebra7500.h"

#include <iostream>
#include <string>

#include "serial.h"
#include "Util.h"
#include "Log.h"

static ThreadQueue<int> mLoopQueue;

Zebra7500::Zebra7500() noexcept
    : DeviceBase("Zebra7500")
{

}

Zebra7500::~Zebra7500()
{

}

bool Zebra7500::Execute(const std::vector<Value> &args, Value &ret)
{
    (void) ret;
    if (args.size() > 0)
    {
        // First argument is the command
        if (args[0].GetType() == Value::STRING)
        {
            std::string cmd = args[0].GetString();

            if (cmd == std::string("INIT"))
            {
                Initialize();
            }
            else if (cmd == std::string("TEST"))
            {
                AutoTest();
            }
            else if (cmd == std::string("START"))
            {
                 mLoopQueue.Push(60);
            }
            else if ((cmd == std::string("VOLT")) || cmd == std::string("SOVP"))
            {

            }
        }
    }

    return !HasError();
}

void Zebra7500::AutoTest()
{
    std::string response;
    //Request("GETS\r", response);
}


bool Zebra7500::Request(const std::string &request, std::string &response)
{
    bool success = false;

    return success;
}

static wchar_t hostName[260];
bool g_bUseWin32EventHandling = false; // in lunux, just the callback mechanism is supported.



#define MAX_EVENTS 12
static RFID_EVENT_TYPE gRfidEventTypes[MAX_EVENTS] =
{
    GPI_EVENT, TAG_READ_EVENT, BUFFER_FULL_EVENT, BUFFER_FULL_WARNING_EVENT,
    ANTENNA_EVENT, DISCONNECTION_EVENT,
    INVENTORY_START_EVENT, INVENTORY_STOP_EVENT, ACCESS_START_EVENT, ACCESS_STOP_EVENT, NXP_EAS_ALARM_EVENT, READER_EXCEPTION_EVENT
};

static void ZebraRfidEventCallback(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
    mLoopQueue.Push(eventType);
}


void Zebra7500::Initialize()
{
    if (!mInitialized)
    {
        std::string channel = GetConnectionChannel();
        wcscpy(hostName, Util::ToWString(channel).c_str());

        RFID_STATUS rfidStatus = ConnectReader(&readerHandle, hostName, 5084);
        if(RFID_API_SUCCESS == rfidStatus)
        {
            TAG_STORAGE_SETTINGS tagStorageSettings;

            RFID_GetTagStorageSettings(readerHandle,&tagStorageSettings);
            tagStorageSettings.discardTagsOnInventoryStop = TRUE;
            RFID_SetTagStorageSettings(readerHandle,&tagStorageSettings);

//            CreateEventThread(readerHandle);
            HandleResult(readerHandle, RFID_RegisterEventNotificationCallback(readerHandle, gRfidEventTypes,  MAX_EVENTS, (RfidEventCallbackFunction) ZebraRfidEventCallback, NULL, NULL));

            mThread = std::thread(&Zebra7500::InventoryLoop, this);
            mInitialized = true;
        }
    }
}



void Zebra7500::InventoryLoop()
{
    int order = 0;
    bool loop = true;
//    bool start = false;
    do
    {
        if (mLoopQueue.WaitAndPop(order, 20))
        {
            //
            if (order == 42)
            {
                loop = false;
            }
            else if (order == 60)
            {
//                start = true;

                RFID_STATUS rfidStatus = RFID_API_SUCCESS;

                rfidStatus = RFID_PerformInventory(readerHandle, NULL, NULL, NULL, NULL);
                if(RFID_API_SUCCESS != rfidStatus)
                {
                    HandleResult(readerHandle, rfidStatus);
                    // FIXME: handle error
                }

            }
            else if (order == TAG_READ_EVENT)
            {
                TAG_DATA* pTagData = NULL;
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
                    //printTagDataWithResults(pTagData);
                }

                if(pTagData)
                {
                    RFID_DeallocateTag(readerHandle, pTagData);
                }
            }
        }
//        if (start)
//        {
//            SimpleInventory(readerHandle);
//        }
    }
    while(loop);
}


void Zebra7500::Stop()
{    
    if (mInitialized)
    {
        mLoopQueue.Push(42);
        if (mThread.joinable())
        {
            mThread.join();
        }

        mInitialized = false;
      //  KillEventThread();
        RFID_Disconnect(readerHandle);
    }
}
