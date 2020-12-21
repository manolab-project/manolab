#include "ModbusMaster.h"
#include "Log.h"

#include <JsonReader.h>
#include <sstream>
#include <iostream>
#include <thread>

ModbusMaster::ModbusMaster()
    : DeviceBase("ModbusMaster")
{

}

ModbusMaster::~ModbusMaster()
{

}

bool ModbusMaster::Execute(const std::vector<Value> &args, Value &ret)
{
    (void) ret;

    if (args.size() > 0)
    {
        // First argument is the command
        if (args[0].GetType() == Value::STRING)
        {
            std::string cmd = args[0].GetString();

            if (cmd == std::string("INIT"))
            {
                Initialize();
            }
            else if (cmd == std::string("FUNC_3_READ_HOLDING_REGISTERS"))
            {
                if (args.size() >= 4)
                {
                    int32_t slave_address = args[1].GetInteger();
                    int32_t start_address = args[2].GetInteger();
                    int32_t nb_words = args[3].GetInteger();
                    ret = Function3Request(static_cast<uint8_t>(slave_address),
                                           static_cast<uint16_t>(start_address),
                                           static_cast<uint16_t>(nb_words));
                }
                else
                {
                    SetError("Not enough parameters");
                }
            }
            else if (cmd == std::string("FUNC_4_READ_INPUT_REGISTERS"))
            {
                if (args.size() >= 4)
                {
                    int32_t slave_address = args[1].GetInteger();
                    int32_t start_address = args[2].GetInteger();
                    int32_t nb_words = args[3].GetInteger();
                    ret = Function4Request(static_cast<uint8_t>(slave_address),
                                           static_cast<uint16_t>(start_address),
                                           static_cast<uint16_t>(nb_words));
                }
                else
                {
                    SetError("Not enough parameters");
                }
            }
            else if ((cmd == std::string("FUNC_15_WRITE_MULTIPLE_COILS")) ||
                     (cmd == std::string("FUNC_16_WRITE_REGISTERS")))
            {
                if (args.size() >= 4)
                {
                    int32_t slave_address = args[1].GetInteger();
                    int32_t start_address = args[2].GetInteger();

                    if (args[3].IsJsonString())
                    {
                        JsonValue json;
                        JsonReader reader;

                        if (reader.ParseString(json, args[3].GetString()))
                        {
                            if (json.IsArray())
                            {
                                JsonArray array = json.GetArray();
                                uint8_t data[MAX_WORD_TO_WRITE * 2];
                                uint16_t nb_words = static_cast<uint16_t>(array.Size());

                                if ((nb_words > 0) && (nb_words <= MAX_WORD_TO_WRITE))
                                {
                                    for (uint32_t i = 0; i < nb_words; i++)
                                    {
                                        uint16_t word = static_cast<uint16_t>(array.GetEntry(i).GetInteger());
                                        data[i*2] = (word >> 8) & 0xFFU;
                                        data[(i*2) + 1] = word & 0xFFU;
                                    }
                                    if (cmd == std::string("FUNC_15_WRITE_MULTIPLE_COILS"))
                                    {
                                        ret = Function15Request(static_cast<uint8_t>(slave_address),
                                                               static_cast<uint16_t>(start_address),
                                                               data,
                                                               nb_words);
                                    }
                                    else
                                    {
                                        ret = Function16Request(static_cast<uint8_t>(slave_address),
                                                               static_cast<uint16_t>(start_address),
                                                               data,
                                                               nb_words);
                                    }

                                }
                                else
                                {
                                     SetError("Bad argument: bad JSON array size");
                                }
                            }
                            else
                            {
                                 SetError("Bad argument: expected JSON array");
                            }
                        }
                        else
                        {
                             SetError("Bad argument: bad JSON");
                        }
                    }
                    else
                    {
                        SetError("Bad argument: expected JSON");
                    }
                }
                else
                {
                    SetError("Not enough parameters");
                }
            }
            else if (cmd == std::string("FUNC_1_READ_COILS"))
            {
                if (args.size() >= 4)
                {
                    int32_t slave_address = args[1].GetInteger();
                    int32_t start_address = args[2].GetInteger();
                    int32_t nb_words = args[3].GetInteger();
                    ret = Function1Request(static_cast<uint8_t>(slave_address),
                                           static_cast<uint16_t>(start_address),
                                           static_cast<uint16_t>(nb_words));
                }
                else
                {
                    SetError("Not enough parameters");
                }
            }
            else if (cmd == std::string("FUNC_5_WRITE_SINGLE_COIL"))
            {
                if (args.size() >= 4)
                {
                    int32_t slave_address = args[1].GetInteger();
                    int32_t address = args[2].GetInteger();
                    bool force = args[3].GetBool();
                    ret = Function5Request(static_cast<uint8_t>(slave_address),
                                           static_cast<uint16_t>(address),
                                           force);
                }
                else
                {
                    SetError("Not enough parameters");
                }
            }
            else if (cmd == std::string("FUNC_6_WRITE_SINGLE_REGISTER"))
            {
                if (args.size() >= 4)
                {
                    int32_t slave_address = args[1].GetInteger();
                    int32_t address = args[2].GetInteger();
                    int32_t value = args[3].GetInteger();
                    ret = Function6Request(static_cast<uint8_t>(slave_address),
                                           static_cast<uint16_t>(address),
                                           value);
                }
                else
                {
                    SetError("Not enough parameters");
                }
            }
        }
    }

    return !HasError();
}

