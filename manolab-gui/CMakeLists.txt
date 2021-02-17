cmake_minimum_required(VERSION 3.5)

project(manolab-gui LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(GLFW_LIBRARY glfw)

#if (WIN32)
#    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-mingw-w64)
#    include_directories(glfw/include)
#    set(GLFW_LIBRARY glfw3)
#endif (WIN32)

find_package(OpenGL REQUIRED)

add_executable(manolab-gui
    gl3w/GL/gl3w.c
    main.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_sdl.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# add_definitions(-DUSE_UNIX_OS)
add_link_options(-static-libgcc -static-libstdc++ -lpthread)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

target_link_libraries(manolab-gui
    dl
    SDL2
    glfw
    OpenGL::GL)

target_include_directories(
    manolab-gui
    PUBLIC
    /usr/include/SDL2
    ${CMAKE_CURRENT_SOURCE_DIR}/gl3w
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)



