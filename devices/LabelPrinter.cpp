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
#include <QDir>

#include "LabelPrinter.h"
#include "qrencode.h"
#include "JsonValue.h"
#include "JsonReader.h"
#include "modbus.h"
#include "Util.h"

std::string LabelPrinter::mWorkspacePath;

LabelPrinter::LabelPrinter()
    : DeviceBase ("LabelPrinter")
    , mPix(100, 100, QImage::Format_Grayscale8)
{
    mPix.fill(Qt::white);

}

#include "Base64Util.h"

Value LabelPrinter::StartPrint()
{
    // Always save the image to the filesystem
    mPixPreview.save("output.png");

    std::ifstream input("output.png", std::ios::in | std::ios::binary );

    std::string buffer;

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
    std::stringstream ss;
    std::string base64_str = Base64::Encode(buffer);

    ss.clear();
    ss << R"({ "buffer": ")" << base64_str << R"(", "size": )";
    ss << buffer.size() << R"( })";

    std::ofstream out("output.txt");
    out << ss.str();
    out.close();

    Value retVal(ss.str());
    retVal.SetJsonString(true);

    callback(mLabel.show, base64_str);

    if (mLabel.defaultPrinter)
    {
        QPrinter printer;
        QPainter painter;

        printer.setResolution(mLabel.dpi);
        painter.begin(&printer);
        painter.drawImage(QRect(0, 0, mLabel.width, mLabel.height), mPix);
        painter.end();

        QPrinter::PrinterState state = printer.printerState();
        std::cout << "Printer state: " << static_cast<int>(state) <<std::endl;
        switch(state)
        {
            case QPrinter::Error:
                std::cout << "Printer error!" <<std::endl;
            break;
            default:
                break;
        }
    }

    return retVal;
}


LabelItem LabelPrinter::GetItem(int index)
{
    LabelItem item;
    if (index < mLabel.items.size())
    {
        item = mLabel.items.at(index);
    }
    return item;
}


bool LabelPrinter::Execute(const std::vector<Value> &args, Value &ret)
{
    bool retOk = true;
    if (args.size() >= 1)
    {
        if (args[0].GetType() == Value::STRING)
        {
            ParseLabelParameters(args[0].GetString());
            BuildLabel();
            ret = StartPrint();
        }
    }
    return retOk;
}

void LabelPrinter::Initialize()
{

}

void LabelPrinter::Stop()
{

}

void LabelPrinter::AutoTest()
{

}


void LabelPrinter::ParseLabelParameters(const std::string &param)
{
    JsonReader reader;
    JsonValue json;
    bool status = reader.ParseString(json, param);

    if (status)
    {
        mLabel.items.clear();
        mLabel.width = json.FindValue("width").GetInteger();
        mLabel.height = json.FindValue("height").GetInteger();
        mLabel.dpi = json.FindValue("dpi").GetInteger();
        mLabel.rotate = json.FindValue("rotate").GetInteger();
        mLabel.show = json.FindValue("show").GetBool();
        mLabel.inverted = json.FindValue("inverted").GetBool();
        std::string defaultPrinter = json.FindValue("printer").GetString();
        if (defaultPrinter == "default")
        {
            mLabel.defaultPrinter = true;
        }
        else
        {
            mLabel.defaultPrinter = false;
        }

        JsonValue layers = json.FindValue("layers");
        if (layers.IsArray())
        {
            for (uint32_t i = 0U; i < layers.GetArray().Size(); i++)
            {
                JsonValue layer = layers.GetArray().GetEntry(i);
                if (layer.IsObject())
                {
                    LabelItem item;
                    item.height = layer.GetObj().GetValue("height").GetInteger();
                    item.width = layer.GetObj().GetValue("width").GetInteger();
                    item.x = layer.GetObj().GetValue("x").GetInteger();
                    item.y = layer.GetObj().GetValue("y").GetInteger();
                    item.param1 = layer.GetObj().GetValue("param1").GetString();
                    item.name = layer.GetObj().GetValue("name").GetString();
                    item.type = layer.GetObj().GetValue("type").GetString();
                    item.value = layer.GetObj().GetValue("value").GetString();
                    mLabel.items.push_back(item);
                }

            }
        }
    }
}


