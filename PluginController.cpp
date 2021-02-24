#include "PluginController.h"
#include <iostream>
#include "Util.h"

PluginController::PluginController()
{

}

void PluginController::Load()
{
}

void PluginController::SetPlugins(const std::vector<std::string> &plugins)
{
    mList = plugins;
}

void PluginController::LoadOnePlugin(const std::string &name)
{
    // Set the plugin shared library location
    std::string fullPath = Util::GetCurrentDirectory() +
            Util::DIR_SEPARATOR + "plugins"  +
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

    std::shared_ptr<PluginInterface> iface = std::make_shared<PluginInterface>();

    iface->lib.Open(fullPath);

    // Get plugin descriptor and exports
    if (iface->lib.Sym("exports", reinterpret_cast<void**>(&iface->info)))
    {
        std::cout << "Plugin Info: "
            << "\n\tAPI Version: " << iface->info->apiVersion
            << "\n\tFile Name: " << iface->info->fileName
            << "\n\tClass Name: " << iface->info->className
            << "\n\tPlugin Name: " << iface->info->pluginName
            << "\n\tPlugin Version: " << iface->info->pluginVersion
            << std::endl;

        // API Version checking
        if (iface->info->apiVersion != MANOLAB_PLUGIN_API_VERSION)
        {
                std::cout << "Plugin ABI version mismatch." << std::endl;
        }
        else
        {
            // Instantiate the plugin
            iface->plugin = reinterpret_cast<mano::IPlugin*>(iface->info->GetInterface());

            mLibs[iface->info->pluginName] = iface;

            // Close the plugin and free memory
          //  std::cout << "Closing" << std::endl;
         //   lib.Close();
        }
    }
}
