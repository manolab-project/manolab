#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <string>
#include <vector>
#include "SharedLibrary.h"


class PluginController
{
public:
    PluginController();
    void Load(const std::string &dir, const std::string &name);

private:
    std::vector<SharedLibrary> mLibs;
};

#endif // PLUGINCONTROLLER_H