void LabelPrinter::BuildLabel()
{
    QImage image(mLabel.width, mLabel.height, QImage::Format_Grayscale8);
    image.fill(Qt::white);
    QPainter painter;

    painter.begin(&image);

    for (auto &item : mLabel.items)
    {
        if (item.type == "image")
        {
            QFileInfo fileInfo(item.value.c_str());

            // Absolute file not found, try to open relative to workspace path
            if (!fileInfo.exists())
            {
                QString path = mWorkspacePath.c_str();
                path += QDir::separator() + QString(item.value.c_str());

                fileInfo.setFile(path);
            }


            if (fileInfo.exists())
            {
                painter.drawImage(QRect(item.x, item.y, item.width, item.height), QImage(fileInfo.absoluteFilePath()));
            }
            else
            {
                std::cout << "Cannot find item image" << std::endl;
            }
        }
        else if (item.type == "qrcode")
        {
            // Generate QR Code
            const char *qrText = item.value.c_str();

            QRecLevel ec = QR_ECLEVEL_L;

            if (item.param1 == "H")
            {
                ec = QR_ECLEVEL_H;
            }
            else if (item.param1 == "M")
            {
                ec = QR_ECLEVEL_M;
            }
            else if (item.param1 == "Q")
            {
                ec = QR_ECLEVEL_Q;
            }
            else
            {
                ec = QR_ECLEVEL_L;
            }

            QRcode *code = QRcode_encodeString8bit(qrText, 0, ec);

            QImage qrImage(code->width, code->width, QImage::Format_Grayscale8);
            qrImage.fill(Qt::white);
            QPainter qrPainter;

            qrPainter.begin(&qrImage);
            int x = 0;
            int y = 0;
            for (int i = 0; i < (code->width*code->width); i++)
            {
                if ((code->data[i] & 0x01) == 0x01)
                {
                    qrPainter.drawPoint(x, y);
                }
                x++;
                if (x >= code->width)
                {
                    x = 0;
                    y++;
                }
            }
            qrPainter.end();

            QRcode_free(code);
            painter.drawImage(QRect(item.x, item.y, item.width, item.height), qrImage);
        }
        else if (item.type == "text")
        {
            QStringList params = QString(item.param1.c_str()).split(',');

            int alignment = Qt::AlignCenter;
            if (params.size() >= 7)
            {
                QFont font;
                QString fontName = params.at(0);
                QString fontPixelSize = params.at(1);
                QString fontWeight = params.at(2);
                QString fontItalic = params.at(3);
                QString fontUnderline = params.at(4);
                QString fontStrikeout = params.at(5);
                QString fontAlignment = params.at(6);

                font.setFamily(fontName);
                font.setPixelSize(fontPixelSize.toInt());
                font.setWeight(fontWeight.toInt());
                if (fontItalic.toInt() == 1)
                {
                    font.setItalic(true);
                }
                if (fontUnderline.toInt() == 1)
                {
                    font.setUnderline(true);
                }
                if (fontStrikeout.toInt() == 1)
                {
                    font.setStrikeOut(true);
                }

                if (fontAlignment.toInt() == 1)
                {
                    alignment = Qt::AlignLeft;
                }

                if (fontAlignment.toInt() == 2)
                {
                    alignment = Qt::AlignRight;
                }

                painter.setFont(font);
            }
            painter.drawText(QRect(item.x, item.y, item.width, item.height), alignment, item.value.c_str());
        }
    }

    painter.end();

    // save image in memory
    mPix = image;
    mPixPreview = image;

    // Draw a border around the image, only for the preview
    painter.begin(&mPixPreview);
    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(0, 0, mLabel.width-1, mLabel.height-1);
    painter.end();

    if (mLabel.inverted)
    {
        mPix.invertPixels();
        mPixPreview.invertPixels();
    }

    // Apply rotation
    QMatrix rm;
    rm.rotate(mLabel.rotate);
    mPix = mPix.transformed(rm);
    mPixPreview = mPixPreview.transformed(rm);
}


LabelPrinter::~LabelPrinter()
{

}
