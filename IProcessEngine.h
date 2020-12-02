#ifndef I_PROCESS_ENGINE_H
#define I_PROCESS_ENGINE_H

#include <string>
#include "IModbusMaster.h"

class IProcessEngine
{
public:

    virtual ~IProcessEngine() {}

    virtual IModbusMaster *GetModbusChannel(const std::string &id) = 0;
};

#endif // I_PROCESS_ENGINE_H


