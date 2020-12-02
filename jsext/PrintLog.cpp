#include "PrintLog.h"
#include "Log.h"
#include <algorithm>

PrintLog::PrintLog()
{

}

bool PrintLog::Execute(const std::vector<Value> &args, Value &ret)
{
    bool normalCode = true;
    (void) ret;

    if (args.size() >= 1)
    {
        // First argument is the command
        if (args[0].GetType() == Value::STRING)
        {
            std::string type = args[0].GetString();
            std::string text;
            std::transform(type.begin(), type.end(), type.begin(), ::tolower);

            if (args.size() >= 1)
            {
                if (args[1].GetType() != Value::STRING)
                {
                    TLogWarning("printLog expected arguments to be of type string");
                }
                text = args[1].GetString();
            }

            if (type == "error")
            {
                TLogError(text);
                normalCode = false;
            }
            else if (type == "warning")
            {
                TLogWarning(text);
            }
            else if (type == "info")
            {
                TLogInfo(text);
            }
        }
        else
        {
            TLogWarning("printLog expected arguments to be of type string");
        }
    }
    else
    {
        TLogWarning("printLog expected two arguments");
    }

    return normalCode;
}
