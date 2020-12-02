#ifndef I_MODBUS_MASTER_H
#define I_MODBUS_MASTER_H

#include <cstdint>
#include <string>
#include "modbus.h"
#include "Value.h"

class IModbusMaster
{
public:
    virtual ~IModbusMaster() {}

    virtual bool ModbusRequest(std::uint32_t size, uint8_t slave_address, std::int32_t timeout_sec) = 0;
    virtual int32_t BuildFunc1Packet(modbus_mode_t mode, std::uint8_t slave, std::uint16_t start_addr, std::uint16_t size) = 0;
    virtual int32_t BuildFunc5Packet(modbus_mode_t mode, uint8_t slave, uint16_t addr, bool force) = 0;
    virtual int32_t BuildFunc3Packet(modbus_mode_t mode, std::uint8_t slave, std::uint16_t start_addr, std::uint16_t size) = 0;
    virtual int32_t BuildFunc16Packet(modbus_mode_t mode, uint8_t *wr_data, uint8_t slave, uint16_t start_addr, uint16_t size) = 0;
    virtual int32_t BuildFunc4Packet(modbus_mode_t mode, uint8_t slave, uint16_t start_addr, uint16_t size) = 0;
    virtual int32_t BuildFunc6Packet(modbus_mode_t mode, uint8_t slave, uint16_t addr, uint16_t value) = 0;
    virtual int32_t BuildFunc15Packet(modbus_mode_t mode, uint8_t *wr_data, uint8_t slave, uint16_t start_addr, uint16_t size) = 0;
    virtual std::uint32_t GetUint32Be(uint8_t index) = 0;
    virtual std::uint16_t GetUint16Be(uint8_t index) = 0;
    virtual std::uint8_t GetUint8(uint8_t index) = 0;
    virtual bool HasModbusError() const = 0;
    virtual std::string GetModbusError() const  = 0;
};

#endif // I_MODBUS_MASTER_H
