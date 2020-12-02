#include "Controllino.h"

#include <iostream>
#include <string>

#include "serial.h"
#include "Util.h"
#include "Log.h"
#include "modbus.h"


Controllino::Controllino() noexcept
    : DeviceBase("Controllino")
{

}

Controllino::~Controllino()
{

}

bool ParseIO(const std::string &str, Controllino::AreaName &port, uint32_t &io)
{
    bool success = false;
    // IO examples: A3, IN0,
    if ((str.size() == 2) || (str.size() == 3))
    {
        uint32_t begin_at = 1U;
        if ((str[0] == 'I') && (str[1] == 'N'))
        {
            begin_at = 2;
            success = true;
            port = Controllino::PORT_IN;
        }
        else if (str[0] == 'A')
        {
            success = true;
            port = Controllino::PORT_A;
        }
        else if (str[0] == 'D')
        {
            success = true;
            port = Controllino::PORT_D;
        }
        else if (str[0] == 'R')
        {
            success = true;
            port = Controllino::PORT_R;
        }

        if (success)
        {
            std::string io_num = str.substr(begin_at);
            io = Util::FromString<std::uint32_t>(io_num);
        }
    }
    return success;
}

bool Controllino::Execute(const std::vector<Value> &args, Value &ret)
{
    (void) ret;

    if (args.size() > 0)
    {
        // First argument is the command
        if (args[0].GetType() == Value::STRING)
        {
            std::string cmd = args[0].GetString();

            if ((cmd == std::string("INIT")) && (args.size() >= 2))
            {
                Initialize();
            }
            else if (cmd == std::string("SET_DIGITAL"))
            {
                if (args.size() >= 2)
                {
                    ret = RequestIO(SET_DIGITAL, args[1].GetString());
                }
                else
                {
                    SetError("IO write need extra argument with an IO name, eg: 3A, IN0...");
                }
            }
            else if (cmd == std::string("CLEAR_DIGITAL"))
            {
                if (args.size() >= 2)
                {
                    ret = RequestIO(CLEAR_DIGITAL, args[1].GetString());
                }
                else
                {
                    SetError("IO read need extra argument with an IO name, eg: 3A, IN0...");
                }
            }
            else if (cmd == std::string("GET_DIGITAL"))
            {
                if (args.size() >= 2)
                {
                    ret = RequestIO(GET_DIGITAL, args[1].GetString());
                }
                else
                {
                    SetError("IO read need extra argument with an IO name, eg: 3A, IN0...");
                }
            }
            else if (cmd == std::string("GET_ANALOG"))
            {
                if (args.size() >= 2)
                {
                    ret = RequestIO(GET_ANALOG, args[1].GetString());
                }
                else
                {
                    SetError("IO read need extra argument with an IO name, eg: 3A, IN0...");
                }
            }
            else if (cmd == std::string("READ_FIRMWARE"))
            {
                ret = ReadRequest(66, 2);
            }
            else if (cmd == std::string("WRITE_COMMAND"))
            {
                if (args.size() >= 2)
                {
                    std::uint16_t cmd = static_cast<uint16_t>(args[1].GetInteger());
                    uint8_t cmdBuff[2];
                    cmdBuff[0] = (cmd >> 8) & 0xFFU;
                    cmdBuff[1] = cmd & 0xFFU;

                    ret = WriteRequest(68, &cmdBuff[0], 1);
                }
                else
                {
                    SetError("Commands need extra integer argument with the order.");
                }
            }
            else if (cmd == std::string("READ_COMMAND"))
            {
                ret = ReadRequest(68, 1);
            }
        }
    }

    return !HasError();
}

void Controllino::AutoTest()
{
    (void) ReadRequest(66, 2);
}

struct RegisterSet {
    Controllino::AreaName port;
    uint16_t start_addr;
    uint16_t size;
    uint8_t access;
};


static const std::vector<RegisterSet> gMapping {
    { Controllino::PORT_D, 0U, 12, Controllino::SET_DIGITAL | Controllino::CLEAR_DIGITAL },
    { Controllino::PORT_D, 12U, 12, Controllino::GET_DIGITAL },
    { Controllino::PORT_R, 24U, 10, Controllino::SET_DIGITAL | Controllino::CLEAR_DIGITAL },
    { Controllino::PORT_R, 34U, 10, Controllino::GET_DIGITAL },
    { Controllino::PORT_A, 44U, 10, Controllino::GET_ANALOG },
    { Controllino::PORT_IN, 54U, 2, Controllino::GET_DIGITAL },
    { Controllino::PORT_A, 56U, 10, Controllino::GET_DIGITAL }
};

