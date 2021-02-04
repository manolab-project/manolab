CONFIG  += c++17 # qtquickcompiler
CONFIG  += exceptions rtti
QT      += core serialport printsupport multimedia

TARGET = manolab-cli
TEMPLATE = app

BASE_DIR = $${PWD}

CONFIG(debug, debug|release) {
debug:      DESTDIR = $$BASE_DIR/build-manoab/debug
}

CONFIG(release, debug|release) {
release:    DESTDIR = $$BASE_DIR/build-manolab/release
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
        QMAKE_CXXFLAGS_CXX14 = -std=c++17
        QMAKE_CXXFLAGS_GNUCXX14 = -std=c++17

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
    QMAKE_RPATHDIR += $ORIGIN/plugins
    LIBS += -ldl -static-libgcc -static-libstdc++ -lpthread -ludev
}

#

## Force using C++ compiler even for .c files
#QMAKE_CC = $$QMAKE_CXX
#QMAKE_CFLAGS  = $$QMAKE_CXXFLAGS

# ------------------------------------------------------------------------------
# Main files
# ------------------------------------------------------------------------------

SOURCES += main-cli.cpp
HEADERS += main-cli.h
   
# ------------------------------------------------------------------------------
# Manolab library
# ------------------------------------------------------------------------------
MANOLAB_ROOT_DIR = $$BASE_DIR
include($$MANOLAB_ROOT_DIR/manolab-core.pri)

