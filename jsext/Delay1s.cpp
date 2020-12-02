#include "Delay1s.h"

#include <thread>
#include <chrono>
#include "Log.h"

Delay1s::Delay1s()
    : mStop(false)
{

}

bool Delay1s::Execute(const std::vector<Value> &val, Value &ret)
{
    (void) ret;
    int seconds = 1U;

    // Delay of minimum 1 second
    if (val.size() > 0)
    {
        seconds = val[0].GetInteger();
        if (seconds == 0U)
        {
            seconds = 1U;
        }
    }

    mMutex.lock();
    mStop = false;
    mMutex.unlock();

    do
    {
        callback(seconds);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        seconds--;

        mMutex.lock();
        if (mStop)
        {
            mStop = false;
            seconds = 0;
        }
        mMutex.unlock();
    }
    while (seconds > 0);
    callback(seconds); // last call

    return true;
}

void Delay1s::Stop()
{
    mMutex.lock();
    mStop = true;
    mMutex.unlock();
}
