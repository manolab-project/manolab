#ifndef CAN_DEVICE_H
#define CAN_DEVICE_H

#include <vector>
#include <thread>
#include "DeviceBase.h"
#include "libixxat.h"
#include <windows.h>
#include "TcpClient.h"

class CanDevice : public DeviceBase
{
public:
    CanDevice();
    ~CanDevice();
    bool Execute(const std::vector<Value> &args, Value &ret);

    static std::vector<std::string> GetList();
    static bool StartIxxatServer();
    static void StopIxxatServer();

    // From DeviceBase
    void AutoTest();
    void Initialize();
    void Stop();

private:
    tcp::TcpClient mClient;
    std::string mCurrentDevId;

    bool mInitialized;

    void FindIxxatByName();
    static bool Request(tcp::TcpClient &client, const std::string &req, std::string &res);
    int Exec(const std::string &CmdLine, const std::string &CmdRunDir);
};


#endif // CAN_DEVICE_H
