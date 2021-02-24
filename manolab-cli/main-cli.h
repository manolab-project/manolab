#ifndef MAIN_CLI_H
#define MAIN_CLI_H

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
    void run();

signals:
    void ended();

private:
    CommandLine &opt;
    ProcessEngine engine;
    bool finished = false;
    QString filesPath;

    void ReadSettings(ProcessEngine &engine);
    void WriteSettings(const ProcessEngine &engine);
    void LoadScript(ProcessEngine &engine, const std::string &filename);
    void EngineEvents(int signal, const std::vector<Value> &args);

};

#endif // MAIN_CLI_H
