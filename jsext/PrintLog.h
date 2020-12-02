#ifndef PRINT_LOG_H
#define PRINT_LOG_H

#include <vector>
#include "IScriptEngine.h"


class PrintLog : public IScriptEngine::IFunction
{
public:
    PrintLog();

    // From
    bool Execute(const std::vector<Value> &args, Value &ret);
};

#endif // PRINT_LOG_H
