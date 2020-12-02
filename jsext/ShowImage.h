#ifndef SHOW_IMAGE_H
#define SHOW_IMAGE_H

#include <QString>
#include <QImage>
#include <QObject>
#include <functional>
#include <QSoundEffect>
#include "DeviceBase.h"

class ShowImage : public QObject, public IScriptEngine::IFunction
{
    Q_OBJECT

public:
    static std::string mWorkspacePath;

    ShowImage();
    virtual ~ShowImage();

    std::function< void(bool show, const std::string &) > callback;

    // From IScriptEngine::IFunction
    bool Execute(const std::vector<Value> &args, Value &ret);

private:
    bool showIt(bool enable, const std::string &fileName);
};

#endif // LABEL_PRINTER_H
