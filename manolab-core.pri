
# ------------------------------------------------------------------------------
# GUI files
# ------------------------------------------------------------------------------
manolab_gui {

SOURCES += QEngineModel.cpp
HEADERS += QEngineModel.h
}

# ------------------------------------------------------------------------------
# Device files
# ------------------------------------------------------------------------------

SOURCES += \
    VoltcraftPPS.cpp \
    BK8500.cpp \
    DeviceBase.cpp \
    ProcessEngine.cpp \
    Delay1s.cpp \
    PrintLog.cpp \
    PrintReport.cpp \
    SerialDevice.cpp \
    LabelPrinter.cpp \
    SerialPort.cpp \
    Controllino.cpp \
    ManoLabServer.cpp \
    LonganCanModule.cpp \
    ExecuteCommand.cpp \
    ModbusMaster.cpp \
    InputText.cpp \
    MiniCircuitsPwrSen.cpp \
    AcuDC.cpp \
    SoundPlayer.cpp \
    ShowImage.cpp \
    Zebra7500.cpp

HEADERS += \
    $$PWD/Plugin.h \
    VoltcraftPPS.h \
    Registry.hpp \
    BK8500.h \
    DeviceBase.h \
    ProcessEngine.h \
    Delay1s.h \
    PrintLog.h \
    PrintReport.h \
    SerialDevice.h \
    LabelPrinter.h \
    SerialPort.h \
    Controllino.h \
    ManoLabServer.h \
    LonganCanModule.h \
    MiniCircuitsPwrSen.h \
    IProcessEngine.h \
    ExecuteCommand.h \
    ModbusMaster.h \
    AcuDC.h \
    InputText.h \
    IModbusMaster.h \
    SoundPlayer.h \
    ShowImage.h \
    Zebra7500.h

VPATH += $$MANOLAB_ROOT_DIR/devices
VPATH += $$MANOLAB_ROOT_DIR/jsext
INCLUDEPATH += $$MANOLAB_ROOT_DIR/devices
INCLUDEPATH += $$MANOLAB_ROOT_DIR/jsext

# ------------------------------------------------------------------------------
# ICL library
# ------------------------------------------------------------------------------
CONFIG += icl_zip icl_http
ICL_DIR = $$MANOLAB_ROOT_DIR/lib/icl
include($$ICL_DIR/icl.pri)

# ------------------------------------------------------------------------------
# Serial library
# ------------------------------------------------------------------------------
SOURCES += serial.c
HEADERS += serial.h

VPATH += $$MANOLAB_ROOT_DIR/lib/serial
INCLUDEPATH += $$MANOLAB_ROOT_DIR/lib/serial

# ------------------------------------------------------------------------------
# HID library
# ------------------------------------------------------------------------------
unix {
SOURCES += hid_linux.c
}
windows {
SOURCES += hid_win32.c
}
HEADERS += hidapi.h

VPATH += $$MANOLAB_ROOT_DIR/lib/libhid
INCLUDEPATH += $$MANOLAB_ROOT_DIR/lib/libhid


# ------------------------------------------------------------------------------
# Zebra7500 library
# ------------------------------------------------------------------------------
QMAKE_LIBDIR += $$MANOLAB_ROOT_DIR/lib/libzebrarfid
LIBS += -lrfidapi32 -lutils -lltk -lxml2
INCLUDEPATH += $$MANOLAB_ROOT_DIR/lib/libzebrarfid
VPATH += $$MANOLAB_ROOT_DIR/lib/libzebrarfid

DEFINES += UNICODE linux
SOURCES += Zebra7500Util.cpp
HEADERS += rfidapi.h Zebra7500Util.h

# ------------------------------------------------------------------------------
# USB library
# ------------------------------------------------------------------------------
#HEADERS += libusb.h
#VPATH += libusb
#INCLUDEPATH += libusb
#QMAKE_LIBDIR += libusb


#windows {
#    *-g++* {
#        LIBS += -L$$MANOLAB_ROOT_DIR/libusb -lusb-1.0
#    }
#}

# ------------------------------------------------------------------------------
# IXXAT CAN BUS library
# ------------------------------------------------------------------------------
windows {
    INCLUDEPATH += $$MANOLAB_ROOT_DIR/lib/ixxat/libixxat
    VPATH += $$MANOLAB_ROOT_DIR/lib/ixxat/libixxat
    LIBS += -L$$MANOLAB_ROOT_DIR/lib/ixxat/libixxat
    QMAKE_LIBS +=
    HEADERS += libixxat.h CanDevice.h
    SOURCES += CanDevice.cpp
}

# ------------------------------------------------------------------------------
# Modbus library
# ------------------------------------------------------------------------------
SOURCES += modbus.c
HEADERS += modbus.h

VPATH += $$MANOLAB_ROOT_DIR/lib/modbuslib
INCLUDEPATH += $$MANOLAB_ROOT_DIR/lib/modbuslib

# ------------------------------------------------------------------------------
# QR Code library
# ------------------------------------------------------------------------------

QR_LIB_PATH = $$MANOLAB_ROOT_DIR/lib/qrencode-4.0.0

VPATH += $$QR_LIB_PATH
INCLUDEPATH += $$QR_LIB_PATH

SOURCES += bitstream.c \
mask.c \
mmask.c \
mqrspec.c \
qrencode.c \
qrinput.c \
qrspec.c \
rsecc.c \
split.c

HEADERS += bitstream.h \
mask.h \
mmask.h \
mqrspec.h \
qrencode.h \
qrencode_inner.h \
qrinput.h \
qrspec.h \
rsecc.h \
split.h

DEFINES += STATIC_IN_RELEASE= MAJOR_VERSION=4 MINOR_VERSION=0 MICRO_VERSION=0 VERSION=\\\"4.0.0\\\"
