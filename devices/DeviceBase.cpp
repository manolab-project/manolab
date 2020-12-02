
#include <vector>
#include <iostream>

#include "DeviceBase.h"
#include "Log.h"
#include "Util.h"

DeviceBase::DeviceBase(const std::string &name)
    : mName(name)
    , mUsed(false)
    , mHasError(false)
    , mEngine(nullptr)
{

}

DeviceBase::~DeviceBase()
{

}

std::string DeviceBase::GetType()
{
    return mName;
}

void DeviceBase::Take()
{
    mUsed = true;
}

void DeviceBase::Release()
{
    mUsed = false;
}

bool DeviceBase::IsUsed()
{
    return mUsed;
}

void DeviceBase::SetProcessEngine(IProcessEngine *engine)
{
    mEngine = engine;
}

IProcessEngine *DeviceBase::GetProcessEngine()
{
    return mEngine;
}

void DeviceBase::SetDeviceId(const std::string &id)
{
    mId = id;
}

std::string DeviceBase::GetDeviceId()
{
    return mId;
}

void DeviceBase::SetConnectionChannel(const std::string &conn)
{
    mConnChannel = conn;
}

std::string DeviceBase::GetConnectionChannel()
{
    return mConnChannel;
}

void DeviceBase::SetConnectionSettings(const std::string &params)
{
    mConnSettings = params;
}

std::string DeviceBase::GetConnectionSettings()
{
    return mConnSettings;
}

void DeviceBase::SetDeviceOptions(const std::string &options)
{
    mDeviceOptions = options;
}

std::string DeviceBase::GetDeviceOptions()
{
    return mDeviceOptions;
}

void DeviceBase::SetError(const std::string &err)
{
    mError = GetType() + ": " + err;
    mHasError = true;
}

void DeviceBase::SetInfo(const std::string &str)
{
    TLogInfo(GetType() + ": " + str);
}

std::string DeviceBase::GetError() const
{
    return mError;
}

void DeviceBase::ClearError()
{
    mHasError = false;
    mError.clear();
}

bool DeviceBase::HasError() const
{
    return mHasError;
}

