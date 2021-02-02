#ifndef MANUFENGINE_H
#define MANUFENGINE_H

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
#include "LabelPrinter.h"
#include "SoundPlayer.h"
#include "BK8500.h"
#include "Controllino.h"
#include "ManoLabServer.h"
#include "LonganCanModule.h"
#include "ExecuteCommand.h"
#include "ModbusMaster.h"
#include "IProcessEngine.h"
#include "InputText.h"
#include "ShowImage.h"
#include "MiniCircuitsPwrSen.h"
#include "Zebra7500.h"

#ifdef USE_WINDOWS_OS
#include "CanDevice.h"
#endif

struct Test
{
    std::string title;
    bool enable;
    std::vector<std::string> steps;

    Test()
        : enable(true)
    {

    }
};


#define MAX_DEVICES 10

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


class ProcessEngine : public Observer<Log::Infos>, public IScriptEngine::IPrinter, public IProcessEngine, public IScriptEngine::IFunction
{

public:
    static const int cTestSkipped = 0;
    static const int cTestStarted = 1;
    static const int cTestFinished = 2;
    static const int cTestError = 3;


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
        SIG_SHOW_IMAGE
    };
    explicit ProcessEngine();
    ~ProcessEngine();

    // From IScriptEngine::IPrinter
    virtual void Print(const std::string &msg);

    // From IScriptEngine::IFunction
    bool Execute(const std::vector<Value> &args, Value &ret);

    void Initialize();
    bool IsRunning();
    bool IsAdmin() const;
    bool IsLoaded() const;
    std::string GetDescription() const;
    std::string GetTestTitle(unsigned int index) const;
    void LoadScript(const std::string &scriptFullPath);
    std::vector<Test> GetTests();
    std::vector<std::string>  GetConnList();
    void Stop();
    void Start();
    void Pause();
    void Resume();
    void Quit();
    void AutoTest();
    void RegisterEventEmitter(std::function<void (int, const std::vector<Value> &)> &func);
    void SelectAllTests(bool enalble);
    void SelectOneTest(unsigned int index, bool enable);
    void AcceptInputText(const std::string &text, bool accepted);
    std::string GetLabelImage();
    void SetWorkspace(const std::string &path);
    std::string GetWorkspace() const;

    IModbusMaster *GetModbusChannel(const std::string &id);

private:
    JSEngine mJsEngine;

    std::thread mThread;
    std::mutex mMutex;
    ThreadQueue<std::uint32_t> mQueue;

    std::vector<Test> mTests;
    std::vector<Device> mDevices;
    std::vector<std::string> mConnList;

    std::string mWorkspacePath;
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
#ifdef USE_WINDOWS_OS
    CanDevice mCanDevice[MAX_DEVICES];
#endif
    Controllino mControllino[MAX_DEVICES];
    AcuDC mAcuDC[MAX_DEVICES];
    LonganCanModule mLonganCan;
    LabelPrinter mLabelPrinter;
    SerialDevice mSerial;    
    ManoLabServer mManoLabServer;
    MiniCircuitsPwrSen mMiniCircuitsPwrSen;
    Zebra7500 mZebra7500;

    // Interne
    std::shared_ptr<ShowImage> mShowImage;
    std::shared_ptr<SoundPlayer> mSoundPlayer;
    std::shared_ptr<PrintLog> mPrintLog;
    std::shared_ptr<Delay1s> mDelays1s;
    std::shared_ptr<InputText> mInputText;
    std::shared_ptr<PrintReport> mPrintReport;
    std::shared_ptr<ExecuteCommand> mExecCommand;

    std::vector<std::shared_ptr<DeviceBase>> mDeviceList;
    std::string mBufferedLabelImage;

    void Run();
    void Join();
    // From Observer<std::string>
    virtual void Update(const Log::Infos &info);
    void Delay1sCallback(int value);
    bool ParseConfig();
    void CreateDevice(Device &device);
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

#endif // MANUFENGINE_H
