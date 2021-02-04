cmake_minimum_required(VERSION 3.5)

project(glfw3 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_link_libraries(glfw3 libglfw3.a libglfw3dll.a) 

target_include_directories(
    glfw3
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)
