#include "BK8500.h"

#include <iostream>
#include <string>

#include "serial.h"
#include "Util.h"
#include "Log.h"

BK8500::BK8500() noexcept
    : DeviceBase("BK8500")
{

}

BK8500::~BK8500()
{

}

bool BK8500::Execute(const std::vector<Value> &args, Value &ret)
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
            else if (cmd == std::string("TEST"))
            {
                AutoTest();
            }
            else if (cmd == std::string("SET_REMOTE"))
            {
                SetRemote();
            }
            else if (cmd == std::string("REMOTE_SENSING_ON"))
            {
                SetRemoteSensing(true);
            }
            else if (cmd == std::string("REMOTE_SENSING_OFF"))
            {
                SetRemoteSensing(false);
            }
            else if (cmd == std::string("LOAD_ON"))
            {
                SwitchOnOff(true);
            }
            else if (cmd == std::string("LOAD_OFF"))
            {
                SwitchOnOff(false);
            }
            else if (cmd == std::string("SET_MAX_VOLTAGE"))
            {
                if (args.size() >= 2)
                {
                    SetValue(MAX_VOLTAGE, args[1].GetInteger());
                }
            }
            else if (cmd == std::string("SET_MAX_CURRENT"))
            {
                if (args.size() >= 2)
                {
                    SetValue(MAX_CURRENT, args[1].GetInteger());
                }
            }
            else if (cmd == std::string("SET_CC"))
            {
                if (args.size() >= 2)
                {
                    SetValue(CONSTANT_CURRENT, args[1].GetInteger());
                }
            }
            else if (cmd == std::string("SET_CV"))
            {
                if (args.size() >= 2)
                {
                    SetValue(CONSTANT_VOLTAGE, args[1].GetInteger());
                }
            }
            else if (cmd == std::string("SET_MODE_CC"))
            {
                SetConstantMode(MODE_CC);
            }
            else if (cmd == std::string("SET_MODE_CV"))
            {
                SetConstantMode(MODE_CV);
            }
        }
    }

    return !HasError();
}

std::string BK8500::BuildPacket(const std::string &cmd)
{
    std::string order;
    uint16_t checksum = 0U;
    order.reserve(26);
    order.resize(26);

    for (uint32_t i = 0; i < 25; i++)
    {
        order[i] = 0;
    }

    // Header of the command
    order[0] = static_cast<char>(0xAA);
    order[1] = static_cast<char>(0x00);

    for (std::uint32_t i = 0U; (i < cmd.size()) && (i < 23); i++)
    {
        order[i + 2U] = cmd[i];
    }

    for (uint32_t i = 0; i < 25; i++)
    {
        checksum += order[i];
    }
    checksum %= 256;
    order[25] = static_cast<char>(checksum);

    return order;
}

void BK8500::SetValue(ValueType type, double value)
{
    std::string cmd;

    char code;
    if (type == MAX_VOLTAGE)
    {
        code = 0x22;
    }
    else if (type == MAX_CURRENT)
    {
        code = 0x24;
        value *= 10; // unit is 100mA
    }
    else if (type == MAX_POWER)
    {
        code = 0x26;
    }
    else if (type == CONSTANT_VOLTAGE)
    {
        code = 0x2C;
    }
    else if (type == CONSTANT_CURRENT)
    {
        code = 0x2A;
        value *= 10; // unit is 100mA
    }
    else if (type == CONSTANT_POWER)
    {
        code = 0x2E;
    }
    else
    {
        code = 0x00; // error
        SetError("Value type not managed!");
    }

   // std::uint32_t dec_value = static_cast<std::uint32_t>(value * 1000);

    std::uint32_t dec_value = value;

    std::cout << "BK8500 set value: " << value << " decimal: " << dec_value << std::endl;
    cmd.push_back(code);
    cmd.push_back(static_cast<char>(dec_value & 0xFF));
    cmd.push_back(static_cast<char>((dec_value>>8) & 0xFF));
    cmd.push_back(static_cast<char>((dec_value>>16) & 0xFF));
    cmd.push_back(static_cast<char>((dec_value>>24) & 0xFF));

    cmd = BuildPacket(cmd);

    std::string response;
    if (SendToDevice(cmd, response))
    {
        if (CheckResponse(response))
        {
            SetInfo("Set remote");
        }
    }
}

