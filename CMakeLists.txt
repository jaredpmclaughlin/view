# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# set CMake project name
project(VIEW)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(OpenGL REQUIRED)

add_executable( view
    src/view.cpp
)
