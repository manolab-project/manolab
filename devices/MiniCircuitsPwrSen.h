#ifndef MINI_CIRCUITS_PWR_SEN_H
#define MINI_CIRCUITS_PWR_SEN_H


#include <vector>
#include "DeviceBase.h"
#include "SerialPort.h"
#include "hidapi.h"

class MiniCircuitsPwrSen : public DeviceBase
{
public:

    MiniCircuitsPwrSen() noexcept;
    ~MiniCircuitsPwrSen();
    bool Execute(const std::vector<Value> &args, Value &ret);

    void Initialize();
    void Stop();
    void AutoTest();

private:
     hid_device *mHandle;
     bool mInitialized;
     uint8_t mBuffer[64];
};


#endif // MINI_CIRCUITS_PWR_SEN_H
