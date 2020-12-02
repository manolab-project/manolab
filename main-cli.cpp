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

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

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
