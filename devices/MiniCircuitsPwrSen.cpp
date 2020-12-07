#include "MiniCircuitsPwrSen.h"

#include <iostream>
#include <string>

#include "serial.h"
#include "Util.h"
#include "Log.h"


MiniCircuitsPwrSen::MiniCircuitsPwrSen() noexcept
    : DeviceBase ("MiniCircuitsPwrSen")
{
    mHandle = nullptr;
    mInitialized = false;
}

MiniCircuitsPwrSen::~MiniCircuitsPwrSen()
{

}

bool MiniCircuitsPwrSen::Execute(const std::vector<Value> &args, Value &ret)
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
            else if (cmd == std::string("GET_DEVICE_MODEL_NAME"))
            {
                if (args.size() >= 1)
                {
                    std::string data = args[1].GetString();

                    // Request state (cmd 0x81). The first byte is the report number (0x0).
                    mBuffer[0] = 104;
                    int res = hid_write(mHandle, mBuffer, 64);

                    // Read requested state
                    res = hid_read(mHandle, mBuffer, 64);
                    ret = std::string(reinterpret_cast<char *>(&mBuffer[1]));
                }
            }
        }
    }

    return retCode;
}

void  MiniCircuitsPwrSen::AutoTest()
{

}

#define MAX_STR 255
void MiniCircuitsPwrSen::Initialize()
{
    int res;

    // Initialize the hidapi library
    res = hid_init();

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    mHandle = hid_open(0x20ce, 0x11, NULL);

    if (mHandle != 0)
    {
        mInitialized = true;
    }
    else
    {
        SetError("Cannot open USB HID device");
        mInitialized = false;
        mHandle = nullptr;
    }
/*
        // Read Indexed String 1
        res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
        wprintf(L"Indexed String 1: %s\n", wstr);

        // Toggle LED (cmd 0x80). The first byte is the report number (0x0).
        buf[0] = 0x0;
        buf[1] = 0x80;
        res = hid_write(handle, buf, 65);

        // Request state (cmd 0x81). The first byte is the report number (0x0).
        buf[0] = 0x0;
        buf[1] = 0x81;
        res = hid_write(handle, buf, 65);

        // Read requested state
        res = hid_read(handle, buf, 65);

        // Print out the returned buffer.
        for (i = 0; i < 4; i++)
            printf("buf[%d]: %d\n", i, buf[i]);
*/

}


void MiniCircuitsPwrSen::Stop()
{
    if (mInitialized && (mHandle != nullptr))
    {
        // Close the device
        hid_close(mHandle);

        // Finalize the hidapi library
        (void)hid_exit();
    }
    mInitialized = false;
}



