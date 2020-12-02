
#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H

#include <stdexcept>

#include "IScriptEngine.h"
#include "IProcessEngine.h"

class DeviceBase : public IScriptEngine::IFunction
{

public:
    DeviceBase(const std::string &name);
    virtual ~DeviceBase();

    std::string GetType();

    void Take();
    void Release();
    bool IsUsed();

    void SetProcessEngine(IProcessEngine *engine);
    IProcessEngine *GetProcessEngine();

    void SetDeviceId(const std::string &id);
    std::string GetDeviceId();

    // Connection parameters
    void SetConnectionChannel(const std::string &conn);
    std::string GetConnectionChannel();
    void SetConnectionSettings(const std::string &params);
    std::string GetConnectionSettings();

    // Device specific parameters
    void SetDeviceOptions(const std::string &options);
    std::string GetDeviceOptions();

    // Error management
    void SetError(const std::string &err);
    std::string GetError() const;
    void ClearError();
    bool HasError() const;

    // To implement in device specific implementation
    virtual void AutoTest() = 0;
    virtual void Initialize() = 0;
    virtual void Stop() = 0;

    void SetInfo(const std::string &str);
private:
    std::string mName;
    std::string mConnSettings;
    std::string mId;
    std::string mConnChannel;
    std::string mDeviceOptions;
    bool mUsed;
    bool mHasError;
    std::string mError;
    IProcessEngine *mEngine;
};

#endif // DEVICE_BASE_H
