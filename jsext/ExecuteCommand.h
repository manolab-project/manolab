#ifndef EXECUTE_COMMAND_H
#define EXECUTE_COMMAND_H

#include <vector>
#include "IScriptEngine.h"


class ExecuteCommand : public IScriptEngine::IFunction
{
public:
    ExecuteCommand();

    // From IScriptEngine::IFunction
    bool Execute(const std::vector<Value> &args, Value &ret);
};

#endif // EXECUTE_COMMAND_H
