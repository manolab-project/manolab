#ifndef BK8500_H
#define BK8500_H

#include <vector>
#include "DeviceBase.h"
#include "SerialPort.h"

class BK8500 : public DeviceBase
{
public:
    enum ValueType
    {
        MAX_VOLTAGE,
        MAX_CURRENT,
        MAX_POWER,
        CONSTANT_VOLTAGE,
        CONSTANT_CURRENT,
        CONSTANT_POWER
    };

    enum Mode
    {
        MODE_CC,
        MODE_CV,
        MODE_CW
    };


    BK8500() noexcept;
    ~BK8500();
    bool Execute(const std::vector<Value> &args, Value &ret);

    // From DeviceBase
    void AutoTest();
    void Initialize();
    void Stop();

private:
    SerialPort mPort;
    std::string BuildPacket(const std::string &cmd);
    void SetRemote();
    bool SendToDevice(const std::string &packet, std::string &dataRead);
    bool CheckResponse(const std::string &response);
    void GetIdentifiers();
    void SwitchOnOff(bool enable);
    void SetConstantMode(Mode mode);
    void SetValue(ValueType type, double value);
    void SetRemoteSensing(bool enable);
};


#endif // BK8500_H
