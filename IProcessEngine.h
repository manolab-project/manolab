#ifndef I_PROCESS_ENGINE_H
#define I_PROCESS_ENGINE_H

#include <string>
#include <vector>
#include "IModbusMaster.h"

class IProcessEngine
{
public:

    virtual ~IProcessEngine() {}

    virtual void SetPlugins(const std::vector<std::string> &plugins) = 0;
    virtual std::string GetWorkspace() const = 0;
    virtual void SetWorkspace(const std::string &path) = 0;
    virtual IModbusMaster *GetModbusChannel(const std::string &id) = 0;
};

#endif // I_PROCESS_ENGINE_H


