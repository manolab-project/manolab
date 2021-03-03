
#ifdef WIN32
#include <windows.h>
#include <commctrl.h>
#endif
 
#include "Zebra7500.h"
#include "Zebra7500Util.h"
#include <iostream>
#include "Plugin.h"
#include "JsonReader.h"
#include "Util.h"
#include "Log.h"
#include "ThreadQueue.h"
#include "IProcessEngine.h"

MANOLAB_PLUGIN(Zebra7500, "ZebraFX7500 RFID UHF Reader", "0.1.1")


static ThreadQueue<int> mLoopQueue;

Zebra7500::Zebra7500()
    : PluginBase("Zebra7500")
{

}

Zebra7500::~Zebra7500()
{
    Stop();
}

std::string Zebra7500::Request(const std::string &req)
{
    JsonReader reader;
    JsonValue json;
    bool success = false;
    std::string cmd;
    std::string returnValue;
    std::string message;

    if (reader.ParseString(json, req))
    {
        cmd = json.FindValue("cmd").GetString();
        JsonValue data = json.FindValue("data");

        if (cmd == "SetParameters")
        {
            if (data.IsObject())
            {
                dev.name = data.FindValue("name").GetString();
                dev.type = data.FindValue("type").GetString();
                dev.conn_channel = data.FindValue("conn_channel").GetString();
                dev.conn_settings = data.FindValue("conn_settings").GetString();
                dev.id = data.FindValue("id").GetString();
                dev.options = data.FindValue("options").GetString();
                success = true;

                success = Initialize();
            }
        }
        else if (cmd == "Execute")
        {
            if (data.IsObject())
            {
                JsonArray array = data.FindValue("args").GetArray();
                std::vector<Value> args;
                for (const auto &a : array)
                {
                    args.push_back(PluginBase::JsonToValue(a));
                }

                Value ret;
                success = Execute(args, ret);

                if (success)
                {
                    JsonObject obj;
                    obj.AddValue("cmd", "ReplyExecute");
                    obj.AddValue("message", "");
                    obj.AddValue("success", true);
                    obj.AddValue("data", PluginBase::ValueToJson(ret));

                    returnValue = obj.ToString();
                }
            }
        }
    }

    if (!success)
    {
        returnValue = PluginBase::ErrorResponse(cmd, message);
    }
    else if (returnValue == "")
    {
        returnValue = PluginBase::GenericResponse(true, cmd);
    }
    return returnValue;
}

bool Zebra7500::Register(mano::IPlugin::ICallBack *cb)
{
    mCb = cb;
    return true;
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
                // FIXME create auto test
            }
            else if (cmd == std::string("START"))
            {
                 mLoopQueue.Push(60);
            }
        }
    }

    return true;
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
    (void) readerHandle;
    mLoopQueue.Push(eventType);
}

void HandleResult(RFID_HANDLE32 readerHandle, RFID_STATUS rfidStatus)
{
	if(rfidStatus != RFID_API_SUCCESS)
	{
		ERROR_INFO errorInfo;
		RFID_GetLastErrorInfo(readerHandle, &errorInfo);
		wprintf(L"Error: %ls\n\t%ls\n\t%ls", RFID_GetErrorDescription(rfidStatus), errorInfo.statusDesc, errorInfo.vendorMessage);
	}

}

static READER_CAPS readerCaps;
static ANTENNA_INFO g_antennaInfo;
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



bool Zebra7500::Initialize()
{
    if (!mInitialized)
    {
        std::string channel = dev.conn_channel;
        wcscpy(hostName, Util::ToWString(channel).c_str());

        RFID_STATUS rfidStatus = ConnectReader(&readerHandle, hostName, 5084);
        if(RFID_API_SUCCESS == rfidStatus)
        {
            TAG_STORAGE_SETTINGS tagStorageSettings;

            RFID_GetTagStorageSettings(readerHandle,&tagStorageSettings);
            tagStorageSettings.discardTagsOnInventoryStop = TRUE;
            RFID_SetTagStorageSettings(readerHandle,&tagStorageSettings);

            HandleResult(readerHandle, RFID_RegisterEventNotificationCallback(readerHandle, gRfidEventTypes,  MAX_EVENTS, (RfidEventCallbackFunction) ZebraRfidEventCallback, NULL, NULL));

            mThread = std::thread(&Zebra7500::InventoryLoop, this);
            mInitialized = true;
        }
    }

    return mInitialized;
}

void Zebra7500::SendToManolab(int64_t id)
{

    JsonObject json;
    json.AddValue("cmd", "SetTableEntry");
    json.AddValue("tag", id);
    json.AddValue("time", Util::CurrentTimeStamp64());

    mCb->Callback(json.ToString().c_str());
}

void Zebra7500::InventoryLoop()
{
    int order = 0;
    bool loop = true;
//    bool start = false;

    TAG_DATA* pTagData = NULL;
    pTagData = RFID_AllocateTag(readerHandle);
    if(NULL == pTagData)
    {
        // Handle memory allocation failure
        // Optimally, Tag Allocation can be done once and pointer reused till disconnection.
        wprintf(L"RFID_AllocateTag Failed.");
        return;
    }

    do
    {
        if (mLoopQueue.TryPop(order))
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
                // on ne fait rien, cela permet juste de débloquer la boucle
            }
        }

        if (RFID_API_SUCCESS == RFID_GetReadTag(readerHandle, pTagData))
        {
            // On récupère le Tag id, il doit être de 12 octets
            UINT32 epcLength =  pTagData->tagIDLength;
            char tagidBuffer[12]; // en ascii
            uint64_t tid = 0;
            if (epcLength == 12)
            {
                for (uint32_t i = 0; i < epcLength; i++)
                {
                    char b = pTagData->pTagID[i];
                    if ((b < 0x30) || (b > 0x39))
                    {
                        b = 0x20;
                    }

                    tagidBuffer[i] = b;
                }
                std::istringstream iss(tagidBuffer);
                iss >> tid;
            }

            SendToManolab(tid);
        }
    }
    while(loop);

    if(pTagData)
    {
        RFID_DeallocateTag(readerHandle, pTagData);
    }
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
