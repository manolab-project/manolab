#include "PluginController.h"
#include <iostream>
#include "Util.h"
#include "Log.h"
#include "JsonValue.h"
#include "JsonReader.h"
#include "IProcessEngine.h"

PluginController::PluginController(IProcessEngine &engine)
    : mEngine(engine)
{

}
/*****************************************************************************/
void PluginController::Load()
{
    mLibs.clear();
    for (const auto &p : mList)
    {
        LoadOnePlugin(p);
    }
}
/*****************************************************************************/
void PluginController::SetPlugins(const std::vector<std::string> &plugins)
{
    mList = plugins;
}
/*****************************************************************************/
bool PluginController::LinkDevice(Device &device)
{
    for (auto & dev : mLibs)
    {
        if ((dev.second->info->className == device.type))
        {
            device.Reset();
            mEngine.RegisterJsFunction(device.name, dev.second);

            JsonReader reader;
            JsonValue json;

            // device.json contains the parameters for this plug-in
            if (reader.ParseString(json, device.json))
            {
                JsonObject data = json.GetObj();
                PluginController::PluginInterface::SendRequestToDevice("SetParameters", data, dev.second);

                // FIXME : add register listener

                if (!dev.second->success)
                {
                    device.error = dev.second->message; // copy error related to that device for user information
                    TLogError(device.error);
                }
                else
                {
                    TLogInfo("Device initialization success: " + device.name);
                    device.connected = true;
                }
            }

            break;
        }
    }

    return device.connected;
}
/*****************************************************************************/
std::string PluginController::Callback(int category, const char *req)
{
    if (category == 0)
    {
        mPool.enqueue_work([=](){
            TLogInfo(req);
        });
    }
    else
    {
        std::vector<Value> args;
        Value val = Value(req);
        val.SetJsonString(true);
        args.push_back(val);

        //TLogInfo(req);
        mPool.enqueue_work([=](){

            mEngine.SendEvent(IProcessEngine::SIG_TABLE_ACTION, args);
        });
    }
    return "";
}
/*****************************************************************************/
void PluginController::PluginInterface::SendRequestToDevice(const std::string &cmd, const JsonObject &data, std::shared_ptr<PluginInterface> p)
{
    p->success = false;
    JsonObject req;
    req.AddValue("cmd", cmd);
    req.AddValue("data", data);

    std::string res = p->plugin->Request(req.ToString());

    JsonReader reader;
    JsonValue json;

    if (reader.ParseString(json, res))
    {
        if (json.FindValue("cmd").GetString() == ("Reply" + cmd))
        {
            p->success = json.FindValue("success").GetBool();
            p->reply_data = json.FindValue("data").GetObj();
            p->message = json.FindValue("message").GetString();
        }
    }
}
/*****************************************************************************/
void PluginController::LoadOnePlugin(const std::string &name)
{
    std::string prefix;
    #ifdef USE_LINUX_OS
        prefix = "lib";
    #endif
    // Set the plugin shared library location
    std::string fullPath = Util::GetWorkingDirectory() +
            Util::DIR_SEPARATOR + "plugins"  +
            Util::DIR_SEPARATOR + prefix + name;
    #if USE_WINDOWS_OS
        fullPath += ".dll";
    #else
        fullPath += ".so";
    #endif
    // Load the shared library

    if (!Util::FileExists(fullPath))
    {
        TLogError("File does not exist: " + fullPath);
        return;
    }

    std::shared_ptr<PluginInterface> iface = std::make_shared<PluginInterface>();

    if (iface->lib.Open(fullPath))
    {
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
                TLogError("Plugin ABI version mismatch.");
            }
            else
            {
                // Instantiate the plugin
                iface->plugin = reinterpret_cast<mano::IPlugin*>(iface->info->GetInterface());

                // Register callback to here
                // ==> allow communication from the plugin to manolab
                iface->plugin->Register(this);

                mLibs[iface->info->pluginName] = iface;

                TLogInfo("Loaded plugin success: " + fullPath);

                // Close the plugin and free memory
              //  std::cout << "Closing" << std::endl;
             //   lib.Close();
            }
        }
        else
        {
            TLogError("Failed to load exports: " + iface->lib.GetErrorMessage());
        }
    }
    else
    {
        TLogError("Failed to load plugin: " + iface->lib.GetErrorMessage());
    }
}
