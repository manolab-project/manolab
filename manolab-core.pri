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
    AcuDC.cpp \
    SoundPlayer.cpp \
    ShowImage.cpp

HEADERS += \
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
    IProcessEngine.h \
    ExecuteCommand.h \
    ModbusMaster.h \
    AcuDC.h \
    InputText.h \
    IModbusMaster.h \
    SoundPlayer.h \
    ShowImage.h

VPATH += devices jsext
INCLUDEPATH += devices jsext

# ------------------------------------------------------------------------------
# ICL library
# ------------------------------------------------------------------------------
CONFIG += icl_zip icl_http
ICL_DIR = $$BASE_DIR/lib/icl
include($$ICL_DIR/icl.pri)

# ------------------------------------------------------------------------------
# Serial library
# ------------------------------------------------------------------------------
SOURCES += serial.c
HEADERS += serial.h

VPATH += lib/serial
INCLUDEPATH += lib/serial

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
# IXXAT CAN BUS library
# ------------------------------------------------------------------------------
windows {
    INCLUDEPATH += $$BASE_DIR/lib/ixxat/libixxat
    VPATH += $$BASE_DIR/lib/ixxat/libixxat
    LIBS += -L$$BASE_DIR/lib/ixxat/libixxat
    QMAKE_LIBS +=
    HEADERS += libixxat.h
}

# ------------------------------------------------------------------------------
# Modbus library
# ------------------------------------------------------------------------------
SOURCES += modbus.c
HEADERS += modbus.h

VPATH += lib/modbuslib
INCLUDEPATH += lib/modbuslib

# ------------------------------------------------------------------------------
# QR Code library
# ------------------------------------------------------------------------------

QR_LIB_PATH = $$BASE_DIR/lib/qrencode-4.0.0

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
