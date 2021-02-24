#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <string>
#include <map>
#include "SharedLibrary.h"
#include "IPlugin.h"
#include "Plugin.h"
#include "SharedLibrary.h"


class PluginController
{
public:
    PluginController();
    void Load();

    void SetPlugins(const std::vector<std::string> &plugins);

    struct PluginInterface {
        SharedLibrary lib;
        mano::PluginDetails* info;
        mano::IPlugin* plugin;
    };

private:
    std::map<std::string, std::shared_ptr<PluginInterface>> mLibs;
    std::vector<std::string> mList;
    void LoadOnePlugin(const std::string &name);
};

#endif // PLUGINCONTROLLER_H
