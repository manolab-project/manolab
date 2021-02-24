#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "Environment.h"
#include "Util.h"
#include "Log.h"

Environment::Environment()
{

}


Environment::~Environment()
{

}



bool Environment::Execute(const std::vector<Value> &args, Value &ret)
{
    bool retOk = false;
    (void) ret;

    if (args.size() >= 1)
    {
        if (args[0].GetType() == Value::STRING)
        {
            if (args[0].GetString() == "GetWorkspace")
            {
                retOk = true;
                ret = Value(mWorkspace);
            }
        }
    }
    return retOk;
}

