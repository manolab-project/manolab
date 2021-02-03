#include "PluginController.h"
#include "Plugin.h"
#include "SharedLibrary.h"
#include <iostream>
#include "IPlugin.h"
#include "Util.h"

PluginController::PluginController()
{

}

void PluginController::Load(const std::string &dir, const std::string &name)
{
    // Set the plugin shared library location
    std::string fullPath = Util::GetCurrentDirectory() +
            Util::DIR_SEPARATOR + "modules"  +
            Util::DIR_SEPARATOR + dir +
            Util::DIR_SEPARATOR + name;
    #if WIN32
        # ifdef _DEBUG
            fullPath += "d.dll";
        # else
            fullPath += ".dll";
        # endif
    #else
        fullPath += ".so";
    #endif
    // Load the shared library
    std::cout << "Loading: " << fullPath << std::endl;
    SharedLibrary lib;
    lib.Open(fullPath);

    // Get plugin descriptor and exports
    mano::PluginDetails* info;
    if (lib.Sym("exports", reinterpret_cast<void**>(&info)))
    {
        std::cout << "Plugin Info: "
            << "\n\tAPI Version: " << info->apiVersion
            << "\n\tFile Name: " << info->fileName
            << "\n\tClass Name: " << info->className
            << "\n\tPlugin Name: " << info->pluginName
            << "\n\tPlugin Version: " << info->pluginVersion
            << std::endl;

        // API Version checking
        if (info->apiVersion != MANOLAB_PLUGIN_API_VERSION)
        {
                std::cout << "Plugin ABI version mismatch." << std::endl;
        }
        else
        {
            // Instantiate the plugin
            auto plugin = reinterpret_cast<mano::IPlugin*>(info->Initialize());

            // Call plugin methods
            plugin->onCommand("some:command", "random:data", 11);

            mLibs.push_back(lib);

            // Close the plugin and free memory
          //  std::cout << "Closing" << std::endl;
         //   lib.Close();
        }
    }

}
