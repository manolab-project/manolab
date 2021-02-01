#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

#include "modbus.h"
#include "DeviceBase.h"
#include "SerialPort.h"
#include "IModbusMaster.h"

class ModbusMaster : public DeviceBase, public IModbusMaster
{
public:
    ModbusMaster();
    ~ModbusMaster();

    // From IModbusMaster
    bool ModbusRequest(uint32_t size, uint8_t slave_address, std::int32_t timeout_sec);
    int32_t BuildFunc1Packet(modbus_mode_t mode, uint8_t slave, uint16_t start_addr, uint16_t size);
    int32_t BuildFunc5Packet(modbus_mode_t mode, uint8_t slave, uint16_t addr, bool force);
    int32_t BuildFunc3Packet(modbus_mode_t mode, uint8_t slave, uint16_t start_addr, uint16_t size);
    int32_t BuildFunc16Packet(modbus_mode_t mode, uint8_t *wr_data, uint8_t slave, uint16_t start_addr, uint16_t size);
    int32_t BuildFunc4Packet(modbus_mode_t mode, uint8_t slave, uint16_t start_addr, uint16_t size);
    int32_t BuildFunc6Packet(modbus_mode_t mode, uint8_t slave, uint16_t addr, uint16_t value);
    int32_t BuildFunc15Packet(modbus_mode_t mode, uint8_t *wr_data, uint8_t slave, uint16_t start_addr, uint16_t size);

    std::uint32_t GetUint32Be(uint8_t index);
    std::uint16_t GetUint16Be(uint8_t index);
    uint8_t GetUint8(uint8_t index);
    bool HasModbusError() const;
    std::string GetModbusError() const;

    // From DeviceBase
    void Initialize();
    void Stop();
    void AutoTest();

    bool Execute(const std::vector<Value> &args, Value &ret);


private:
    SerialPort mPort;
    uint8_t mPacket[MAX_MODBUS_LENGTH];
    Value Function3Request(uint8_t slave_address, uint16_t start_addr, uint16_t nbWords);
    Value Function1Request(uint8_t slave_address, uint16_t start_addr, uint16_t nbCoils);
    Value Function5Request(uint8_t slave_address, uint16_t addr, bool force);
    Value Function16Request(uint8_t slave_address, uint16_t start_addr, uint8_t *data, uint16_t nb_words);
    Value Function6Request(uint8_t slave_address, uint16_t addr, uint16_t value);
    Value Function15Request(uint8_t slave_address, uint16_t start_addr, uint8_t *data, uint16_t nb_words);
    Value Function4Request(uint8_t slave_address, uint16_t start_addr, uint16_t nbWords);
    Value Function3_4Request(bool isFunc3, uint8_t slave_address, uint16_t start_addr, uint16_t nbWords);
};

#endif // MODBUSM_ASTER_H
