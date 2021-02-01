#include "CanDevice.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>

#include "Util.h"
#include "Log.h"

#define MAX_CAN_DEVICE_NAME_LENGTH 100

static init_server_t libixxat_init_server = nullptr;
static stop_server_t libixxat_stop_server = nullptr;
static HMODULE mModule = nullptr;
static std::vector<std::string> gDevList;


CanDevice::CanDevice()
    : DeviceBase("CanDevice")
    , mInitialized(false)
{

}

CanDevice::~CanDevice()
{
    Stop();
}
#include "TcpClient.h"

bool CanDevice::Execute(const std::vector<Value> &args, Value &ret)
{
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
            else if (cmd == std::string("FLUSH"))
            {
                std::string data;
                bool okIxxat = Request(mClient, "FLUSH;" + mCurrentDevId, data);
                if (!okIxxat)
                {
                    SetError("IXXAT communication error: " + data);
                }
            }
            else if (cmd == std::string("READ"))
            {
                std::uint32_t timeout = 5; // seconds
                bool filterOnId = false;
                std::uint32_t filteredId = 0U;
                std::vector<std::uint32_t> filteredData;

                if (args.size() >= 2)
                {
                    timeout = static_cast<std::uint32_t>(args[1].GetInteger());

                    if (timeout == 0)
                    {
                        timeout = 5;
                    }

                    if (args.size() >= 3)
                    {
                        filterOnId = true;
                        filteredId = static_cast<std::uint32_t>(args[2].GetInteger());

                        // Also filter on data values
                        if (args.size() >= 4)
                        {
                            for (std::uint32_t i = 3U; i < args.size(); i++)
                            {
                                filteredData.push_back(static_cast<std::uint32_t>(args[i].GetInteger()));
                            }
                        }
                    }
                }

                std::vector<std::string> list;
                std::uint32_t id = 0U;
                std::vector<std::uint32_t> canData;
                bool success = false;

                do
                {
                    std::string data;
                    bool okIxxat = Request(mClient, "READ;" + mCurrentDevId, data);
                    if (!okIxxat)
                    {
                        timeout = 0;
                        success = false;
                        SetError("IXXAT communication error: " + data);
                    }
                    else if (data == std::string("NODATA"))
                    {
                        // Still wait
                        std::this_thread::sleep_for(std::chrono::seconds(1U));
                        timeout--;
                    }
                    else
                    {
                        list = Util::Split(data, ";");
                        if (list.size() > 0)
                        {
                            id = Util::FromString<uint32_t>(list[0]);

                            if (list.size() > 1)
                            {
                                canData.clear();
                                for (std::uint32_t i = 1; i < list.size(); i++)
                                {
                                    canData.push_back(Util::FromString<uint32_t>(list[i]));
                                }
                            }

                            if (filterOnId)
                            {
                                if (id == filteredId)
                                {
                                    if (filteredData.size() > 0)
                                    {
                                        bool dataEqual = true;
                                        // Also filter on data
                                        for (std::uint32_t i = 0U; i < filteredData.size(); i++)
                                        {
                                            if (i < canData.size())
                                            {
                                                if (filteredData[i] != canData[i])
                                                {
                                                    dataEqual = false;
                                                }
                                            }
                                            else
                                            {
                                                std::cout << "FILTER FAILED: NOT ENOUGH DATA" << std::endl;
                                                dataEqual = false;
                                            }
                                        }

                                        if (dataEqual)
                                        {
                                            success = true;
                                            timeout = 0; // Got the message we want!
                                            std::cout << "READ CAN SUCCESS WITH ID AND DATA" << std::endl;
                                        }
                                    }
                                    else
                                    {
                                        success = true;
                                        timeout = 0; // Got the message we want!
                                        std::cout << "READ CAN SUCCESS WITH ID" << std::endl;
                                    }
                                }
                            }
                            else
                            {
                                timeout = 0;
                                success = true; // we got data
                                std::cout << "READ CAN SUCCESS" << std::endl;
                            }
                        }
                    }
                }
                while(timeout > 0);

                std::stringstream ss;
                if (success)
                {
                    ss << R"({"status": "success", "id":)" << id << R"(, "size": )" << canData.size() << R"(, "data": [)";

                    if (canData.size() > 0)
                    {
                       for (std::uint32_t i = 0; i < canData.size(); i++)
                       {
                           ss << canData[i];
                           if (i < (canData.size() - 1))
                           {
                               ss << ", ";
                           }
                       }
                    }

                    ss  << "]}";

                   std::cout << "CAN READ: " << ss.str() << std::endl;
                }
                else
                {
                    ss << R"({"status": "error"})";
                }
                ret = Value(ss.str());
                ret.SetJsonString(true);
            }
            else if (cmd == std::string("SEND"))
            {
                unsigned long id;
                int data_size = 0;

                if (args.size() >= 2)
                {
                    id = static_cast<unsigned long>(args[1].GetInteger());
                    data_size = static_cast<int>(args.size()) - 2;

                    std::string request = "SEND;" + GetDeviceId() + ";" + std::to_string(id);

                    if (data_size <= 8)
                    {

                        for (int i = 0; i < data_size; i++)
                        {
                            unsigned int index = static_cast<unsigned int>(2 + i);
                            request += ";" + std::to_string(args[index].GetInteger());
                        }

                        std::string response;

                        if (Request(mClient, request, response))
                        {
                            if (response  != std::string("OK"))
                            {
                                SetError("CAN message data not sent");
                            }
                        }
                        else
                        {
                            SetError("IXXAT communication error: " + response);
                        }
                    }
                    else
                    {
                        SetError("CAN message data cannot exceed 8 bytes.");
                    }
                }
                else
                {
                    SetError("Specify CAN message ID to send.");
                }
            }

        }
    }

    return !HasError();
}

