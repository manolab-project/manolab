#include "ShowMessage.h"
#include <thread>
#include <chrono>
#include "Log.h"

ShowMessage::ShowMessage()
{

}

bool ShowMessage::Execute(const std::vector<Value> &args, Value &ret)
{
    bool success = true;
    if (args.size() > 2)
    {
        std::string cmd = args[0].GetString();
        if (cmd == std::string("IMAGE"))
        {
            std::string value = args[1].GetString();

        }
        else if (cmd == std::string("CLEAN"))
        {

        }
    }
    return success;
}
