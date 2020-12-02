#include "InputText.h"

#include <thread>
#include <chrono>
#include "Log.h"

InputText::InputText()
    : mAccepted(false)
{

}

bool InputText::Execute(const std::vector<Value> &val, Value &ret)
{
    int seconds = 60U;
    bool success = false;
    std::string showText;
    mStop = false;
    mAccepted = false;

    // Delay of zero means no timeout (no delay)
    if (val.size() >= 3)
    {
        seconds = val[0].GetInteger();
        showText = val[1].GetString();
        bool showControls = val[2].GetBool();

        // argument optionnel
        bool generateError = true;
        if (val.size() >= 4)
        {
            generateError = val[3].GetBool();
        }

        callback(showText, showControls);
        if (seconds > 0)
        {
            TLogInfo("Waiting for user input, max seconds to wait: " + std::to_string(seconds));
            do
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                seconds--;

                mMutex.lock();
                if (mStop)
                {
                    seconds = 0;
                }
                mMutex.unlock();
            }
            while (seconds > 0);

            callback("", false);

            if (!mAccepted)
            {
                if (generateError)
                {
                    TLogError("User input text failure: timeout or rejected by user");
                    ret = Value("");
                }
                else
                {
                    success = true;
                    ret = Value("!error!");
                }
            }
            else
            {
                success = true;
                ret = Value(mInputText);
            }
        }
        else
        {
            success = true;
            ret = Value(mInputText);
        }
    }

    return success;
}

void InputText::Stop()
{
    mMutex.lock();
    mStop = true;
    mMutex.unlock();
}


void InputText::SetText(const std::string &text, bool accepted)
{
    mMutex.lock();
    mAccepted = accepted;
    mInputText = text;
    mStop = true;
    mMutex.unlock();
}
