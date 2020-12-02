#ifndef ACDU_DC_H
#define ACDU_DC_H


#include <vector>
#include "DeviceBase.h"
#include "modbus.h"
#include "SerialPort.h"

class AcuDC : public DeviceBase
{
public:
    AcuDC() noexcept;
    ~AcuDC();
    bool Execute(const std::vector<Value> &args, Value &ret);

    void Initialize();
    void Stop();
    void AutoTest();

private:
    uint8_t mSlaveAddress;
    Value Read(uint16_t start_addr, uint16_t nbWords);
};


#endif // ACDU_DC_H
