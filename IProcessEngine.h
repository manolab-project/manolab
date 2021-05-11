#ifndef I_PROCESS_ENGINE_H
#define I_PROCESS_ENGINE_H

#include <string>
#include <vector>
#include <memory>
#include "IModbusMaster.h"
#include "IScriptEngine.h"
#include "Node.h"

struct Device
{
    std::string name;
    std::string type;
    std::string conn_channel;
    std::string conn_settings;
    std::string id;
    std::string options;

    bool connected; // Connected to a physical device
    std::string error;

    std::string json;

    Device()
        : connected(false)
    {

    }

    void Reset()
    {
        connected = false;
        error = "";
    }
};


class IProcessEngine
{
public:

    enum Event {
        SIG_DELAY_1S,
        SIG_TEST_NUMBER,
        SIG_STEP_NUMBER,
        SIG_TEST_SKIPPED,
        SIG_TEST_ENDED,
        SIG_TEST_ERROR,
        SIG_TEST_FINISHED,
        SIG_LOADED,
        SIG_MESSAGE,
        SIG_INPUT_TEXT,
        SIG_AUTO_TEST_FINISHED,
        SIG_SHOW_IMAGE,
        SIG_TABLE_ACTION,
    };

    virtual ~IProcessEngine() {}

    virtual void Initialize() = 0;
    virtual bool IsRunning() = 0;
    virtual bool IsAdmin() const = 0;
    virtual bool IsLoaded() const = 0;
    virtual void SendEvent(Event ev, const std::vector<Value> &args) = 0;
    virtual std::string GetDescription() const = 0;
    virtual std::string GetTestTitle(unsigned int index) const = 0;
    virtual void LoadScript(const std::string &fileName) = 0;
    virtual std::vector<Node> GetTests() = 0;
    virtual std::vector<std::string>  GetConnList() = 0;
    virtual void Stop() = 0;
    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void SetPlugins(const std::vector<std::string> &plugins) = 0;
    virtual std::string GetWorkspace() const = 0;
    virtual bool ScriptExists(const std::string &fileName) = 0;
    virtual void SetWorkspace(const std::string &path) = 0;
    virtual IModbusMaster *GetModbusChannel(const std::string &id) = 0;
    virtual void RegisterJsFunction(const std::string &name, std::shared_ptr<IScriptEngine::IFunction> function) = 0;
    virtual void SetTableEntry(const std::string &name, uint32_t line, const std::vector<Value> &columns) = 0;
    virtual bool GetTableEntry(const std::string &name, uint32_t line, std::vector<Value> &columns) = 0;
    virtual uint32_t GetTableSize(const std::string &name) = 0;
};

#endif // I_PROCESS_ENGINE_H


