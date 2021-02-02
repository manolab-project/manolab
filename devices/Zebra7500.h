#ifndef ZEBRA7500_H
#define ZEBRA7500_H

#include <vector>
#include <thread>
#include "ThreadQueue.h"
#include "DeviceBase.h"

#include "Zebra7500Util.h"

class Zebra7500 : public DeviceBase
{
public:
    Zebra7500() noexcept;
    ~Zebra7500();
    bool Execute(const std::vector<Value> &args, Value &ret);

    // From DeviceBase
    void AutoTest();
    void Initialize();
    void Stop();

private:
    RFID_HANDLE32 readerHandle;

    std::thread mThread;
    bool mInitialized = false;

    bool Request(const std::string &request, std::string &response);
    void InventoryLoop();
};


#endif // ZEBRA7500_H
