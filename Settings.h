#ifndef SETTINGS_H 
#define SETTINGS_H

#include "IProcessEngine.h"

class Settings
{
public:

    void ReadSettings(IProcessEngine &engine);
    void WriteSettings(const IProcessEngine &engine);
private:

};

#endif // SETTINGS_H