void BK8500::SetRemote()
{
    std::string cmd;
    cmd.push_back(static_cast<char>(0x20));
    cmd.push_back(static_cast<char>(0x01));

    cmd = BuildPacket(cmd);

    std::string response;
    if (SendToDevice(cmd, response))
    {
        if (CheckResponse(response))
        {
            SetInfo("Set remote");
        }
    }
}

void BK8500::SetConstantMode(Mode mode)
{
    std::string cmd;
    cmd.push_back(static_cast<char>(0x28));

    if (mode == MODE_CC)
    {
        cmd.push_back(static_cast<char>(0x00));
    }
    else if (mode == MODE_CV)
    {
        cmd.push_back(static_cast<char>(0x01));
    }
    else if (mode == MODE_CW)
    {
        cmd.push_back(static_cast<char>(0x02));
    }
    else
    {
        SetError("Constant mode not managed");
    }

    cmd = BuildPacket(cmd);

    std::string response;
    if (SendToDevice(cmd, response))
    {
        if (CheckResponse(response))
        {
            std::string type = (mode == MODE_CC) ? "Constant Current" : (mode == MODE_CV) ? "Constant Voltage" : "Constant Power";
            SetInfo("Set " + type);
        }
    }
}

void BK8500::SetRemoteSensing(bool enable)
{
    std::string cmd;
    cmd.push_back(static_cast<char>(0x56));

    if (enable)
    {
        cmd.push_back(static_cast<char>(0x01));
    }
    else
    {
        cmd.push_back(static_cast<char>(0x00));
    }

    cmd = BuildPacket(cmd);

    std::string response;
    if (SendToDevice(cmd, response))
    {
        if (CheckResponse(response))
        {
            std::string type = enable ? "ON" : "OFF";
            SetInfo("Remote sensing " + type);
        }
    }
}

void BK8500::SwitchOnOff(bool enable)
{
    std::string cmd;
    cmd.push_back(static_cast<char>(0x21));

    if (enable)
    {
        cmd.push_back(static_cast<char>(0x01));
    }
    else
    {
        cmd.push_back(static_cast<char>(0x00));
    }

    cmd = BuildPacket(cmd);

    std::string response;
    if (SendToDevice(cmd, response))
    {
        if (CheckResponse(response))
        {
            std::string type = enable ? "ON" : "OFF";
            SetInfo("Switch " + type);
        }
    }
}

void BK8500::GetIdentifiers()
{
    std::string cmd;
    cmd.push_back(static_cast<char>(0x6A));
    cmd = BuildPacket(cmd);

    std::string response;
    if (SendToDevice(cmd, response))
    {
        if (CheckResponse(response))
        {
            SetInfo("Read device identifiers");
        }
    }

/*
       //   Exemple de retour pour le Get product information

 0000 aa,  00,  6a,  38,  35,  30,  30,  00,  84,  01,  31,  36,  38,  37,  35,  31,   ..j8500...168751
 0010 00,  00,  10,  88,  22,  4f,  04,  00,  00,  00,                     ...."O....

 */

}

bool BK8500::SendToDevice(const std::string &packet, std::string &dataRead)
{
    bool success = false;
    if (mPort.IsOpen())
    {
        if (mPort.Write(packet) == SerialPort::cPortWriteSuccess)
        {
            // 2 seconds timeout: FIXME: make it as a setting
            if (mPort.Read(dataRead, 2) == SerialPort::cPortReadSuccess)
            {
                success = true;
            }
        }
    }

    if (!success)
    {
        SetError(mPort.GetLastError());
    }

    return success;
}

bool BK8500::CheckResponse(const std::string &response)
{
    bool success = false;
    if ((response[0] == static_cast<char>(0xAA)) && (response[1] == static_cast<char>(0x00)))
    {
        if (response[3] == static_cast<char>(0x12))
        {
            // Command status, check it
            std::uint8_t status = static_cast<std::uint8_t>(response[4]);
            if (status == 0x80)
            {
                success = true;
            }
            else if (status == 0x90)
            {
                SetError("Bad request checksum");
            }
            else if (status == 0xA0)
            {
                SetError("Bad parameter");
            }
            else if (status == 0xB0)
            {
                SetError("Unkown command");
            }
            else if (status == 0xC0)
            {
                SetError("Invalid command");
            }
        }
    }
    else
    {
        SetError("Not a valid BK8500 frame");
    }

    return success;
}


void BK8500::AutoTest()
{

    GetIdentifiers();
}

void BK8500::Initialize()
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


void BK8500::Stop()
{
    mPort.Close();
}
