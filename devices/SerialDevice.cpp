#include "SerialDevice.h"

#include <iostream>
#include <string>

#include "serial.h"
#include "Util.h"
#include "Log.h"

SerialDevice::SerialDevice() noexcept
    : DeviceBase ("SerialDevice")
{

}

SerialDevice::~SerialDevice()
{

}

bool SerialDevice::Execute(const std::vector<Value> &args, Value &ret)
{
    bool retCode = true;
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
            else if (cmd == std::string("WRITE"))
            {
                if (args.size() >= 2)
                {
                    std::string data = args[1].GetString();
                    mPort.Write(data);
                }
            }
        }
    }

    return retCode;
}

void  SerialDevice::AutoTest()
{

}


void SerialDevice::Initialize()
{

}


void SerialDevice::Stop()
{

}



