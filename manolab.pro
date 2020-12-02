CONFIG  += c++14  # qtquickcompiler
CONFIG  += exceptions rtti
QT      += core gui serialport printsupport quickwidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NovProd
TEMPLATE = app

BASE_DIR = $${PWD}

CONFIG(debug, debug|release) {
debug:      DESTDIR = $$BASE_DIR/build-novprod/debug
}

CONFIG(release, debug|release) {
release:    DESTDIR = $$BASE_DIR/build-novprod/release
}

# ------------------------------------------------------------------------------
# Where to find header files
# ------------------------------------------------------------------------------

UI_DIR          = $$DESTDIR/ui
UI_HEADERS_DIR  = $$DESTDIR/include
UI_SOURCES_DIR  = $$DESTDIR/src
OBJECTS_DIR     = $$DESTDIR/obj
RCC_DIR         = $$DESTDIR/rcc
MOC_DIR         = $$DESTDIR/moc


# ------------------------------------------------------------------------------
# Compiler definitions
# ------------------------------------------------------------------------------

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


windows {
    DEFINES += USE_WINDOWS_OS
    RC_FILE = icon.rc

    *-g++* {
        # MinGW
        QMAKE_CXXFLAGS_CXX14 = -std=c++14
        QMAKE_CXXFLAGS_GNUCXX14 = -std=c++14

        CXXFLAGS += -fexceptions

        # Let's make everything's static so that we don't need any DLL
        QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lpthread
        LIBS +=  libws2_32 -lpsapi libcfgmgr32 libsetupapi -lhid -lole32 -loleaut32 -luuid libWinmm
    }
    *-msvc* {
        # MSVC
        QMAKE_LIBS += ws2_32.lib psapi.lib setupapi.lib cfgmgr32.lib advapi32.lib Dbghelp.lib
    }
}


unix {
    DEFINES += USE_UNIX_OS
    LIBS += -ldl
}

## Force using C++ compiler even for .c files
#QMAKE_CC = $$QMAKE_CXX
#QMAKE_CFLAGS  = $$QMAKE_CXXFLAGS

# ------------------------------------------------------------------------------
# Main files
# ------------------------------------------------------------------------------

SOURCES += \
        main.cpp \
    VoltcraftPPS.cpp \
    BK8500.cpp \
    DeviceBase.cpp \
    QEngineModel.cpp \
    ProcessEngine.cpp \
    Delay1s.cpp \
    PrintLog.cpp \
    PrintReport.cpp \
    SerialDevice.cpp \
    LabelPrinter.cpp \
    SerialPort.cpp \
    Controllino.cpp \
    ManoLabServer.cpp \
    CanDevice.cpp \
    LonganCanModule.cpp \
    ExecuteCommand.cpp \
    ModbusMaster.cpp \
    InputText.cpp \
    AcuDC.cpp \
    SoundPlayer.cpp \
    ShowImage.cpp

HEADERS += \
    VoltcraftPPS.h \
    Registry.hpp \
    BK8500.h \
    DeviceBase.h \
    QEngineModel.h \
    ProcessEngine.h \
    Delay1s.h \
    PrintLog.h \
    PrintReport.h \
    SerialDevice.h \
    LabelPrinter.h \
    SerialPort.h \
    CanDevice.h \
    Controllino.h \
    ManoLabServer.h \
    libixxat.h \
    LonganCanModule.h \
    IProcessEngine.h \
    ExecuteCommand.h \
    ModbusMaster.h \
    AcuDC.h \
    InputText.h \
    IModbusMaster.h \
    SoundPlayer.h \
    ShowImage.h


OTHER_FILES = $$BASE_DIR/README.md

VPATH += devices jsext
INCLUDEPATH += devices jsext

RESOURCES += qml.qrc

# ------------------------------------------------------------------------------
# ICL library
# ------------------------------------------------------------------------------
ICL_DIR = $$BASE_DIR/icl
include($$ICL_DIR/icl.pri)

# ------------------------------------------------------------------------------
# Serial library
# ------------------------------------------------------------------------------
SOURCES += serial.c
HEADERS += serial.h

VPATH += serial
INCLUDEPATH += serial

# ------------------------------------------------------------------------------
# USB library
# ------------------------------------------------------------------------------
#HEADERS += libusb.h
#VPATH += libusb
#INCLUDEPATH += libusb
#QMAKE_LIBDIR += libusb

#windows {
#    *-g++* {
#        LIBS += -L$$BASE_DIR/libusb -lusb-1.0
#    }
#}

# ------------------------------------------------------------------------------
# CAN BUS library
# ------------------------------------------------------------------------------
INCLUDEPATH += $$BASE_DIR/ixxat/libixxat
VPATH += $$BASE_DIR/ixxat/libixxat
LIBS += -L$$BASE_DIR/ixxat/libixxat
QMAKE_LIBS +=

# ------------------------------------------------------------------------------
# Modbus library
# ------------------------------------------------------------------------------
SOURCES += modbus.c
HEADERS += modbus.h

VPATH += modbuslib
INCLUDEPATH += modbuslib

# ------------------------------------------------------------------------------
# QR Code library
# ------------------------------------------------------------------------------

QR_LIB_PATH = $$BASE_DIR/qrencode-4.0.0

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
