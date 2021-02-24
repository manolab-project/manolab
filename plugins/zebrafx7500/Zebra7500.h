#ifndef ZEBRA7500_H
#define ZEBRA7500_H

#include "Plugin.h"
#include "rfidapi.h"
#include <thread>

class Zebra7500 : public mano::PluginBase
{
public:
    Zebra7500();
    virtual ~Zebra7500();

    bool Initialize();
    void Stop();
    void AutoTest();

    bool Execute(const std::vector<Value> &args, Value &ret);

private:
    RFID_HANDLE32 readerHandle;

    std::thread mThread;
    bool mInitialized = false;

    bool Request(const std::string &request, std::string &response);
    void InventoryLoop();
};

#endif // ZEBRA7500_H
