cmake_minimum_required(VERSION 3.5)

project(manolab LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set( CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX AND NOT APPLE)
    add_definitions(-DUSE_UNIX_OS) 
    add_definitions(-DUSE_LINUX_OS) 
    add_definitions(-fPIC)
endif()

if (WIN32)
    add_definitions(-DUSE_WINDOWS_OS)
endif()

add_definitions(-DUNICODE)

set(MANOLAB_SRC

    ProcessEngine.cpp
    ProcessEngine.h

    IProcessEngine.h
    IPlugin.h

    PluginController.cpp
    PluginController.h
    Plugin.h

    Settings.cpp
    Settings.h

    Node.cpp
    Node.h

    # Extensions
    jsext/Environment.cpp
    jsext/Environment.h
    jsext/Delay1s.cpp
    jsext/Delay1s.h
    jsext/PrintLog.cpp
    jsext/PrintLog.h
    jsext/PrintReport.cpp
    jsext/PrintReport.h
    jsext/ExecuteCommand.cpp
    jsext/ExecuteCommand.h
    jsext/InputText.cpp
    jsext/InputText

    # Instrument devices
    devices/VoltcraftPPS.cpp
    devices/BK8500.cpp
    devices/DeviceBase.cpp
    devices/SerialDevice.cpp
    devices/Controllino.cpp
    devices/ManoLabServer.cpp
    devices/LonganCanModule.cpp
    devices/ModbusMaster.cpp
    devices/MiniCircuitsPwrSen.cpp
    devices/AcuDC.cpp

    # Third party libraries
    lib/serial/SerialPort.cpp
    lib/serial/serial.c
    lib/modbuslib/modbus.c
)

if (WIN32)
    list(APPEND MANOLAB_SRC lib/libhid/hid_win32.c)

elseif (UNIX)
    list(APPEND MANOLAB_SRC lib/libhid/hid_linux.c)
endif()

add_library(
    manolab
    STATIC
    ${MANOLAB_SRC}

    
)

target_include_directories (
    manolab
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/devices
    ${CMAKE_CURRENT_SOURCE_DIR}/jsext
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/modbuslib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/serial
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhid
)
