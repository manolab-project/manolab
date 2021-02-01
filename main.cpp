#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QFontDatabase>
#include <QIcon>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QApplication>

#include <iostream>       // std::cerr
#include <exception>      // std::set_terminate
#include <cstdlib>        // std::abort
#include <csignal>


#include "Log.h"
#include "QEngineModel.h"
#include "Util.h"

#include "serial.h"

#ifdef USE_WINDOWS_OS
#include <windows.h>
#include <dbghelp.h>
#endif

#include <stdio.h>
#include <stdlib.h>


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
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    std::set_terminate(terminate_handler);

    qputenv("QT_OPENGL", "software");

    auto previous_handler = std::signal(SIGABRT, signal_handler);
    if (previous_handler == SIG_ERR)
    {
        std::cerr << "Setup failed\n";
        return EXIT_FAILURE;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QIcon::setThemeName("novea");

    QCoreApplication::setOrganizationName("Novea Energies");
    QCoreApplication::setOrganizationDomain("novea-energies.com");
    QCoreApplication::setApplicationName("NovProd");

    // We register new type so that the manuf engine can be instanciated in QML
    qmlRegisterType<QEngineModel>("fr.novea.manuf", 1, 0, "ManufEngine");

    QQmlApplicationEngine engine;

//    engine.addImageProvider(QLatin1String("engine"), new ColorPixmapProvider);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
