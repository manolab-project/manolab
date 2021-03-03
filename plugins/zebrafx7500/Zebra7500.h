#ifndef ZEBRA7500_H
#define ZEBRA7500_H

#include "PluginBase.h"
#include "rfidapi.h"
#include <thread>
#include "IProcessEngine.h"
#include "EventLoop.h"
#include "Util.h"

class Zebra7500 : public mano::PluginBase
{
public:
    Zebra7500();
    virtual ~Zebra7500();

    // From Manolab to plugin
    virtual std::string Request(const std::string &req);
    virtual bool Register(mano::IPlugin::ICallBack *cb);

private:
    RFID_HANDLE32 readerHandle;

    std::thread mThread;

    Device dev;

    mano::IPlugin::ICallBack *mCb = nullptr;
    bool mInitialized = false;

    bool Initialize();
    void Stop();
    bool Execute(const std::vector<Value> &args, Value &ret);
    void InventoryLoop();
    void SendToManolab(int64_t id);
};

#endif // ZEBRA7500_H