int32_t ModbusMaster::BuildFunc3Packet(modbus_mode_t mode, uint8_t slave, uint16_t start_addr, uint16_t size)
{
    return modbus_func3_4_request(3, mode, &mPacket[0], slave, start_addr, size);
}

int32_t ModbusMaster::BuildFunc4Packet(modbus_mode_t mode, uint8_t slave, uint16_t start_addr, uint16_t size)
{
    return modbus_func3_4_request(4, mode, &mPacket[0], slave, start_addr, size);
}

int32_t ModbusMaster::BuildFunc1Packet(modbus_mode_t mode, uint8_t slave, uint16_t start_addr, uint16_t size)
{
    return modbus_func1_request(mode, &mPacket[0], slave, start_addr, size);
}

int32_t ModbusMaster::BuildFunc5Packet(modbus_mode_t mode, uint8_t slave, uint16_t addr, bool force)
{
    return modbus_func5_request(mode, &mPacket[0], slave, addr, force);
}

int32_t ModbusMaster::BuildFunc6Packet(modbus_mode_t mode, uint8_t slave, uint16_t addr, uint16_t value)
{
    return modbus_func5_6_request(6, mode, &mPacket[0], slave, addr, value);
}

int32_t ModbusMaster::BuildFunc16Packet(modbus_mode_t mode, uint8_t *wr_data, uint8_t slave, uint16_t start_addr, uint16_t size)
{
    return modbus_func15_16_request(16, mode, &mPacket[0], wr_data, slave, start_addr, size);
}

int32_t ModbusMaster::BuildFunc15Packet(modbus_mode_t mode, uint8_t *wr_data, uint8_t slave, uint16_t start_addr, uint16_t size)
{
    return modbus_func15_16_request(15, mode, &mPacket[0], wr_data, slave, start_addr, size);
}

uint32_t ModbusMaster::GetUint32Be(uint8_t index)
{
    return modbus_reply_get_u32_be(&mPacket[0], index);
}

uint16_t ModbusMaster::GetUint16Be(uint8_t index)
{
    return modbus_reply_get_u16_be(&mPacket[0], index);
}

uint8_t ModbusMaster::GetUint8(uint8_t index)
{
    return mPacket[index];
}

bool ModbusMaster::HasModbusError() const
{
    return HasError();
}

std::string ModbusMaster::GetModbusError() const
{
    return GetError();
}

bool ModbusMaster::ModbusRequest(std::uint32_t size, uint8_t slave_address, std::int32_t timeout_sec)
{
    bool success = false;
    if (mPort.Write(&mPacket[0], size) == SerialPort::cPortWriteSuccess)
    {
        std::string readData;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        if (mPort.Read(readData, timeout_sec) == SerialPort::cPortReadSuccess)
        {
            uint32_t read_size = readData.size() > MAX_MODBUS_LENGTH ? MAX_MODBUS_LENGTH : readData.size();
            readData.copy(reinterpret_cast<char *>(&mPacket[0]), read_size);

            uint8_t response = modbus_reply_check(&mPacket[0], static_cast<uint16_t>(read_size), slave_address);
            if (response == 0U)
            {
                success = true;
            }
            else if (response == 1U)
            {
                SetError("Modbus reply error: bad slave number in reply");
            }
            else if (response == 2U)
            {
                SetError("Modbus reply error: exception");
            }
        }
        else
        {
            SetError("Modbus Read error");
        }
    }
    else
    {
        SetError("Modbus Write error");
    }
    return success;
}


Value ModbusMaster::Function3Request(uint8_t slave_address, uint16_t start_addr, uint16_t nbWords)
{
    return Function3_4Request(true, slave_address, start_addr, nbWords);
}

Value ModbusMaster::Function4Request(uint8_t slave_address, uint16_t start_addr, uint16_t nbWords)
{
    return Function3_4Request(false, slave_address, start_addr, nbWords);
}

Value ModbusMaster::Function3_4Request(bool isFunc3, uint8_t slave_address, uint16_t start_addr, uint16_t nbWords)
{
    Value ret;
    std::stringstream ss;
    int32_t req_size = 0;

    if (isFunc3)
    {
        req_size = BuildFunc3Packet(MODBUS_RTU, slave_address, start_addr, nbWords);
    }
    else
    {
        req_size = BuildFunc4Packet(MODBUS_RTU, slave_address, start_addr, nbWords);
    }

    if (req_size > 0)
    {
        if (ModbusRequest(static_cast<std::uint32_t>(req_size), slave_address, 4))
        {
            ss << R"({"success": true, "size": )" << nbWords << R"(, "data": [)";

            if (nbWords > 0)
            {
               for (std::uint32_t i = 0; i < (nbWords * 2); i += 2)
               {
                   uint16_t value = GetUint16Be(i);
                   ss << value;
                   if (i < ((nbWords - 1) * 2))
                   {
                       ss << ", ";
                   }
               }
            }

            ss  << "]}";

           std::cout << "CAN READ: " << ss.str() << std::endl;
        }
        else
        {
            SetError(GetModbusError());
            ss << R"({"success": false})";
        }
    }
    else
    {
        SetError("Modbus request error: " + std::to_string(req_size));
    }

    ret = Value(ss.str());
    ret.SetJsonString(true);
    return ret;
}


