#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include <QString>
#include <QImage>
#include <QObject>
#include <functional>
#include <QSoundEffect>
#include "DeviceBase.h"

class SoundPlayer : public QObject, public IScriptEngine::IFunction
{
    Q_OBJECT

public:
    SoundPlayer();
    virtual ~SoundPlayer();

    static std::string mWorkspacePath;

    // From IScriptEngine::IFunction
    bool Execute(const std::vector<Value> &args, Value &ret);

private slots:

private:
    QSoundEffect mEffect;

    bool Play(const QString &soundFile, qreal volume, int loopCount);
};

#endif // LABEL_PRINTER_H
