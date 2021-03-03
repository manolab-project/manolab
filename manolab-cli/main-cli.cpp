#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QTimer>

#include <iostream>       // std::cerr
#include <exception>      // std::set_terminate
#include <cstdlib>        // std::abort
#include <csignal>

#include "Log.h"
#include "Util.h"
#include "GetOptions.h"
#include "serial.h"
#include "ProcessEngine.h"
#include "JsonReader.h"
#include "JsonWriter.h"

#ifdef USE_WINDOWS_OS
#include <windows.h>
#include <dbghelp.h>
#endif // USE_WINDOWS_OS

#include <stdio.h>
#include <stdlib.h>

#include "main-cli.h"


[[noreturn]] void signal_handler(int sig)
{
    std::fprintf(stderr, "Error: signal %d\n", sig);
    std::abort();
}

[[noreturn]] void terminate_handler()
{

    std::exception_ptr exptr = std::current_exception();
    // the only useful feature of std::exception_ptr is that it can be rethrown...
    try
    {
        std::rethrow_exception(exptr);
    }
    catch (std::exception &ex)
    {
        std::fprintf(stderr, "Terminated due to exception: %s\n", ex.what());
    }
    catch (...)
    {
        std::fprintf(stderr, "Terminated due to unknown exception\n");
    }


    std::abort();
}



int main(int argc, char *argv[])
{
    std::set_terminate(terminate_handler);

    auto previous_handler = std::signal(SIGABRT, signal_handler);
    if (previous_handler == SIG_ERR)
    {
        std::cerr << "Setup failed\n";
        return EXIT_FAILURE;
    }

    CommandLine opt(argc, argv);

    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName("D8S EURL");
    QCoreApplication::setOrganizationDomain("d8s.eu");
    QCoreApplication::setApplicationName("Manolab");

    Task *task = new Task(&app, opt);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(task, SIGNAL(ended()), &app, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));

    return app.exec();
}

void Task::run()
{
    // Do processing here


    ReadSettings(engine);
    WriteSettings(engine);

    std::string workspaceDir = opt.GetOption("-w");

    if (workspaceDir.size() > 0)
    {
        engine.SetWorkspace(workspaceDir);
        std::cout << "Switched workspace to: " + workspaceDir << std::endl;
    }

    std::string p = engine.GetWorkspace() + "/scripts";

    filesPath = p.c_str();
    QList<QString> files;

    QDir directory(filesPath);
    files = directory.entryList(QStringList() << "*.js", QDir::Files);

    std::string currentTestFile = opt.GetOption("-f");
    std::cout << "Running test file: " << currentTestFile << std::endl;

    std::function< void(int, const std::vector<Value>&) > cb = std::bind( &Task::EngineEvents, this, std::placeholders::_1 , std::placeholders::_2 );
    engine.RegisterEventEmitter(cb);

    for (const auto & f : files)
    {
        std::cout << f.toStdString() << std::endl;
    }

    std::string loopDelay = opt.GetOption("-d");
    int delayBetweenLoops = 500;

    if (loopDelay.size() > 0)
    {
        delayBetweenLoops = Util::FromString<int>(loopDelay);
        engine.SetWorkspace(workspaceDir);
        std::cout << "Using delay between loops: " + std::to_string(delayBetweenLoops) << "ms" << std::endl;
    }

    std::string loopCounter = opt.GetOption("-l");
    int loopsMax = -1;

    if (loopCounter.size() > 0)
    {
        loopsMax = Util::FromString<int>(loopCounter);
        engine.SetWorkspace(workspaceDir);
        std::cout << "Number of script executions: " + std::to_string(loopsMax) << std::endl;
    }

    if (loopsMax <= 0)
    {
        std::cout << "Looping forever " << std::endl;
    }

    int loops = 0;
    bool processEnded = false;

    do
    {
        LoadScript(engine, currentTestFile);

        while(!finished); // wait load script finish event FIXME : add timeout
        finished = false;

        engine.Start();

        while(!finished); // wait script finish event FIXME : add timeout

        std::this_thread::sleep_for(std::chrono::milliseconds(delayBetweenLoops));

        loops++;

        if (loopsMax > 0)
        {
            if (loops >= loopsMax)
            {
                processEnded = true;
            }
        }
    }
    while(!processEnded);


    emit ended();
}



void Task::LoadScript(ProcessEngine &engine, const std::string &filename)
{
    QString filePath = filesPath + "/" + filename.c_str();

    if (QFileInfo::exists(filePath))
    {
        engine.LoadScript(filePath.toStdString());
    }
    else
    {
        TLogError("cannot read input file");
    }
}

void Task::EngineEvents(int signal, const std::vector<Value> &args)
{
    switch (signal)
    {

    case ProcessEngine::SIG_DELAY_1S:
        if (args.size() > 0)
        {
            //            QMetaObject::invokeMethod(this, "sigDelay", Qt::QueuedConnection,
            //                                      Q_ARG(int, args[0].GetInteger()));

        }
        break;
    case ProcessEngine::SIG_TEST_NUMBER:
        if (args.size() > 1)
        {
            //            QMetaObject::invokeMethod(this, "sigTest", Qt::QueuedConnection,
            //                                      Q_ARG(int, args[0].GetInteger()),
            //                                      Q_ARG(int, args[1].GetInteger())
            //                                      );
        }
        break;
    case ProcessEngine::SIG_STEP_NUMBER:
        if (args.size() >= 2)
        {
            //            QMetaObject::invokeMethod(this, "sigStep", Qt::QueuedConnection,
            //                                      Q_ARG(QString, args[0].GetString().c_str()),
            //                                      Q_ARG(bool, args[1].GetBool()));
        }
        break;
    case ProcessEngine::SIG_TEST_FINISHED:
        //        QMetaObject::invokeMethod(this, "sigFinished", Qt::QueuedConnection);
        finished = true;
        break;

    case ProcessEngine::SIG_MESSAGE:
        if (args.size() > 0)
        {
            std::cout << args[0].GetString() << std::endl;
            //            QMetaObject::invokeMethod(this, "sigMessage", Qt::QueuedConnection,
            //                                      Q_ARG(QString, args[0].GetString().c_str()));
        }
        break;

    case ProcessEngine::SIG_LOADED:
        //        QMetaObject::invokeMethod(this, "sigScriptLoaded", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_INPUT_TEXT:
        if (args.size() >= 2)
        {
            //            QMetaObject::invokeMethod(this, "sigInputText", Qt::QueuedConnection,
            //                                      Q_ARG(QString, args[0].GetString().c_str()),
            //                                      Q_ARG(bool, args[1].GetBool()));
        }
        break;

    case ProcessEngine::SIG_AUTO_TEST_FINISHED:
        //        BuildComChannelModel(); // refresh model
        //        // THEN, update UI
        //        QMetaObject::invokeMethod(this, "sigAutoTestFinished", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_SHOW_IMAGE:
        //        QMetaObject::invokeMethod(this, "sigShowImage", Qt::QueuedConnection,
        //                                  Q_ARG(bool, args[0].GetBool()));
        break;

    case ProcessEngine::SIG_TEST_SKIPPED:
        //        QMetaObject::invokeMethod(this, "sigTestSkipped", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_TEST_ENDED:
        //        QMetaObject::invokeMethod(this, "sigTestEnded", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_TEST_ERROR:
        //        QMetaObject::invokeMethod(this, "sigTestError", Qt::QueuedConnection);
        break;

    default:
        TLogWarning("Un-managed signal: " + std::to_string(signal));
        break;
    }

    //QMetaObject::invokeMethod(this, "sigRunningChanged", Qt::QueuedConnection);
}
