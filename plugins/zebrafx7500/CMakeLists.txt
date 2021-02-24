cmake_minimum_required(VERSION 3.14)

project(zebrafx7500 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The second parameter is output directory for the results of the targets from that subdirectory.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/icl build_icl)

add_library(zebrafx7500 SHARED
  Zebra7500.cpp
  Zebra7500Util.cpp
  Zebra7500.h
  Zebra7500Util.h
)

target_include_directories (
    zebrafx7500
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
)
#LIBS += -lrfidapi32 -lutils -lltk -lxml2

if(UNIX AND NOT APPLE)
    add_definitions(-Dlinux)
endif()

target_link_libraries(zebrafx7500 PRIVATE icl rfidapi32 utils ltk xml2)
target_link_directories(zebrafx7500 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_compile_definitions(zebrafx7500 PRIVATE ZEBRAFX7500_LIBRARY UNICODE)
