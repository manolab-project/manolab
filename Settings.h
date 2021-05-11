#ifndef SETTINGS_H 
#define SETTINGS_H

#include "IProcessEngine.h"

class Settings
{
public:

    void ReadSettings(IProcessEngine &engine);
    void WriteSettings(const IProcessEngine &engine);

private:
    std::vector<std::string> mPlugins;
    std::string mAutoLoadScriptName;
};

#endif // SETTINGS_H
