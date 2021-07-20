# Minimum required version of CMake
# target link options with v3.13
cmake_minimum_required(VERSION 3.13)

# set CMake project name
project(VIEW)

set(default_build_type "RelWithDebInfo")

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GFLW_INCLUDE_DIRS})


include_directories(include)

add_executable( view
    src/view.cpp
    include/tclap/CmdLine.h
)

target_compile_options(view PUBLIC "$<$<CONFIG:Debug>:-pg>")
target_link_options(view PUBLIC "$<$<CONFIG:Debug>:-pg>")
target_link_libraries(view ${GLFW_LIBRARIES})
target_link_libraries(view ${GLEW_LIBRARIES})
# glm is header-only lib



