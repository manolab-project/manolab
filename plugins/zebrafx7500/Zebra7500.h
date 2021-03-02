#ifndef ZEBRA7500_H
#define ZEBRA7500_H

#include "PluginBase.h"
#include "rfidapi.h"
#include <thread>
#include "IProcessEngine.h"
#include "EventLoop.h"
#include "Util.h"

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

        enum TagState {
            DISCOVERABLE,
            CAPTURED,
        };

        int64_t first_seen;
        int64_t last_seen;
        uint32_t counter;
        uint32_t prev;
        uint32_t save_counter;
        TagState state;
        bool blocked;
        uint64_t id;

        TagInfo() {
            first_seen = Util::CurrentTimeStamp64();
            last_seen = first_seen;
            counter = 0;
            prev = 0;
            state = DISCOVERABLE;
            blocked = true;
            id = 0;
        }
    };

    std::map<uint64_t, TagInfo> mTags;

    mano::IPlugin::ICallBack *mCb = nullptr;
    bool mInitialized = false;

    EventLoop mEv;

    bool Initialize();
    void Stop();
    bool Execute(const std::vector<Value> &args, Value &ret);
    void InventoryLoop();
    void ManageTagEvent(uint64_t tid);
    void ManageTimeout();
    void SendToManolab(int64_t id);
};

#endif // ZEBRA7500_H
