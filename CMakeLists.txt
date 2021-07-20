# Minimum required version of CMake
# target link options with v3.13
cmake_minimum_required(VERSION 3.13)

# set CMake project name
project(VIEW)

set(default_build_type "RelWithDebInfo")

find_package(OpenGL REQUIRED)

include_directories(include)

add_executable( view
    src/view.cpp
    include/tclap/CmdLine.h
)

target_compile_options(view PUBLIC "$<$<CONFIG:Debug>:-pg>")
target_link_options(view PUBLIC "$<$<CONFIG:Debug>:-pg>")



