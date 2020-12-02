#ifndef CONTROLLINO_H
#define CONTROLLINO_H


#include <vector>
#include "DeviceBase.h"
#include "SerialPort.h"
#include "modbus.h"

class Controllino : public DeviceBase
{
public:
    enum RequestType
    {
        SET_DIGITAL     = 0x01,
        GET_DIGITAL     = 0x02,
        CLEAR_DIGITAL   = 0x04,
        GET_ANALOG      = 0x08,
        READ_FIRMWARE   = 0x10,
        WRITE_COMMAND   = 0x20,
        READ_COMMAND    = 0x40
    };

    enum AreaName
    {
        PORT_A,
        PORT_IN,
        PORT_D,
        PORT_R,
        FW_VERSION,
    };

    Controllino() noexcept;
    ~Controllino();
    bool Execute(const std::vector<Value> &args, Value &ret);

    void Initialize();
    void Stop();
    void AutoTest();

private:
    uint8_t mSlaveAddress;

    Value RequestIO(RequestType req, const std::string &io);
    bool IsAllowed(RequestType req, Controllino::AreaName port, std::uint32_t io_num, uint16_t &read_addr);

    Value WriteRequest(std::uint16_t start_addr, uint8_t *data, std::uint16_t size);
    Value ReadRequest(uint16_t start_addr, std::uint16_t size);
};


#endif // CONTROLLINO_H
