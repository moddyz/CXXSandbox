cmake_minimum_required(VERSION 3.12)

# Project Name
project(
    CXXSandbox
    VERSION 0.0
    DESCRIPTION "Sandbox for experimenting with C++ language and commonly used libraries."
    LANGUAGES CXX
)

# Set module search paths.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tools)
# Apply options.
include(Options)

# Tools
include(Tools)

# Apply project defaults.
include(Defaults)

# Discover dependencies.
include(Packages)

# Add targets.
add_subdirectory(thirdparty)
add_subdirectory(src)

# Install exported targets.
# export_project(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in)
