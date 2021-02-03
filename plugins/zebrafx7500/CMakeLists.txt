cmake_minimum_required(VERSION 3.14)

project(zebrafx7500 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

# add_subdirectory(icl build_icl)

add_library(zebrafx7500 SHARED
  zebrafx7500_global.h
  zebra7500.cpp
  zebra7500.h
)

target_include_directories (
    zebrafx7500
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
)


target_link_libraries(zebrafx7500 PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(zebrafx7500 PRIVATE ZEBRAFX7500_LIBRARY)
