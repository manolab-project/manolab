#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <vector>
#include <string>
#include <cstdint>
#include <chrono>

#include "libserialport.h"

struct SerialInfos
{
    SerialInfos()
        : isFtdi(false)
        , isCypress(false)
        , associated(false)
    {

    }
    std::string friendName;
    std::string physName;
    std::string enumName;
    std::string hardwareIDs;
    std::string portName;
    std::string vendorId;
    std::string productId;
    std::string manufacturer;
    std::string pnpId;
    std::string serial;
    bool isFtdi;
    bool isCypress;
    bool associated;

    std::string FullUsbId()
    {
        return vendorId + ":" + productId;
    }
};

#define COM_PORT_BUF_SIZE   4096


class SerialPort
{
public:
    static const std::int32_t cPortReadTimeout      = -8;
    static const std::int32_t cPortReadError        = -7;
    static const std::int32_t cPortWriteError       = -6;
    static const std::int32_t cPortBadParameters    = -5;
    static const std::int32_t cPortOpenError        = -4;
    static const std::int32_t cPortNotOpen          = -3;
    static const std::int32_t cPortNotFree          = -2;
    static const std::int32_t cPortNotFound         = -1;
    static const std::int32_t cPortAssociated       = 0; // means, success
    static const std::int32_t cPortWriteSuccess     = 1;
    static const std::int32_t cPortReadSuccess      = 2;

    SerialPort();
    ~SerialPort();

    std::int32_t Open(const std::string &ident, const std::string &params);
    void Close();
    std::int32_t Write(const uint8_t *data, uint32_t size);
    std::int32_t Write(const std::string &data);
    std::int32_t Read(std::string &data);
    std::int32_t Read(std::string &data, const std::chrono::milliseconds &timeout_ms);
    bool IsOpen() const;
    std::string GetLastError();
    std::string GetLastSuccess();
    void Flush();

//    SerialHandle GetHandle() { return mFd; }

    static void EnumeratePorts();
    static std::int32_t AssociatePort(const std::string &ident, std::string &portName);
    static std::vector<SerialInfos> GetList();

private:
//    SerialHandle mFd;

    enum Mode {
        MODE_BLOCKING,
        MODE_NON_BLOCKING
    };

    struct sp_port *port = nullptr;
    bool mIsOpen = false;
    std::string mLastError;
    std::string mLastSuccess;

    Mode mMode = MODE_BLOCKING;
    uint32_t mBaudrate = 9600;
    uint32_t mDataBits = 8;
    uint32_t mStopBits = 1;
    enum sp_parity mParity;
    enum sp_flowcontrol mFlowControl = SP_FLOWCONTROL_NONE;
    uint32_t mReadEndOfFrame = 1000;
    uint32_t mReadTimeout = 1000;

    char mBuffer[COM_PORT_BUF_SIZE];
};

#endif // SERIALPORT_H
