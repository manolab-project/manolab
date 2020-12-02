#ifndef PRINT_REPORT_H
#define PRINT_REPORT_H

#include <vector>
#include "IScriptEngine.h"


class PrintReport : public IScriptEngine::IFunction
{
public:
    PrintReport();

    // From IScriptEngine::IFunction
    bool Execute(const std::vector<Value> &args, Value &ret);

    void SetReportFullFileName(const std::string &reportFullPath);
    std::string GetReportFullFileName() const;

private:
    std::string mReportFullFileName;
};

#endif // PRINT_REPORT_H
