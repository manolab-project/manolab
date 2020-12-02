#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H


#include <vector>
#include "DeviceBase.h"
#include "SerialPort.h"

class SerialDevice : public DeviceBase
{
public:

    SerialDevice() noexcept;
    ~SerialDevice();
    bool Execute(const std::vector<Value> &args, Value &ret);

    void Initialize();
    void Stop();
    void AutoTest();

private:
    SerialPort mPort;
};


#endif // SERIALDEVICE_H
