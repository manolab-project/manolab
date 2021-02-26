#ifndef ZEBRA7500_H
#define ZEBRA7500_H

#include "PluginBase.h"
#include "rfidapi.h"
#include <thread>
#include "IProcessEngine.h"

class Zebra7500 : public mano::PluginBase
{
public:
    Zebra7500();
    virtual ~Zebra7500();

    // From Manolab to plugin
    virtual std::string Request(const std::string &req);
    virtual bool Register(mano::IPlugin::ICallBack *cb);

private:
    RFID_HANDLE32 readerHandle;

    std::thread mThread;

    Device dev;

    struct TagInfo {
        std::chrono::time_point<std::chrono::high_resolution_clock> first_seen;
        uint32_t counter;
        uint32_t prev;
        bool newTag;

        TagInfo() {
            first_seen = std::chrono::high_resolution_clock::now();
            counter = 0;
            prev = 0;
            newTag = true;
        }
    };

    std::map<uint64_t, TagInfo> mTags;

    mano::IPlugin::ICallBack *mCb = nullptr;
    bool mInitialized = false;

    bool Initialize();
    void Stop();
    bool Execute(const std::vector<Value> &args, Value &ret);
    void InventoryLoop();
};

#endif // ZEBRA7500_H
