#ifndef SHOW_MESSAGE_H
#define SHOW_MESSAGE_H

#include <functional>
#include <mutex>
#include "IScriptEngine.h"

class ShowMessage : public IScriptEngine::IFunction
{
public:
    std::function< void(const std::string &message) > callback;

    ShowMessage();

    bool Execute(const std::vector<Value> &args, Value &ret);

private:
    std::mutex mMutex;
};

#endif // SHOW_MESSAGE_H
