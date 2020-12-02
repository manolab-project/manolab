#ifndef VOLTCRAFT_PPS_H
#define VOLTCRAFT_PPS_H

#include <vector>
#include "DeviceBase.h"
#include "SerialPort.h"

class VoltCraftPPS : public DeviceBase
{
public:
    VoltCraftPPS() noexcept;
    ~VoltCraftPPS();
    bool Execute(const std::vector<Value> &args, Value &ret);

    // From DeviceBase
    void AutoTest();
    void Initialize();
    void Stop();

private:
    SerialPort mPort;
    bool Request(const std::string &request, std::string &response);
};


#endif // VOLTCRAFT_PPS_H
