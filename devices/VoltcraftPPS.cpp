#include "VoltcraftPPS.h"

#include <iostream>
#include <string>

#include "serial.h"
#include "Util.h"
#include "Log.h"

VoltCraftPPS::VoltCraftPPS() noexcept
    : DeviceBase("VoltcraftPPS")
{

}

VoltCraftPPS::~VoltCraftPPS()
{

}

bool VoltCraftPPS::Execute(const std::vector<Value> &args, Value &ret)
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
            else if ((cmd == std::string("VOLT")) || cmd == std::string("SOVP"))
            {
                if (args.size() >= 2)
                {
                    std::int32_t voltage = args[1].GetInteger();

                    // Voltage is in mV
                    voltage = voltage / 100; // to 0,1 V

                    if (voltage == 0)
                    {
                        voltage = 1; // Voltcraft is buggy when 0V is sent
                    }

                    // ATTENTION: il faut toujours 3 digits pour la valeur numérique de la commande
                    std::string order = cmd + Util::ToLeadingZeros(voltage, 3) + "\r";

                    std::string response;
                    TLogInfo(GetType() + ": " + order);
                    (void) Request(order, response);
                }
            }
            else if ((cmd == std::string("CURR")) || cmd == std::string("SOCP"))
            {
                if (args.size() >= 2)
                {
                    std::int32_t current = args[1].GetInteger();

                    // Current is in mV
                    current = current / 10; // to 0,01 A
                    // ATTENTION: il faut toujours 3 digits pour la valeur numérique de la commande
                    std::string order = cmd + Util::ToLeadingZeros(current, 3) + "\r";

                    TLogInfo(GetType() + ": " + order);
                    std::string response;
                    (void) Request(order, response);
                }
            }
            else if (cmd == std::string("ON"))
            {
                std::string order = "SOUT0";
                std::string response;
                TLogInfo(GetType() + ": " + order);
            }
            else if (cmd == std::string("OFF"))
            {
                std::string order = "SOUT1";
                std::string response;
                TLogInfo(GetType() + ": " + order);
                (void) Request(order, response);
            }
            else if (cmd == std::string("READ"))
            {
                /**
                    150016001[CR]
                    OK[CR]

                    The PS Display value is 15V and
                    1.60A. It is in CC mode. (0==CV)
                 */

                std::string order = "GETD";
                std::string response;
                TLogInfo(GetType() + ": " + response);
                (void) Request(order, response);
            }
            else if (cmd == std::string("SET"))
            {
                if (args.size() >= 4)
                {
                    std::int32_t voltage = args[1].GetInteger();
                    std::int32_t current = args[2].GetInteger();
                    std::string onOff = args[3].GetString();

                    if (onOff == "ON")
                    {
                        onOff = "0";
                    }
                    else
                    {
                        onOff = "1";
                    }

                    // Voltage is in mV
                    voltage = voltage / 100; // to 0,1 V

                    if (voltage == 0)
                    {
                        voltage = 1; // Voltcraft is buggy when 0V is sent
                    }

                    // Current is in mV
                    current = current / 10; // to 0,01 A

                    // ATTENTION: il faut toujours 3 digits pour la valeur numérique des tension et courant
                    // EX:  SEVC1350000
                    std::string order = "SEVC" + Util::ToLeadingZeros(voltage, 3) + Util::ToLeadingZeros(current, 3) + onOff + "\r";

                    TLogInfo(GetType() + ": " + order);
                    std::string response;
                    (void) Request(order, response);
                }
            }



            /*

              Autres commandes vues  :
GETD
000600001
OK
*IDN?
GMOD
HCS-3404
OK
GMOD
HCS-3404
OK
GETM
050110138110550110
OK
GMAX
605110
OK
GOUT
1
OK
GOVP
605
OK
GOCP
100
OK
GETS
135000
OK
GETD
000600001
OK
GETD
000600001
OK
GETD
000600001
OK


                */
        }
    }

    return !HasError();
}

void VoltCraftPPS::AutoTest()
{
    std::string response;
    Request("GETS\r", response);
}


bool VoltCraftPPS::Request(const std::string &request, std::string &response)
{
    bool success = false;
    if (mPort.IsOpen())
    {
        if (mPort.Write(request) == SerialPort::cPortWriteSuccess)
        {
            if (mPort.Read(response, 2) == SerialPort::cPortReadSuccess)
            {
                //std::cout << response << std::endl;
                if (response.find("OK\r") != std::string::npos)
                {
                    success = true;
                }
                else
                {
                    SetError(GetType() + " : bad response: " + response);
                }
            }
            else
            {
                SetError(GetType() + " : read timeout");
            }
        }
        else
        {
            SetError(GetType() + " : write error");
        }
    }
    else
    {
        SetError(GetType() + " : device not open");
    }
    return success;
}

void VoltCraftPPS::Initialize()
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


void VoltCraftPPS::Stop()
{
    mPort.Close();
}
