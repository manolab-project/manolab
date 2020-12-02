#ifndef MAIN_CLI_H
#define MAIN_CLI_H

#include <QApplication>
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

#include <stdio.h>
#include <stdlib.h>

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent, CommandLine &o)
        : QObject(parent)
        , opt(o)
    {

    }

public slots:
    void run()
    {
        // Do processing here


        ReadSettings(engine);
        WriteSettings(engine);

        std::string workspaceDir = opt.GetOption("-w");

        if (workspaceDir.size() > 0)
        {
            engine.SetWorkspace(workspaceDir);
            std::cout << "Switched workspace to: " + workspaceDir;
        }

        std::string p = engine.GetWorkspace() + "/files";

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
            engine.Start();

            while(!finished);

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

signals:
    void ended();

private:
    CommandLine &opt;
    ProcessEngine engine;
    bool finished = false;
    QString filesPath;

    void ReadSettings(ProcessEngine &engine)
    {
        QSettings settings("novprod.ini", QSettings::IniFormat);

        settings.beginGroup("Application");

        QString ws = settings.value("workspace", QCoreApplication::applicationDirPath()).toString();

        std::cout << "Current workspace is: " << ws.toStdString() << std::endl;

        engine.SetWorkspace(ws.toStdString());
        settings.endGroup();
    }

    void WriteSettings(ProcessEngine &engine)
    {
        QSettings settings("novprod.ini", QSettings::IniFormat);

        settings.beginGroup("Application");
        settings.setValue("workspace", engine.GetWorkspace().c_str());
        settings.endGroup();
    }

    void LoadScript(ProcessEngine &engine, const std::string &filename)
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

    void EngineEvents(int signal, const std::vector<Value> &args)
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

};

#endif // MAIN_CLI_H
