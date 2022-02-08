# Minimum required version of CMake
# target link options with v3.13
cmake_minimum_required(VERSION 3.13)

# set CMake project name
project(VIEW)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()
include_directories(${GLFW_INCLUDE_DIRS})

include_directories(include)

add_executable( view
    src/view.cpp
    include/tclap/CmdLine.h
)

target_compile_options(view PUBLIC "$<$<CONFIG:Debug>:-pg>")
target_link_options(view PUBLIC "$<$<CONFIG:Debug>:-pg>")
target_link_libraries(view ${CONAN_LIBS})
# glm is header-only lib

