#include "AcuDC.h"

#include <iostream>
#include <string>

#include "Util.h"
#include "Log.h"
#include "IModbusMaster.h"

AcuDC::AcuDC() noexcept
    : DeviceBase("AcuDC")
{

}

AcuDC::~AcuDC()
{

}

bool AcuDC::Execute(const std::vector<Value> &args, Value &ret)
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
            else if (cmd == std::string("GET"))
            {
                if (args.size() >= 2)
                {
                    std::string data = args[1].GetString();

                    uint16_t nbWords = 2U;
                    uint16_t start_addr = 0U;

                    if (Util::ToUpper(data) == std::string("VOLTAGE"))
                    {
                        start_addr = 0x200U;
                    }
                    else if (Util::ToUpper(data) == std::string("CURRENT"))
                    {
                        start_addr = 0x202U;
                    }
                    else if (Util::ToUpper(data) == std::string("POWER"))
                    {
                        start_addr = 0x204U;
                    }
                    else if (Util::ToUpper(data) == std::string("SECONDS"))
                    {
                        start_addr = 0x289U;
                        nbWords = 1U;
                    }

                    ret = Read(start_addr, nbWords);
                }
            }
        }
    }

    return !HasError();
}

Value AcuDC::Read(uint16_t start_addr, uint16_t nbWords)
{
    Value ret;
    IProcessEngine *engine = GetProcessEngine();

    if (engine != nullptr)
    {
        IModbusMaster *modbus = engine->GetModbusChannel(GetConnectionChannel());
        if (modbus != nullptr)
        {
            int32_t req_size = modbus->BuildFunc3Packet(MODBUS_RTU, mSlaveAddress, start_addr, nbWords);

            if (req_size > 0)
            {
                if (modbus->ModbusRequest(static_cast<std::uint32_t>(req_size), mSlaveAddress, 4))
                {
                    if (nbWords == 2U)
                    {
                        union ValRead
                        {
                            uint32_t valueInt;
                            float valueFloat;
                        };

                        ValRead valRead;

                        valRead.valueInt = modbus->GetUint32Be(0);
                        ret = Value(static_cast<double>(valRead.valueFloat));
                    }
                    else
                    {
                        uint16_t value = modbus->GetUint16Be(0);
                        ret = Value(static_cast<std::int32_t>(value));
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
    return ret;
}


void AcuDC::AutoTest()
{
   Value slaveAddr = Read(0x101U, 1U);

   if (!HasError())
   {
       if (slaveAddr.GetInteger() != mSlaveAddress)
       {
           SetError("Autotest failed, wrong slave address for that device");
       }
   }
}


void AcuDC::Initialize()
{
    uint32_t addr = Util::FromString<uint32_t>(GetDeviceOptions());
    mSlaveAddress = static_cast<uint8_t>(addr);
}


void AcuDC::Stop()
{

}
