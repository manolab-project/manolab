
#include <iostream>
#include <functional>

#include "ProcessEngine.h"
#include "JsonReader.h"
#include "ProcessEngine.h"
#include "Util.h"


#ifndef EXE_NAME
    #define EXE_NAME "manolab"
#else
    #error "Please spcecify an executable name with define EXE_NAME"
#endif


static const std::uint32_t gEventNone       = 0U;
static const std::uint32_t gEventStart      = 1U;
static const std::uint32_t gEventPause      = 2U;
static const std::uint32_t gEventStop       = 3U;
static const std::uint32_t gEventQuit       = 4U;
static const std::uint32_t gEventAutoTest   = 5U;
static const std::uint32_t gEventLoad       = 6U;

ProcessEngine::ProcessEngine()
    : mPlugins(*this)
    , mRunning(false)
    , mQuit(false)
    , mInitialized(false)
{
    // Add specific devices to a global list
    for (int i = 0; i < MAX_DEVICES; i++)
    {
        mDeviceList.push_back(std::make_shared<VoltCraftPPS>());
        mDeviceList.push_back(std::make_shared<ModbusMaster>());
        mDeviceList.push_back(std::make_shared<BK8500>());

#ifdef USE_IXXAT
        mDeviceList.push_back(&mCanDevice[i]);
#endif
        mDeviceList.push_back(std::make_shared<Controllino>());
        mDeviceList.push_back(std::make_shared<AcuDC>());
    }

    mDeviceList.push_back(std::make_shared<LonganCanModule>());
    mDeviceList.push_back(std::make_shared<ManoLabServer>());
    mDeviceList.push_back(std::make_shared<MiniCircuitsPwrSen>());

    // Création des dev internes
    mDelays1s = std::make_shared<Delay1s>();
    mPrintLog = std::make_shared<PrintLog>();
    mPrintReport = std::make_shared<PrintReport>();
    mExecCommand = std::make_shared<ExecuteCommand>();
    mEnvironment = std::make_shared<Environment>();

    // FIXME
//     mDeviceList.push_back(std::make_shared<LabelPrinter>());
//    mInputText = std::make_shared<InputText>();
//    mShowImage = std::make_shared<ShowImage>();
//    mSoundPlayer = std::make_shared<SoundPlayer>();

    for (auto & dev : mDeviceList)
    {
        dev->SetProcessEngine(this);
    }

    Log::EnableSourceInfos(false);
    Log::RegisterListener(*this);

    // Create the thread the first time only
    if (!mInitialized)
    {
        mThread = std::thread(&ProcessEngine::Run, this);
        mInitialized = true;
    }
}
/*****************************************************************************/
ProcessEngine::~ProcessEngine()
{
    Quit();

    std::cout << "~ProcessEngine() called" << std::endl;

#ifdef USE_IXXAT
    // One time inits
    CanDevice::StopIxxatServer();
#endif
}
/*****************************************************************************/
void ProcessEngine::Initialize()
{
    // Script state variable
    mRunning = false;
    mQuit = false;
    mDevices.clear();
    mTests.clear();

    // FIXME NEW ARCH

    // C++ signals
    mDelays1s->callback = std::bind( &ProcessEngine::Delay1sCallback, this, std::placeholders::_1 );

    // FIXME
//    mInputText->callback = std::bind( &ProcessEngine::InputTextCallback, this, std::placeholders::_1, std::placeholders::_2 );
//    mLabelPrinter.callback = std::bind( &ProcessEngine::LabelImage, this, std::placeholders::_1, std::placeholders::_2 );
//    mShowImage->callback = std::bind( &ProcessEngine::LabelImage, this, std::placeholders::_1, std::placeholders::_2 );

}
/*****************************************************************************/
void ProcessEngine::CloseScriptContext()
{
    // Reload and reset all devices
    for (auto & dev : mDeviceList)
    {
        dev->Stop();
        dev->Release();
        dev->ClearError();
    }
}
/*****************************************************************************/
void ProcessEngine::ScanAvailableConnections()
{
    // Refresh serial port list
    SerialPort::EnumeratePorts();

    mConnList.clear();
    std::vector<SerialInfos> serList = SerialPort::GetList();
    for (const auto &ser : serList)
    {
        mConnList.push_back(ser.friendName + ":" + ser.portName);
    }

#ifdef USE_IXXAT
    std::vector<std::string> canList = CanDevice::GetList();
    for (const auto &can : canList)
    {
        mConnList.push_back(can);
    }
#endif
}
/*****************************************************************************/
bool ProcessEngine::InitializeScriptContext()
{
    bool ret = true;

#ifdef USE_IXXAT
    // One time inits
    CanDevice::StopIxxatServer();
    CanDevice::StartIxxatServer();
#endif

    mJsEngine.Close();
    mJsEngine.Initialize();
    mJsEngine.RegisterPrinter(this);
    mJsEngine.SetModuleSearchPath(mEnvironment->GetWorkspace() + "/modules");

    mJsEngine.RegisterFunction("delay1s", mDelays1s);
    mJsEngine.RegisterFunction("printLog", mPrintLog);
    mJsEngine.RegisterFunction("printReport", mPrintReport);
    mJsEngine.RegisterFunction("executeCommand", mExecCommand);
    mJsEngine.RegisterFunction(EXE_NAME, mEnvironment);

    // FIXME
//    mJsEngine.RegisterFunction("inputText", mInputText);
//    mJsEngine.RegisterFunction("playSound", mSoundPlayer);
//    mJsEngine.RegisterFunction("showImage", mShowImage);

    // Reload and reset all devices
    for (auto & dev : mDeviceList)
    {
        dev->Stop();
        dev->Release();
        dev->ClearError();
    }

    // Chargement des plugins
    mPlugins.Load();

    ScanAvailableConnections();

    // Assign all javascript devices to physical peripherals
    for (auto &dev : mDevices)
    {
        dev.Reset();
        CreateDevice(dev);

        if (!dev.connected)
        {
            // On tente les plug-ins
            if (!mPlugins.LinkDevice(dev))
            {
                TLogError("Cannot create unkown device: " + dev.type + " or too much devices created.");
                ret = false;
            }
            break;
        }
    }

    return ret;
}
/*****************************************************************************/
bool ProcessEngine::IsRunning()
{
    std::scoped_lock<std::mutex>  lock(mMutex);
    bool running;
    running = mRunning;
    return running;
}
/*****************************************************************************/
bool ProcessEngine::IsAdmin() const
{
    return mAdminPass.size() > 0 ? true : false;
}
/*****************************************************************************/
bool ProcessEngine::IsLoaded() const
{
    return mTests.size() > 0 ? true : false;
}
/*****************************************************************************/
void ProcessEngine::SendEvent(IProcessEngine::Event ev, const std::vector<Value> &args)
{
    mEventCallback(ev, args);
}
/*****************************************************************************/
std::string ProcessEngine::GetDescription() const
{
    return mDescription;
}
/*****************************************************************************/
std::string ProcessEngine::GetTestTitle(unsigned int index) const
{
    std::string title;
    if (index < mTests.size())
    {
        title = mTests[index].title;
    }
    return title;
}
/*****************************************************************************/
void ProcessEngine::SelectAllTests(bool enable)
{
    for (auto &test : mTests)
    {
        test.enable = enable;
    }
}
/*****************************************************************************/
void ProcessEngine::SelectOneTest(unsigned int index, bool enable)
{
    if (index < mTests.size())
    {
        mTests[index].enable = enable;
    }
}
/*****************************************************************************/
void ProcessEngine::AcceptInputText(const std::string &text, bool accepted)
{
    // FIXME
//   mInputText->SetText(text, accepted);
}
/*****************************************************************************/
std::string ProcessEngine::GetLabelImage()
{
    return mBufferedLabelImage;
}
/*****************************************************************************/
void ProcessEngine::SetWorkspace(const std::string &path)
{
    mEnvironment->SetWorkspace(path);
    Log::SetLogPath(path + Util::DIR_SEPARATOR + "logs");
}
/*****************************************************************************/
void ProcessEngine::SetPlugins(const std::vector<std::string> &plugins)
{
    mPlugins.SetPlugins(plugins);
}
/*****************************************************************************/
std::string ProcessEngine::GetWorkspace() const
{
    return mEnvironment->GetWorkspace();
}
/*****************************************************************************/
IModbusMaster *ProcessEngine::GetModbusChannel(const std::string &id)
{
    IModbusMaster *ptr = nullptr;
    for (auto &m : mModbusMaster)
    {
        if (m.GetDeviceId() == id)
        {
            ptr = &m;
        }
    }
    return ptr;
}
/*****************************************************************************/
void ProcessEngine::RegisterJsFunction(const std::string &name, std::shared_ptr<IScriptEngine::IFunction> function)
{
    mJsEngine.RegisterFunction(name, function);
}
/*****************************************************************************/
void ProcessEngine::CreateNewLogFiles()
{
    std::string logFileName = Util::GetFileName(mCurrentScript) + "_" + Util::CurrentDateTime("%Y%m%d_%H%M%S");
    Log::SetLogFileName("log_" + logFileName + ".csv");

    // FIXME NEW ARCH
  //  mPrintReport.SetReportFullFileName(mWorkspacePath + Util::DIR_SEPARATOR + "reports/report_" + logFileName + ".txt");
}
/*****************************************************************************/
void ProcessEngine::LoadScript(const std::string &scriptFullPath)
{
    mCurrentScript = scriptFullPath;
    mQueue.Push(gEventLoad);
}
/*****************************************************************************/
void ProcessEngine::DoLoadScript()
{
    bool success = false;
    Initialize();
    CreateNewLogFiles();

    mTests.clear();
    if (ParseTests())
    {
        if (ParseConfig())
        {
            success = true;
        }
    }

    // Cleanup in case of problem
    if (!success)
    {
        Initialize();
    }
}
/*****************************************************************************/
std::vector<Node> ProcessEngine::GetTests()
{
    std::scoped_lock<std::mutex>  lock(mMutex);
    std::vector<Node> t = mTests;
    return t;
}
/*****************************************************************************/
std::vector<std::string> ProcessEngine::GetConnList()
{
    return mConnList;
}
/*****************************************************************************/
void ProcessEngine::InputTextCallback(const std::string &text, bool showControls)
{
    std::vector<Value> args;
    args.push_back(Value(text));
    args.push_back(Value(showControls));
    if (mEventCallback)
    {
        mEventCallback(SIG_INPUT_TEXT, args);
    }
}
/*****************************************************************************/
void ProcessEngine::LabelImage(bool show, const std::string &buffer)
{
    mBufferedLabelImage = buffer;
    std::vector<Value> args;
    args.push_back(Value(show));
    if (mEventCallback)
    {
        mEventCallback(SIG_SHOW_IMAGE, args);
    }
}
/*****************************************************************************/
void ProcessEngine::Delay1sCallback(int value)
{
    std::vector<Value> args;
    args.push_back(Value(value));
    if (mEventCallback)
    {
        mEventCallback(SIG_DELAY_1S, args);
    }
}
/*****************************************************************************/
void ProcessEngine::Print(const std::string &msg)
{
    std::cout << "From JS: " << msg << std::endl;
}
/*****************************************************************************/
bool ProcessEngine::CreateDevice(Device &device)
{
    bool found = false;
    for (auto & dev : mDeviceList)
    {
        if ((dev->GetType() == device.type) && !dev->IsUsed())
        {
            found = true;

            dev->Take();
            dev->SetConnectionChannel(device.conn_channel);
            dev->SetConnectionSettings(device.conn_settings);
            dev->SetDeviceOptions(device.options);
            dev->SetDeviceId(device.id);
            mJsEngine.RegisterFunction(device.name, dev);

            // Now, initialize the device and autotest it (if available)
            dev->Initialize();

            if (dev->HasError())
            {
                TLogError(dev->GetError());
            }
            else
            {
                dev->AutoTest();
                if (dev->HasError())
                {
                    device.error = dev->GetError(); // copy error related to that device for user information
                    TLogError(device.error);
                }
                else
                {
                    TLogInfo("Device auto-test success: " + device.name);
                    device.connected = true;
                }
            }

            break;
        }
    }

    return found;
}
/*****************************************************************************/
bool ProcessEngine::ParseConfig()
{
    bool success = false;
    IScriptEngine::StringList args;

    InitializeScriptContext();

    if (mJsEngine.EvaluateFile(mCurrentConfig))
    {
        Value ret = mJsEngine.Call("setup", args);
        if ((ret.GetType() == Value::STRING) && ret.IsJsonString())
        {
            JsonReader reader;
            JsonValue json;
            if (reader.ParseString(json, ret.GetString()))
            {
                std::string pass = json.FindValue("admin_pass").GetString();

                if (pass != mAdminPass)
                {
                    mAdminPass.clear();
                }

                // Get the device list
                JsonValue value = json.FindValue("devices");
                mDevices.clear();
                if (value.IsArray())
                {
                    JsonArray &array = value.GetArray();
                    for (std::uint32_t i = 0; i < array.Size(); i++)
                    {
                        JsonValue entry = array.GetEntry(i);
                        bool bad_entry = false;
                        if (entry.IsObject())
                        {
                            Device dev;
                            dev.json = entry.GetObj().ToString();
                            dev.name = entry.FindValue("name").GetString();
                            dev.type = entry.FindValue("type").GetString();
                            dev.conn_channel = entry.FindValue("conn_channel").GetString();
                            dev.conn_settings = entry.FindValue("conn_settings").GetString();
                            dev.id = entry.FindValue("id").GetString();
                            dev.options = entry.FindValue("options").GetString();

                            mDevices.push_back(dev);
                        }
                        else
                        {
                            bad_entry = true;
                        }

                        if (bad_entry)
                        {
                           success = false;
                           TLogError("malformed test entry at index " + std::to_string(i));
                           break;
                        }
                        else
                        {
                            success = true;
                        }

                    }

                    TLogInfo("Loaded config file: " + mCurrentConfig);
                }
                else
                {
                    TLogError("Malformed config file, cannot find array of devices");
                }
            }
            else
            {
                TLogError("Malformed config file, not a JSON object");
            }
        }
        else
        {
            TLogError("Malformed config file, not a JSON object");
        }
    }
    else
    {
        TLogError("cannot read config file: " + mCurrentConfig + " error: "  + mJsEngine.GetLastError());
    }
    return success;
}
/*****************************************************************************/
bool ProcessEngine::ParseTests()
{
    bool success = false;
    IScriptEngine::StringList args;

    InitializeScriptContext();

    if (mJsEngine.EvaluateFile(mCurrentScript))
    {
        Value ret = mJsEngine.Call("setup", args);

        if ((ret.GetType() == Value::STRING) && ret.IsJsonString())
        {
            JsonReader reader;
            JsonValue json;

            if (reader.ParseString(json, ret.GetString()))
            {
                JsonValue value = json.FindValue("description");
                if (value.IsString())
                {
                    mDescription = value.GetString();
                }
                else
                {
                    TLogError("cannot find description field");
                }

                value = json.FindValue("product");
                if (value.IsString())
                {
                    mProduct = value.GetString();
                }
                else
                {
                    TLogError("cannot find product field");
                }

                value = json.FindValue("admin_pass");
                if (value.IsString())
                {
                    mAdminPass = value.GetString();
                }
                else
                {
                    mAdminPass.clear();
                    TLogWarning("cannot find admin password field");
                }

                value = json.FindValue("config");
                if (value.IsString())
                {
                    mCurrentConfig = mEnvironment->GetWorkspace() + Util::DIR_SEPARATOR + "config" + Util::DIR_SEPARATOR + value.GetString();
                }
                else
                {
                    mAdminPass.clear();
                    TLogError("cannot find configuration file field");
                }

                // Get the test list
                value = json.FindValue("tests");

                if (value.IsArray())
                {
                    JsonArray &array = value.GetArray();
                    for (std::uint32_t i = 0; i < array.Size(); i++)
                    {
                        JsonValue entry = array.GetEntry(i);
                        Node n;
                        if (n.Parse(entry))
                        {
                            success = true;
                            mTests.push_back(n);
                        }
                        else
                        {
                            success = false;
                            TLogError("malformed test entry at index " + std::to_string(i));
                            break;
                        }
                    }

                    TLogInfo("Loaded script file: " + mCurrentScript);
                }
                else
                {
                     TLogError("cannot read test list");
                }
            }
            else
            {
                TLogError("cannot read test object");
            }
        }
        else
        {
            TLogError("Call setup() in script file: " + mJsEngine.GetLastError());
        }
    }
    else
    {
        TLogError("cannot read test file: " + mCurrentScript + " error: " + mJsEngine.GetLastError());
    }

    return success;
}
/*****************************************************************************/
void ProcessEngine::Stop()
{
    mQueue.Push(gEventStop);

    // FIXME NEW ARCH
    // FIXME: call stop (or another method like Interrupt()?) to all devices
    mDelays1s->Stop();
    //mInputText.Stop();

    TLogInfo("Script stopped by user button");
}
/*****************************************************************************/
void ProcessEngine::Start()
{
    mQueue.Push(gEventStart);
}
/*****************************************************************************/
void ProcessEngine::Pause()
{
    mQueue.Push(gEventPause);
}
/*****************************************************************************/
void ProcessEngine::Resume()
{
    mQueue.Push(gEventStart);
}
/*****************************************************************************/
void ProcessEngine::Quit()
{
    Stop();
    mQueue.Push(gEventQuit);
    Join();
}
/*****************************************************************************/
void ProcessEngine::AutoTest()
{
    mQueue.Push(gEventAutoTest);
}
/*****************************************************************************/
void ProcessEngine::RegisterEventEmitter(std::function<void (int, const std::vector<Value> &)> &func)
{
    mEventCallback = func;
}
/*****************************************************************************/
void ProcessEngine::Join()
{
    if (mInitialized)
    {
        mThread.join();
        mInitialized = false;
    }
}
/*****************************************************************************/
void ProcessEngine::SignalTest(int test, std::uint32_t step_size)
{
    std::vector<Value> args;

    args.push_back(Value(static_cast<std::int32_t>(test)));
    args.push_back(Value(static_cast<std::int32_t>(step_size)));
    if (mEventCallback)
    {
        mEventCallback(Event::SIG_TEST_NUMBER, args);
    }
}
/*****************************************************************************/
void ProcessEngine::SignalStep(const std::string  &step_title, bool isStart)
{
    std::vector<Value> args;
    args.push_back(Value(step_title));
    args.push_back(Value(isStart));
    if (mEventCallback)
    {
        mEventCallback(Event::SIG_STEP_NUMBER, args);
    }
}
/*****************************************************************************/
void ProcessEngine::TestLoop()
{
    int test_number = 0;
    std::uint32_t ev = gEventNone;
    bool stopOnError = false;

    TLogInfo("Started script: " + mCurrentScript);
    TLogInfo("Script file date and time: " + Util::GetModifiedFileDateTime(mCurrentScript));
    TLogInfo("Product: " + mProduct + " (" + mDescription + ")");

    CreateNewLogFiles();

    // FIXME NEW ARCH
//    mManoLabServer.SetProduct(mProduct);
//    mManoLabServer.SetReportFile(mPrintReport.GetReportFullFileName());

    for (const auto & test : mTests)
    {
        if (!test.enable)
        {
            TLogInfo("Skipped test: " + test.title);
            mEventCallback(Event::SIG_TEST_SKIPPED, std::vector<Value>());
        }
        else
        {
            SignalTest(test_number, test.steps.size());
            TLogInfo("============  Running test: " + test.title);

            for (const auto & step : test.steps)
            {
                bool loopThisStep = false;
                bool quitThisNode = false;
                do
                {
                    if (step.mode == "loop")
                    {
                        loopThisStep = true;
                    }

                    TLogInfo("    --> Running step: " + step.name);
                    SignalStep(step.name, true);
                    IScriptEngine::StringList args;

                    (void) mJsEngine.Call(step.name, args);

                    if (mJsEngine.HasError())
                    {
                        stopOnError = true;
                        mEventCallback(Event::SIG_TEST_ERROR, std::vector<Value>());
                        // Throw JS error
                        TLogError(mJsEngine.GetLastError());
                        // Scan for device error
                        for (const auto & dev : mDeviceList)
                        {
                            if (dev->HasError())
                            {
                                TLogError(dev->GetError());
                                dev->ClearError();
                            }
                        }
                        args.clear();
                        (void) mJsEngine.Call("onError", args);

                        // Patch: l'appel à cette fonction peut elle aussi générer une erreur !!
                        if (mJsEngine.HasError())
                        {
                            TLogError(mJsEngine.GetLastError());
                        }
                    }

                    mQueue.TryPop(ev);

                    if (ev == gEventPause)
                    {
                        mQueue.WaitAndPop(ev);
                    }
                    else if ((ev == gEventStop) || (stopOnError))
                    {
                        loopThisStep = false;
                        quitThisNode = true;
                    }

                    // Step ok
                    SignalStep(step.name, false);
                }
                while(loopThisStep);

                if (quitThisNode)
                {
                    break;
                }
            }

            if ((ev == gEventStop) || (ev == gEventQuit) || (stopOnError))
            {
                break;
            }
            else
            {
                mEventCallback(Event::SIG_TEST_ENDED, std::vector<Value>());
            }
        }
        test_number++;
    }
}
/*****************************************************************************/
void ProcessEngine::Run()
{
    mMutex.lock();
    bool quit = mQuit;
    mMutex.unlock();

    while (!quit)
    {
        std::uint32_t ev = gEventNone;
        mQueue.WaitAndPop(ev);

        switch(ev)
        {

        case gEventStart:
            if (IsLoaded())
            {
                mMutex.lock();
                mRunning = true;
                mMutex.unlock();

                if (InitializeScriptContext())
                {
                    if (mJsEngine.EvaluateFile(mCurrentScript))
                    {
                        TestLoop();
                        CloseScriptContext();
                    }
                    else
                    {
                        TLogError("cannot read test file: " + mCurrentScript + " error: " + mJsEngine.GetLastError());
                    }
                }
                mMutex.lock();
                mRunning = false;
                mMutex.unlock();

                if (mEventCallback)
                {
                    mEventCallback(Event::SIG_TEST_FINISHED, std::vector<Value>());
                }
            }
            break;

        case gEventLoad:
        {
            mMutex.lock();
            mRunning = true;
            mMutex.unlock();

            DoLoadScript();

            if (IsLoaded())
            {
                if (InitializeScriptContext())
                {
                    TLogInfo("Script loaded");
                    IScriptEngine::StringList args;

                    if (mJsEngine.EvaluateFile(mCurrentScript))
                    {
                        (void) mJsEngine.Call("onStartup", args);
                    }
                    else
                    {
                        TLogError("cannot read test file: " + mCurrentScript + " error: " + mJsEngine.GetLastError());
                    }

                    CloseScriptContext();

                    if (mEventCallback)
                    {
                        std::vector<Value> args;
                        mEventCallback(Event::SIG_LOADED, args);
                    }
                }
            }
            mMutex.lock();
            mRunning = false;
            mMutex.unlock();

            if (mEventCallback)
            {
                mEventCallback(Event::SIG_TEST_FINISHED, std::vector<Value>());
            }

            break;
        }

        case gEventAutoTest:
        {
            mMutex.lock();
            mRunning = true;
            mMutex.unlock();

            ScanAvailableConnections();
            if (IsLoaded())
            {
                InitializeScriptContext();
            }
            std::vector<Value> args;

            mMutex.lock();
            mRunning = false;
            mMutex.unlock();

            if (mEventCallback)
            {
                mEventCallback(Event::SIG_TEST_FINISHED, std::vector<Value>());
            }
            break;
        }

        case gEventQuit:
            mMutex.lock();
            mQuit = true;
            mMutex.unlock();
            break;
        default:
                break;
        }

        // Refresh
        mMutex.lock();
        quit = mQuit;
        mMutex.unlock();
    }
}
/*****************************************************************************/
void ProcessEngine::Update(const Log::Infos &info)
{
    std::vector<Value> args;
    args.push_back(Value(info.ToString()));
    if (mEventCallback)
    {
        mEventCallback(Event::SIG_MESSAGE, args);
    }
}
