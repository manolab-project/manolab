#include <QPrinter>
#include <QPainter>
#include <iostream>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <QFileInfo>
#include <QApplication>
#include <QDir>

#include "SoundPlayer.h"
#include "Util.h"
#include "Log.h"

std::string SoundPlayer::mWorkspacePath;

SoundPlayer::SoundPlayer()
{

}


SoundPlayer::~SoundPlayer()
{

}


bool SoundPlayer::Play(const QString &soundFile, qreal volume, int loopCount)
{    
    bool success = false;

    QFileInfo fileInfo(soundFile);

    // Absolute file not found, try to open relative to workspace path
    if (!fileInfo.exists())
    {
        QString path = mWorkspacePath.c_str();
        path += QDir::separator() + QString("sounds") + QDir::separator() + soundFile;

        fileInfo.setFile(path);
    }

    if (fileInfo.exists())
    {
        mEffect.setSource(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
        mEffect.setLoopCount(loopCount);
        mEffect.setVolume(volume);
        mEffect.play();

        success = true;
    }
    else
    {
        TLogError("Cannot find sound file: " + fileInfo.absoluteFilePath().toStdString());
    }

    return success;
}


bool SoundPlayer::Execute(const std::vector<Value> &args, Value &ret)
{
    bool retOk = true;
    (void) ret;

    if (args.size() >= 1)
    {
        if (args[0].GetType() == Value::STRING)
        {
            int loopCount = 1;
            qreal volume = 1.0;
            if (args.size() >= 2)
            {
                volume = args[1].GetDouble();
            }

            if (volume > 1.0)
            {
                volume = 1.0;
            }

            if (args.size() >= 3)
            {
                loopCount = args[2].GetInteger();

                if (loopCount < 0)
                {
                    loopCount = QSoundEffect::Infinite;
                }
            }

            if (loopCount == 0)
            {
                mEffect.stop();
            }
            else
            {
                retOk = Play(args[0].GetString().c_str(), volume, loopCount);
            }
        }
    }
    return retOk;
}

