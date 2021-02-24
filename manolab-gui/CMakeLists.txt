cmake_minimum_required(VERSION 3.5)

project(manolab-gui LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE on )

#set(GLFW_LIBRARY glfw)

#if (WIN32)
#    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-mingw-w64)
#    include_directories(glfw/include)
#    set(GLFW_LIBRARY glfw3)
#endif (WIN32)

# The second parameter is output directory for the results of the targets from that subdirectory.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ build_manolab)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/icl build_icl)

find_package(OpenGL REQUIRED)

if(UNIX AND NOT APPLE)
    add_definitions(-DUSE_UNIX_OS)
endif()

if(WIN32)
    add_definitions(-DUSE_WINDOWS_OS)
endif()

add_executable(manolab-gui
    gl3w/GL/gl3w.c
    main.cpp
    ConsoleWindow.cpp
    ConsoleWindow.h
    Gui.h
    Gui.cpp
    CodeEditor.cpp
    CodeEditor.h
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_sdl.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    ImGuiColorTextEdit/TextEditor.cpp
    ImGuiColorTextEdit/TextEditor.h
)

# add_definitions(-DUSE_UNIX_OS)
add_link_options(-static-libgcc -static-libstdc++ -lpthread)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)
add_definitions(-Dcimg_display=0)

target_link_libraries(manolab-gui
    manolab
    icl
    dl
    SDL2
    glfw
    pthread
    udev
    OpenGL::GL)

target_include_directories(
    manolab-gui
    PUBLIC
    /usr/include/SDL2
    ${CMAKE_CURRENT_SOURCE_DIR}/gl3w
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiColorTextEdit
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)



