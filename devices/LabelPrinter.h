#ifndef LABEL_PRINTER_H
#define LABEL_PRINTER_H

#include <QString>
#include <QImage>
#include <QObject>
#include <functional>
#include "DeviceBase.h"

struct LabelItem
{
    enum LabelItemType { LBL_IMAGE, LBL_QRCODE, LBL_TEXT };

    std::string type;
    std::string value;
    std::string name;
    std::string param1;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;

    LabelItem()
        : x(0U)
        , y(0U)
        , width(100U)
        , height(100U)
    {

    }
};

struct LabelParameters
{
    int32_t width;
    int32_t height;
    int32_t dpi;
    int32_t rotate;
    bool inverted;
    bool show;
    bool defaultPrinter;
    QList<LabelItem> items;

    LabelParameters()
        : width(100U)
        , height(100U)
        , dpi(300U)
        , rotate(0)
        , inverted(false)
        , show(false)
        , defaultPrinter(true)
    {

    }
};

class LabelPrinter : public QObject, public DeviceBase
{
    Q_OBJECT

public:
    static std::string mWorkspacePath;

    std::function< void(bool show, const std::string &) > callback;

    LabelPrinter();
    virtual ~LabelPrinter();

    QImage GetPix() { return mPix; }
    Value StartPrint();
    LabelParameters GetLabelParameters() { return mLabel; }
    LabelItem GetItem(int index);
    void ParseLabelParameters(const std::string &param);
    void BuildLabel();

    // From IScriptEngine::IFunction
    bool Execute(const std::vector<Value> &args, Value &ret);

    // From DeviceBase
    void Initialize();
    void Stop();
    void AutoTest();

private:
    QImage mPix;
    QImage mPixPreview;
    LabelParameters mLabel;
};

#endif // LABEL_PRINTER_H
