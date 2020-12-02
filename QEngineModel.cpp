#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include <QQuickImageProvider>
#include <QPainter>
#include <QCoreApplication>
#include <QMetaObject>
#include <QDebug>
#include <QSettings>

#include <Util.h>

#include "QEngineModel.h"

QEngineModel::QEngineModel()
{
    std::function< void(int, const std::vector<Value>&) > cb = std::bind( &QEngineModel::EngineEvents, this, std::placeholders::_1 , std::placeholders::_2 );
    mEngine.RegisterEventEmitter(cb);

    ReadSettings();
    WriteSettings();

    std::string p = mEngine.GetWorkspace() + "/files";

    mFilesPath = p.c_str();
    qDebug() << "Watch path: " << mFilesPath;
    mWatcher.addPath(mFilesPath);
    connect (&mWatcher, &QFileSystemWatcher::directoryChanged, this, &QEngineModel::slotDirectoryChanged);
    slotDirectoryChanged(mFilesPath); // Force updating file list at startup
}

void QEngineModel::WriteSettings()
{
    QSettings settings("novprod.ini", QSettings::IniFormat);

    settings.beginGroup("Application");
    settings.setValue("workspace", mEngine.GetWorkspace().c_str());
    settings.endGroup();
}

void QEngineModel::ReadSettings()
{
    QSettings settings("novprod.ini", QSettings::IniFormat);

    settings.beginGroup("Application");

    QString ws = settings.value("workspace", QCoreApplication::applicationDirPath()).toString();

    qDebug() << "Current workspace is: " << ws;

    mEngine.SetWorkspace(ws.toStdString());
    settings.endGroup();
}

void QEngineModel::slotDirectoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;
    QDir directory(path);
    mFiles = directory.entryList(QStringList() << "*.js", QDir::Files);
    emit sigFilesChanged();
}

void QEngineModel::loadScript(const QString &filename)
{
    QString filePath = mFilesPath + "/" + filename;

    if (QFileInfo::exists(filePath))
    {
        mEngine.LoadScript(filePath.toStdString());
    }
    else
    {
         TLogError("cannot read input file");
    }
}


QString QEngineModel::getTitle() const
{
    return mEngine.GetDescription().c_str();
}

QString QEngineModel::getWorkspace() const
{
    return mEngine.GetWorkspace().c_str();
}

bool QEngineModel::isRunning()
{
    return mEngine.IsRunning();
}

bool QEngineModel::isAdmin() const
{
    return mEngine.IsAdmin();
}

bool QEngineModel::isLoaded() const
{
    return mEngine.IsLoaded();
}

void QEngineModel::EngineEvents(int signal, const std::vector<Value> &args)
{
    switch (signal)
    {

    case ProcessEngine::SIG_DELAY_1S:
        if (args.size() > 0)
        {
            QMetaObject::invokeMethod(this, "sigDelay", Qt::QueuedConnection,
                                      Q_ARG(int, args[0].GetInteger()));

        }
        break;
    case ProcessEngine::SIG_TEST_NUMBER:
        if (args.size() > 1)
        {
            QMetaObject::invokeMethod(this, "sigTest", Qt::QueuedConnection,
                                        Q_ARG(int, args[0].GetInteger()),
                                        Q_ARG(int, args[1].GetInteger())
                                        );
        }
        break;
    case ProcessEngine::SIG_STEP_NUMBER:
        if (args.size() >= 2)
        {
            QMetaObject::invokeMethod(this, "sigStep", Qt::QueuedConnection,
                                      Q_ARG(QString, args[0].GetString().c_str()),
                                      Q_ARG(bool, args[1].GetBool()));
        }
        break;
    case ProcessEngine::SIG_TEST_FINISHED:
        QMetaObject::invokeMethod(this, "sigFinished", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_MESSAGE:
        if (args.size() > 0)
        {
            QMetaObject::invokeMethod(this, "sigMessage", Qt::QueuedConnection,
                                      Q_ARG(QString, args[0].GetString().c_str()));
        }
        break;

    case ProcessEngine::SIG_LOADED:
        QMetaObject::invokeMethod(this, "sigScriptLoaded", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_INPUT_TEXT:
        if (args.size() >= 2)
        {
            QMetaObject::invokeMethod(this, "sigInputText", Qt::QueuedConnection,
                                      Q_ARG(QString, args[0].GetString().c_str()),
                                      Q_ARG(bool, args[1].GetBool()));
        }
        break;

    case ProcessEngine::SIG_AUTO_TEST_FINISHED:
        BuildComChannelModel(); // refresh model
        // THEN, update UI
        QMetaObject::invokeMethod(this, "sigAutoTestFinished", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_SHOW_IMAGE:
        QMetaObject::invokeMethod(this, "sigShowImage", Qt::QueuedConnection,
                                                    Q_ARG(bool, args[0].GetBool()));
        break;

    case ProcessEngine::SIG_TEST_SKIPPED:
        QMetaObject::invokeMethod(this, "sigTestSkipped", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_TEST_ENDED:
        QMetaObject::invokeMethod(this, "sigTestEnded", Qt::QueuedConnection);
        break;

    case ProcessEngine::SIG_TEST_ERROR:
        QMetaObject::invokeMethod(this, "sigTestError", Qt::QueuedConnection);
        break;

    default:
        TLogWarning("Un-managed signal: " + std::to_string(signal));
        break;
    }

    QMetaObject::invokeMethod(this, "sigRunningChanged", Qt::QueuedConnection);
}

void QEngineModel::start()
{
    mEngine.Start();
}

QList<QObject *> QEngineModel::getTestList()
{
    BuildTestModel();
    return mModel;
}

QList<QObject *> QEngineModel::getComList()
{
    return mComModel;
}

QString QEngineModel::getTestTitle(unsigned int index)
{
    return mEngine.GetTestTitle(index).c_str();
}

void QEngineModel::selectAllTests(bool enable)
{
    mEngine.SelectAllTests(enable);
    BuildTestModel();
}

void QEngineModel::selectOneTest(unsigned int index, bool enable)
{
    mEngine.SelectOneTest(index, enable);
}

void QEngineModel::pause()
{
    mEngine.Pause();
}

void QEngineModel::resume()
{
    mEngine.Resume();
}

void QEngineModel::stop()
{
    mEngine.Stop();
}

void QEngineModel::autoTest()
{
    mEngine.AutoTest();
}

void QEngineModel::acceptInputText(const QString &text, bool accepted)
{
    mEngine.AcceptInputText(text.toStdString(), accepted);
}

QString QEngineModel::getImage()
{
    QString image("data:image/bmp;base64,");

    std::string image_data = mEngine.GetLabelImage();
    image.append(QString::fromLatin1(image_data.data()));
    return image;
}

QVariantList QEngineModel::getFiles()
{
    QVariantList variantlist;
    for (const QString &s : mFiles)
    {
        variantlist << s;
    }
    return variantlist;
}

void QEngineModel::BuildTestModel()
{
    mModel.clear();
    std::vector<Test> tests = mEngine.GetTests();
    for (const auto &test : tests)
    {
        // Also the data model for QML
        mModel.append(new DataObject(test.title.c_str(), test.enable));
    }
}

void QEngineModel::BuildComChannelModel()
{
    mComModel.clear();
    std::vector<std::string> comList = mEngine.GetConnList();
    for (const auto &com : comList)
    {
        // Also the data model for QML
        mComModel.append(new DataObject(com.c_str(), true));
    }
}
