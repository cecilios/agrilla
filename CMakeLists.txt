#---------------------------------------------------------------------------------------
#  This file is part of LenMus AGrilla program
#  Copyright (c) 2025 AGRilla project
#
#  This program is free software; you can redistribute it and/or modify it under the
#  terms of the MIT License.
#---------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------
# This is a CMake configuration file for building makefiles and installfiles for
# the AGrilla program
#
# To use it you need CMake which can be downloaded from http://www.cmake.org/
#
# Usage
#   cmake [<options>] <source-tree>
#
# example:
#   mk my-build && cd my-build
#   cmake ..
#
# The default target (in Linux) is "Unix Makefiles". If you would like to generate
# a diferent makefile use option -G<desired-target>. For instance:
#   cmake -G "Ninja" ..
#
#
# Default configuration
# ~~~~~~~~~~~~~~~~~~~~~~~
# The default build type is 'Release'. You can choose the "Debug" build by
# specifying option '-DCMAKE_BUILD_TYPE=Debug'. Example:
#   cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug ..
#
#
## Installation folders (Not yet)
## ~~~~~~~~~~~~~~~~~~~~~
## The default installation prefix is "/usr/local" for Linux. 
## specifying option '-DCMAKE_INSTALL_PREFIX=/new/install/prefix'. Example:
##   cmake -DCMAKE_INSTALL_PREFIX=~/programs ..
#
#-------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
project(agrilla VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define possible builds
set(CMAKE_CONFIGURATION_TYPES Debug Release)

#Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Define variables needed for config.h
set(AGRILLA_RES_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/agrilla/res/")
set(AGRILLA_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}/" )    #AGRILLA_INSTALL_ROOT is needed for config.h
set(AGRILLA_BUILD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/" )

#set shared folder
if(UNIX AND NOT APPLE)
    set(SHARED_DIR ${AGRILLA_INSTALL_ROOT}share/${CMAKE_PROJECT_NAME})
elseif(APPLE)
    set(SHARED_DIR ${AGRILLA_INSTALL_ROOT}Resources )
elseif(WIN32)
    set(SHARED_DIR ${AGRILLA_INSTALL_ROOT} )
endif()

message(STATUS "SHARED_DIR = ${SHARED_DIR}" )
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )


# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Generate the header file with the resources installation path
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_BINARY_DIR}/config.h"
)

# Define include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${CMAKE_BINARY_DIR}"
)

# Source files
set(SOURCE_FILES
    src/app/MainFrame.cpp
    src/app/TheApp.cpp
    src/app/ToolBar.cpp
    src/dialogs/DlgAspectRatio.cpp
    src/dialogs/DlgGridOptions.cpp
)


# Add resources for installation
set(RESOURCE_FILES
    res/grid-on.svg
    res/grid-off.svg
    res/golden-lines-on.svg
    res/golden-lines-off.svg
    res/aspect-ratio-locked.svg
    res/aspect-ratio-unlocked.svg
    res/options.svg
    res/shutdown.svg
    res/set-aspect-ratio.svg
    #res/handlers-on.svg
    #res/handlers-off.svg
)

# Add the executable
add_executable(agrilla ${SOURCE_FILES})

# Link with wxWidgets libraries
target_link_libraries(agrilla ${wxWidgets_LIBRARIES} )

# Define Debug settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(agrilla PUBLIC "DEBUG")    #define DEBUG macro
  target_compile_options(agrilla PUBLIC "-g")    #include debug symbols
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")    #no optimization
endif()

# Define Release settings
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_definitions(agrilla PUBLIC "NDEBUG")    #define NDEBUG to disable asserts
  target_compile_definitions(agrilla PUBLIC "wxDEBUG_LEVEL=0") #disable wxWidgets debug
  target_compile_options(agrilla PUBLIC "-O3")    #maximum optimización
endif()

# Installation rules
install(TARGETS agrilla DESTINATION bin)
install(FILES ${RESOURCE_FILES} DESTINATION share/agrilla/res)
