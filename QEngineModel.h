#ifndef Q_ENGINE_MODEL_H
#define Q_ENGINE_MODEL_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QVariant>
#include "ProcessEngine.h"

class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(bool active READ isActive)

public:
    DataObject(const QString &title, bool active) {
        mTitle = title;
        mActive = active;
    }
    QString title() { return mTitle; }
    bool isActive() { return mActive; }

private:
    QString mTitle;
    bool mActive;
};

class QEngineModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle NOTIFY sigScriptLoaded)
    Q_PROPERTY(bool running READ isRunning NOTIFY sigRunningChanged)
    Q_PROPERTY(bool loaded READ isLoaded NOTIFY sigScriptLoaded)
    Q_PROPERTY(QVariantList files READ getFiles NOTIFY sigFilesChanged)
    Q_PROPERTY(bool admin READ isAdmin NOTIFY sigScriptLoaded)
    Q_PROPERTY(QString workspace READ getWorkspace CONSTANT)

public:
    QEngineModel();

    // Fonctions exportées dans QML, doivent commencer par une minuscule ...
    Q_INVOKABLE void loadScript(const QString &filename);
    Q_INVOKABLE void start();
    Q_INVOKABLE  QList<QObject*> getTestList();
    Q_INVOKABLE  QList<QObject*> getComList();
    Q_INVOKABLE QString getTestTitle(unsigned int index);
    Q_INVOKABLE void selectAllTests(bool enable);
    Q_INVOKABLE void selectOneTest(unsigned int index, bool enable);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void autoTest();
    Q_INVOKABLE void acceptInputText(const QString &text, bool accepted);
    Q_INVOKABLE QString getImage();

    QVariantList getFiles();
    QString getTitle() const;
    QString getWorkspace() const;
    bool isRunning();
    bool isAdmin() const;
    bool isLoaded() const;

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

signals:
    void sigMessage(const QString &msg);
    void sigDelay(int value);
    void sigTest(int number, int steps);
    void sigStep(const QString &title, bool isStart);
    void sigFinished();
    void sigFilesChanged();
    void sigScriptLoaded();
    void sigInputText(const QString &text, bool showControls);
    void sigAutoTestFinished();
    void sigShowImage(bool show);
    void sigTestSkipped();
    void sigTestEnded();
    void sigTestError();
    void sigRunningChanged();

private slots:
    void slotDirectoryChanged(const QString &path);
private:
    QList<QObject*> mModel;
    QList<QObject*> mComModel;
    QFileSystemWatcher mWatcher;
    ProcessEngine mEngine;
    QStringList mFiles;
    QString mFilesPath;

    void EngineEvents(int signal, const std::vector<Value> &args);
    void BuildTestModel();
    void BuildComChannelModel();
    void WriteSettings();
    void ReadSettings();
};

#endif // Q_ENGINE_MODEL_H