bool Controllino::IsAllowed(RequestType req, Controllino::AreaName port, std::uint32_t io_num, uint16_t &read_addr)
{
    bool success = false;
    for (auto const &reg : gMapping)
    {
        if ((reg.port == port) && ((reg.access & req) == req))
        {
            if (io_num < reg.size)
            {
                read_addr = static_cast<uint16_t>(reg.start_addr + io_num);
                success = true;
            }
        }
    }

    return success;
}

Value Controllino::RequestIO(RequestType req, const std::string &io)
{
    Controllino::AreaName port;
    std::uint32_t io_num = 0U;
    Value retValue;

    if (ParseIO(io, port, io_num))
    {
        uint16_t address = 0U;

        if (IsAllowed(req, port, io_num, address))
        {
            if ((req == SET_DIGITAL) || (req == CLEAR_DIGITAL))
            {
                uint16_t bool_val = 0U;
                if (req == SET_DIGITAL)
                {
                    bool_val = 1U;
                }
                uint8_t cmdBuff[2];
                cmdBuff[0] = (bool_val >> 8) & 0xFFU;
                cmdBuff[1] = bool_val & 0xFFU;

                retValue = WriteRequest(address, &cmdBuff[0], 1);
            }
            else
            {
                retValue = ReadRequest(address, 1);
            }
        }
        else
        {
            SetError("Forbidden access to this register (read/write access)");
        }
    }
    else
    {
        SetError("Cannot parse IO name: " + io);
    }

    return retValue;
}


Value Controllino::ReadRequest(uint16_t start_addr, std::uint16_t size)
{
    Value retValue;    
    IProcessEngine *engine = GetProcessEngine();

    if (engine != nullptr)
    {
        IModbusMaster *modbus = engine->GetModbusChannel(GetConnectionChannel());
        if (modbus != nullptr)
        {
            int32_t req_size = modbus->BuildFunc3Packet(MODBUS_RTU, mSlaveAddress, start_addr, size);

            if (req_size > 0)
            {
                if (modbus->ModbusRequest(static_cast<std::uint32_t>(req_size), mSlaveAddress, 4))
                {
                    if (size == 1)
                    {
                        uint16_t value = modbus->GetUint16Be(0);
                        retValue = Value(static_cast<std::int32_t>(value));
                    }
                    else
                    {
                        uint32_t readValue = modbus->GetUint32Be(0);
                        retValue = Value(static_cast<std::int32_t>(readValue));
                    }
                }
                else
                {
                    SetError(modbus->GetModbusError());
                }
            }
            else
            {
                SetError("Modbus request error: " + std::to_string(req_size));
            }
        }
        else
        {
            SetError("Cannot find modbus channel ID: " + GetConnectionChannel());
        }
    }
    else
    {
        SetError("Cannot communicate with process engine");
    }

    return retValue;
}

Value Controllino::WriteRequest(std::uint16_t start_addr, uint8_t *data, std::uint16_t size)
{
    Value retValue;
    IProcessEngine *engine = GetProcessEngine();

    if (engine != nullptr)
    {
        IModbusMaster *modbus = engine->GetModbusChannel(GetConnectionChannel());
        if (modbus != nullptr)
        {
            int32_t req_size = modbus->BuildFunc16Packet(MODBUS_RTU, data, mSlaveAddress, start_addr, size);

            if (req_size > 0)
            {
                if (modbus->ModbusRequest(static_cast<std::uint32_t>(req_size), mSlaveAddress, 4))
                {
                    uint16_t value = modbus->GetUint16Be(0);
                    retValue = Value(static_cast<std::int32_t>(value));
                }
                else
                {
                    SetError(modbus->GetModbusError());
                }
            }
            else
            {
                SetError("Modbus request error: " + std::to_string(req_size));
            }
        }
        else
        {
            SetError("Cannot find modbus channel ID: " + GetConnectionChannel());
        }
    }
    else
    {
        SetError("Cannot communicate with process engine");
    }

    return retValue;
}



void Controllino::Initialize()
{
    uint32_t addr = Util::FromString<uint32_t>(GetDeviceOptions());
    mSlaveAddress = static_cast<uint8_t>(addr);
}


void Controllino::Stop()
{

}
