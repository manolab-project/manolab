#ifndef PROCESS_ENGINE_H
#define PROCESS_ENGINE_H

#include <vector>
#include <string>
#include <mutex>
#include <thread>

// ICL
#include "JSEngine.h"
#include "Log.h"
#include "ThreadQueue.h"

// Project
#include "VoltcraftPPS.h"
#include "Delay1s.h"
#include "PrintLog.h"
#include "PrintReport.h"
#include "SerialDevice.h"
#include "AcuDC.h"
#include "BK8500.h"
#include "Controllino.h"
#include "ManoLabServer.h"
#include "LonganCanModule.h"
#include "ExecuteCommand.h"
#include "ModbusMaster.h"
#include "IProcessEngine.h"
#include "MiniCircuitsPwrSen.h"
#include "PluginController.h"
#include "Environment.h"

// FIXME Modules Qt à porter
//#include "LabelPrinter.h"
//#include "SoundPlayer.h"
//#include "InputText.h"
//#include "ShowImage.h"

#ifdef USE_IXXAT
#include "CanDevice.h"
#endif

#define MAX_DEVICES 10


class ProcessEngine : public Observer<Log::Infos>, public IScriptEngine::IPrinter, public IProcessEngine
{
public:
    static const int cTestSkipped = 0;
    static const int cTestStarted = 1;
    static const int cTestFinished = 2;
    static const int cTestError = 3;


    explicit ProcessEngine();
    ~ProcessEngine();

    // From IScriptEngine::IPrinter
    virtual void Print(const std::string &msg);

    void Quit();
    void AutoTest();
    void RegisterEventEmitter(std::function<void (int, const std::vector<Value> &)> &func);
    void SelectAllTests(bool enalble);
    void SelectOneTest(unsigned int index, bool enable);
    void AcceptInputText(const std::string &text, bool accepted);
    std::string GetLabelImage();

    // IProcessEngine
    void Initialize();
    bool IsRunning();
    bool IsAdmin() const;
    bool IsLoaded() const;
    void SendEvent(Event ev, const std::vector<Value> &args);
    std::string GetDescription() const;
    std::string GetTestTitle(unsigned int index) const;
    void LoadScript(const std::string &fileName);
    std::vector<Node> GetTests();
    std::vector<std::string>  GetConnList();
    void Stop();
    void Start();
    void Pause();
    void Resume();
    void SetPlugins(const std::vector<std::string> &plugins);
    virtual std::string GetWorkspace() const;
    void SetWorkspace(const std::string &path);
    IModbusMaster *GetModbusChannel(const std::string &id);
    void RegisterJsFunction(const std::string &name, std::shared_ptr<IScriptEngine::IFunction> function);
    bool ScriptExists(const std::string &fileName);
    void SetTableEntry(const std::string &name, uint32_t line, const std::vector<Value> &columns);
    bool GetTableEntry(const std::string &name, uint32_t line, std::vector<Value> &columns);
    uint32_t GetTableSize(const std::string &name);

private:
    PluginController mPlugins;
    JSEngine mJsEngine;

    std::thread mThread;
    std::mutex mMutex;
    ThreadQueue<std::uint32_t> mQueue;

    std::vector<Node> mTests;
    std::vector<Device> mDevices;
    std::vector<std::string> mConnList;

    std::string mDescription;
    std::string mAdminPass;
    std::string mProduct;
    bool mRunning;
    bool mQuit;
    bool mInitialized;
    std::string mCurrentScript;
    std::string mCurrentConfig;

    // Event emitter
    std::function< void(int, const std::vector<Value>&) > mEventCallback;

    VoltCraftPPS mVoltcraftPPS[MAX_DEVICES];
    BK8500 mBK8500[MAX_DEVICES];
    ModbusMaster mModbusMaster[MAX_DEVICES];
#ifdef USE_IXXAT
    CanDevice mCanDevice[MAX_DEVICES];
#endif
    Controllino mControllino[MAX_DEVICES];
    AcuDC mAcuDC[MAX_DEVICES];
    LonganCanModule mLonganCan;

    SerialDevice mSerial;    
    ManoLabServer mManoLabServer;
    MiniCircuitsPwrSen mMiniCircuitsPwrSen;

    // Interne
    std::shared_ptr<PrintLog> mPrintLog;
    std::shared_ptr<Delay1s> mDelays1s;
    std::shared_ptr<PrintReport> mPrintReport;
    std::shared_ptr<ExecuteCommand> mExecCommand;
    std::shared_ptr<Environment> mEnvironment;

//    LabelPrinter mLabelPrinter;
//    std::shared_ptr<InputText> mInputText;
//    std::shared_ptr<ShowImage> mShowImage;
//    std::shared_ptr<SoundPlayer> mSoundPlayer;

    std::vector<std::shared_ptr<DeviceBase>> mDeviceList;
    std::string mBufferedLabelImage;

    struct Table
    {
        std::vector<std::vector<Value>> lines;
    };

    // key: table name
    // value: table contents
    std::map<std::string, Table> mSharedTable;

    void Run();
    void Join();
    // From Observer<std::string>
    virtual void Update(const Log::Infos &info);
    void Delay1sCallback(int value);
    bool ParseConfig();
    bool CreateDevice(Device &device);
    bool ParseTests();
    void SignalStep(const std::string &step_title, bool isStart);
    void SignalTest(int test, uint32_t step_size);
    void TestLoop();
    bool InitializeScriptContext();
    void CloseScriptContext();
    void InputTextCallback(const std::string &text, bool showControls);
    void ScanAvailableConnections();
    void LabelImage(bool show, const std::string &buffer);
    void CreateNewLogFiles();
    void DoLoadScript();
};

#endif // PROCESS_ENGINE_H
