#ifndef PLUGIN_CONTROLLER_H
#define PLUGIN_CONTROLLER_H

#include <string>
#include <map>
#include "SharedLibrary.h"
#include "IPlugin.h"
#include "PluginBase.h"
#include "SharedLibrary.h"
#include "IProcessEngine.h"
#include "JsonValue.h"
#include "PluginBase.h"
#include "Pool.h"


class PluginController : public mano::IPlugin::ICallBack
{
public:
    PluginController(IProcessEngine &engine);
    void Load();

    void SetPlugins(const std::vector<std::string> &plugins);

    struct PluginInterface : public std::enable_shared_from_this<PluginInterface>,  public IScriptEngine::IFunction {
        SharedLibrary lib;
        mano::PluginDetails* info;
        mano::IPlugin* plugin;
        JsonObject reply_data;
        bool success = false;
        std::string message;

        // Proxy, between JS world and the plugin
        // This function is called from the Javascript funtion
        bool Execute(const std::vector<Value> &val, Value &ret)
        {
            JsonArray array;

            for (const auto &v : val)
            {
                JsonValue json = mano::PluginBase::ValueToJson(v);
                if (json.IsValid())
                {
                    array.AddValue(json);
                }
            }
            JsonObject obj;
            obj.AddValue("args", array);
            SendRequestToDevice("Execute", obj, shared_from_this());
            if (success)
            {
                ret = mano::PluginBase::JsonToValue(reply_data);
            }
            return success;
        }

        static void SendRequestToDevice(const std::string &cmd, const JsonObject &data, std::shared_ptr<PluginInterface> plugin);
    };

    bool LinkDevice(Device &device);

    virtual std::string Callback(int category, const char *req);

private:
    IProcessEngine &mEngine;

    std::map<std::string, std::shared_ptr<PluginInterface>> mLibs;
    std::vector<std::string> mList;

    thread_pool mPool; // pool de thread pour amortir les actions des plugins

    void LoadOnePlugin(const std::string &name);
};

#endif // PLUGIN_CONTROLLER_H
