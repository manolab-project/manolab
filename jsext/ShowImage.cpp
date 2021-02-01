#include <QPrinter>
#include <QPainter>
#include <iostream>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <QFileInfo>
#include <QApplication>
#include <QDir>

#include "ShowImage.h"
#include "Util.h"
#include "Base64Util.h"
#include "Log.h"

std::string ShowImage::mWorkspacePath;

ShowImage::ShowImage()
{

}


ShowImage::~ShowImage()
{

}

bool ShowImage::showIt(bool enable, const std::string &fileName)
{
    bool success = true;
    std::string base64_str;

    if (fileName.size() > 0)
    {
        QFileInfo fileInfo(fileName.c_str());

        // Absolute file not found, try to open relative to workspace path
        if (!fileInfo.exists())
        {
            QString path = mWorkspacePath.c_str();
            path += QDir::separator()  + QString("images") + QDir::separator() + QString(fileName.c_str());

            fileInfo.setFile(path);
        }

        if (fileInfo.exists())
        {
            std::string buffer;
            std::ifstream input(fileInfo.absoluteFilePath().toStdString(), std::ios::in | std::ios::binary );
            input.seekg(0, std::ios::end);
            buffer.reserve(input.tellg());
            input.seekg(0, std::ios::beg);

            // Copy all data into a buffer
            // note the extra parentheses around the first argument to the string constructor.
            // These are essential. They prevent the problem known as the "most vexing parse",
            // which in this case won't actually give you a compile error like it usually does,
            // but will give you interesting (read: wrong) results.
            buffer.assign((std::istreambuf_iterator<char>(input)),
                          (std::istreambuf_iterator<char>()));
            input.close();

            // Build a simple JSON object as a string so that the script can manipulate it
            base64_str = Base64::Encode(buffer);
        }
        else
        {
            TLogError("Image file does not exists: " + fileInfo.absoluteFilePath().toStdString());
            success = false;
        }
    }

    callback(enable, base64_str);
    return success;
}

bool ShowImage::Execute(const std::vector<Value> &args, Value &ret)
{
    bool retOk = true;
    (void) ret;

    if (args.size() >= 1)
    {
        bool enable = false;
        std::string filePath;
        if (args[0].GetType() == Value::BOOLEAN)
        {
            enable = args[0].GetBool();
        }

        if (args.size() >= 2)
        {
            if (args[1].GetType() == Value::STRING)
            {
                filePath = args[1].GetString();
            }
        }

        retOk = showIt(enable, filePath);
    }
    return retOk;
}

