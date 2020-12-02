#include "ExecuteCommand.h"
#include "Log.h"
#include "Util.h"

// C++ / STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


#ifdef USE_WINDOWS_OS
#include <windows.h>
#include <tchar.h>
#endif

#include <stdio.h>
#include <cstdlib>
#include <string>


ExecuteCommand::ExecuteCommand()
{

}

bool ExecuteCommand::Execute(const std::vector<Value> &args, Value &ret)
{
    bool success = false;

    if (args.size() >= 1)
    {
        // First argument is the command
        if ((args[0].GetType() == Value::STRING))
        {
            std::string command = args[0].GetString();
            std::string params = "";

            if (args.size() >= 2)
            {
                params = args[1].GetString();
            }

            std::string result, err;
            int32_t retCode = -1;

            std::uint32_t retCmd = Util::Exec(command, params, result, err, retCode);

            result = Util::EscapeChar(result);
            err = Util::EscapeChar(err);

            std::stringstream jsonStr;
            jsonStr << R"({ "status": )"    << retCmd <<
                       R"(, "output": ")"    << result <<
                       R"(", "error": ")"     << err <<
                       R"(", "retcode": )"   << retCode << "}";

            std::cout << "Execute command response: " << jsonStr.str() << std::endl;

            ret = Value(jsonStr.str());
            ret.SetJsonString(true);

            success = true;
        }
        else
        {
            TLogWarning("ExecuteCommand expected arguments to be of type string");
        }
    }
    else
    {
        TLogWarning("ExecuteCommand expected one arguments");
    }

    return success;
}

