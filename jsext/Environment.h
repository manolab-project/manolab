#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include <functional>
#include "DeviceBase.h"

class Environment : public IScriptEngine::IFunction
{

public:
    Environment();
    virtual ~Environment();

    std::string GetWorkspace() const { return mWorkspace; }
    void SetWorkspace(const std::string &path) { mWorkspace = path; }

    // From IScriptEngine::IFunction
    bool Execute(const std::vector<Value> &args, Value &ret);

private:
    std::string mWorkspace;
};

#endif // ENVIRONMENT_H