Value ModbusMaster::Function1Request(uint8_t slave_address, uint16_t start_addr, uint16_t nbCoils)
{
    Value ret;
    std::stringstream ss;
    int32_t req_size = BuildFunc1Packet(MODBUS_RTU, slave_address, start_addr, nbCoils);

    if (req_size > 0)
    {
        if (ModbusRequest(static_cast<std::uint32_t>(req_size), slave_address, 4))
        {
            ss << R"({"success": true, "size": )" << nbCoils << R"(, "data": [)";

            if (nbCoils > 0)
            {
               for (std::uint32_t i = 0; i < nbCoils; i++)
               {
                   uint16_t value =  mPacket[3 + i];
                   ss << value;
                   if (i < (nbCoils - 1))
                   {
                       ss << ", ";
                   }
               }
            }

            ss  << "]}";

           std::cout << "CAN READ: " << ss.str() << std::endl;
        }
        else
        {
            SetError(GetModbusError());
            ss << R"({"success": false})";
        }
    }
    else
    {
        SetError("Modbus request error: " + std::to_string(req_size));
    }

    ret = Value(ss.str());
    ret.SetJsonString(true);
    return ret;
}

Value ModbusMaster::Function5Request(uint8_t slave_address, uint16_t addr, bool force)
{
    Value ret;
    std::stringstream ss;
    int32_t req_size = BuildFunc5Packet(MODBUS_RTU, slave_address, addr, force);

    if (req_size > 0)
    {
        if (ModbusRequest(static_cast<std::uint32_t>(req_size), slave_address, 4))
        {
           ss << R"({"success": true})";
        }
        else
        {
            SetError(GetModbusError());
            ss << R"({"success": false})";
        }
    }
    else
    {
        SetError("Modbus request error: " + std::to_string(req_size));
    }

    ret = Value(ss.str());
    ret.SetJsonString(true);
    return ret;
}

Value ModbusMaster::Function6Request(uint8_t slave_address, uint16_t addr, uint16_t value)
{
    Value ret;
    std::stringstream ss;
    int32_t req_size = BuildFunc6Packet(MODBUS_RTU, slave_address, addr, value);

    if (req_size > 0)
    {
        if (ModbusRequest(static_cast<std::uint32_t>(req_size), slave_address, 4))
        {
           ss << R"({"success": true})";
        }
        else
        {
            SetError(GetModbusError());
            ss << R"({"success": false})";
        }
    }
    else
    {
        SetError("Modbus request error: " + std::to_string(req_size));
    }

    ret = Value(ss.str());
    ret.SetJsonString(true);
    return ret;
}

Value ModbusMaster::Function15Request(uint8_t slave_address, uint16_t start_addr, uint8_t *data, uint16_t nb_words)
{
    Value ret;
    std::stringstream ss;
    int32_t req_size = BuildFunc15Packet(MODBUS_RTU, data, slave_address, start_addr, nb_words);

    if (req_size > 0)
    {
        if (ModbusRequest(static_cast<std::uint32_t>(req_size), slave_address, 4))
        {
           ss << R"({"success": true})";
        }
        else
        {
            SetError(GetModbusError());
            ss << R"({"success": false})";
        }
    }
    else
    {
        SetError("Modbus request error: " + std::to_string(req_size));
    }

    ret = Value(ss.str());
    ret.SetJsonString(true);
    return ret;
}

Value ModbusMaster::Function16Request(uint8_t slave_address, uint16_t start_addr, uint8_t *data, uint16_t nb_words)
{
    Value ret;
    std::stringstream ss;
    int32_t req_size = BuildFunc16Packet(MODBUS_RTU, data, slave_address, start_addr, nb_words);

    if (req_size > 0)
    {
        if (ModbusRequest(static_cast<std::uint32_t>(req_size), slave_address, 4))
        {
           ss << R"({"success": true})";
        }
        else
        {
            SetError(GetModbusError());
            ss << R"({"success": false})";
        }
    }
    else
    {
        SetError("Modbus request error: " + std::to_string(req_size));
    }

    ret = Value(ss.str());
    ret.SetJsonString(true);
    return ret;
}


void ModbusMaster::AutoTest()
{
    // Nothing to do with this generic device
}


void ModbusMaster::Initialize()
{
    if (mPort.Open(GetConnectionChannel(), GetConnectionSettings()) == SerialPort::cPortAssociated)
    {
        TLogInfo(mPort.GetLastSuccess());
    }
    else
    {
        SetError(mPort.GetLastError());
    }
}

void ModbusMaster::Stop()
{
    mPort.Close();
}
