#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "IScriptEngine.h"

namespace mano {

class IPlugin : public IScriptEngine::IFunction
{
public:
    IPlugin() {};
    virtual ~IPlugin() {};

    virtual bool Initialize() = 0;
    virtual void Stop() = 0;
    virtual void AutoTest() = 0;

    // Identification
    virtual std::string GetName() = 0;
    virtual void SetDeviceId(const std::string &id) = 0;
    virtual std::string GetDeviceId() = 0;

    // Connection parameters
    virtual void SetConnectionChannel(const std::string &conn) = 0;
    virtual std::string GetConnectionChannel() = 0;
    virtual void SetConnectionSettings(const std::string &params) = 0;
    virtual std::string GetConnectionSettings() = 0;

    // Device specific parameters
    virtual void SetDeviceOptions(const std::string &options) = 0;
    virtual std::string GetDeviceOptions() = 0;

    // Error management
    virtual void SetError(const std::string &err) = 0;
    virtual std::string GetError() const  = 0;
    virtual void ClearError() = 0;
    virtual bool HasError() const = 0;

};


}  // namespace mano


#endif // IPLUGIN_H
