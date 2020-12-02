#ifndef DELAY_1S_H
#define DELAY_1S_H

#include <functional>
#include <mutex>
#include "IScriptEngine.h"

class Delay1s : public IScriptEngine::IFunction
{
public:
    std::function< void(int) > callback;

    Delay1s();

    bool Execute(const std::vector<Value> &val, Value &ret);

    void Stop();

private:
    std::mutex mMutex;
    bool mStop;
};

#endif // DELAY1S_H
