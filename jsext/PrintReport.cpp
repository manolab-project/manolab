#include "PrintReport.h"
#include "Log.h"
#include "Util.h"

// C++ / STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

PrintReport::PrintReport()
{

}

bool PrintReport::Execute(const std::vector<Value> &args, Value &ret)
{
    bool normalCode = true;
    (void) ret;

    if (args.size() >= 1)
    {
        // First argument is the command
        if ((args[0].GetType() == Value::STRING))
        {
            std::string str = args[0].GetString();
            std::fstream f;

            // Make sure the output directory exists
            Util::Mkdir(Util::GetDirectoryPath(mReportFullFileName));

            f.open(mReportFullFileName, std::ios_base::out | std::ios_base::app);

            if (f.is_open())
            {
                f << str << std::endl;
                f.close();
            }
            else
            {
                TLogWarning("PrintReport cannot open: " + mReportFullFileName);
            }
        }
        else
        {
            TLogWarning("PrintReport expected arguments to be of type string");
        }
    }
    else
    {
        TLogWarning("PrintReport expected one arguments");
    }

    return normalCode;
}

void PrintReport::SetReportFullFileName(const std::string &reportFullPath)
{
    mReportFullFileName = Util::EscapeChar(reportFullPath);
}

std::string PrintReport::GetReportFullFileName() const
{
    return mReportFullFileName;
}
