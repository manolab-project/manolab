#ifndef INPUT_TEXT_H
#define INPUT_TEXT_H

#include <functional>
#include <mutex>
#include "IScriptEngine.h"

class InputText : public IScriptEngine::IFunction
{
public:
    std::function< void(const std::string &text, bool showControls) > callback;

    InputText();

    void SetText(const std::string &text, bool accepted);
    bool Execute(const std::vector<Value> &val, Value &ret);

    void Stop();

private:
    std::mutex mMutex;
    std::string mInputText;
    bool mAccepted;
    bool mStop;
};

#endif // INPUT_TEXT_H