std::vector<std::string> CanDevice::GetList()
{
    gDevList.clear();
    tcp::TcpClient client;

    client.Initialize();
    if (client.Connect("127.0.0.1", 64000))
    {
        std::cout << "Connected to server" << std::endl;
        std::string list;
        if (Request(client, "LIST", list))
        {
            gDevList = Util::Split(list, ";");
        }
        else
        {
            std::cout << "IXXAT communication failure" << list << std::endl;
        }
    }
    else
    {
        std::cout << "Cannot connect to DLL server" << std::endl;
    }
    return gDevList;
}

bool CanDevice::StartIxxatServer()
{
    bool success = false;
    tcp::TcpSocket::Initialize();

    std::string exePath = Util::ExecutablePath();;

    exePath += Util::DIR_SEPARATOR + std::string("libixxat.dll");
    std::wstring widePath = Util::ToWString(exePath);

    std::wcout << L"Loading DLL: " << widePath << std::endl;

    mModule = LoadLibrary(widePath.c_str());

    if (mModule == nullptr)
    {
        std::cout << "Error loading DLL: " << GetLastError() << std::endl;
    }
    else
    {
        libixxat_init_server = reinterpret_cast<init_server_t>(GetProcAddress(mModule, "init_server"));
        libixxat_stop_server = reinterpret_cast<stop_server_t>(GetProcAddress(mModule, "stop_server"));
        if ((libixxat_init_server != nullptr) && (libixxat_stop_server != nullptr))
        {
            libixxat_init_server();
            success = true;
        }
    }
    return success;
}

void CanDevice::StopIxxatServer()
{
    if (libixxat_stop_server != nullptr)
    {
        libixxat_stop_server();
    }
    if (mModule != nullptr)
    {
        FreeLibrary(mModule);
    }
    mModule = nullptr;
    libixxat_init_server = nullptr;
    libixxat_stop_server = nullptr;
}

void CanDevice::AutoTest()
{
    FindIxxatByName();
}


bool CanDevice::Request(tcp::TcpClient &client, const std::string &req, std::string &res)
{
    bool success = false;


    if (client.IsConnected())
    {
        client.Send(req);
        if (client.RecvWithTimeout(res, 10*1024, 1000))
        {
            success = true;
            std::cout << "RCV: " << res << std::endl;
        }
        else
        {
            success = false;
            res = "Timeout";
        }
    }
    return success;
}

void CanDevice::Initialize()
{
    mClient.Initialize();
    if (mClient.Connect("127.0.0.1", 64000))
    {
        std::cout << "Connected to server" << std::endl;
        mInitialized = true;
    }
    else
    {
        SetError("Cannot connect to DLL server");
    }
}

void CanDevice::FindIxxatByName()
{
    bool foundDevice = false;

    // Explicit nname, try to find it
    if (GetDeviceId().size() > 0)
    {
        for (const auto & dev : gDevList)
        {
            if (dev == GetDeviceId())
            {
                // Found device
                foundDevice = true;
                mCurrentDevId = GetDeviceId();
                break;
            }
        }
    }
    else
    {
        // No explicit Ixxat name specified, choose one
        if (gDevList.size() > 0)
        {
            mCurrentDevId = gDevList[0];
        }
    }

    if (!foundDevice)
    {
        SetError("Cannot find any hardware with id: " + GetDeviceId());
    }
    else
    {
        TLogInfo("Using IXXAT device: " + mCurrentDevId);
        std::string response;
        std::string bitrate = GetDeviceOptions();

        if (bitrate.size() == 0)
        {
            SetError("IXXAT: missing option bitrate eg: options: \"125K\" ");
        }
        else
        {
            // Init found device
            if (Request(mClient, "INIT;" + GetDeviceId() + ";" + bitrate, response))
            {
                if (response != std::string("OK"))
                {
                    SetError("Device not initialized: " + mCurrentDevId);
                }
            }
            else
            {
                SetError("IXXAT DLL failure: " + response);
            }
        }
    }
}


void CanDevice::Stop()
{
    if (mInitialized)
    {
        mClient.Close();
        mInitialized = false;
    }
}
