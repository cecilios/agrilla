#---------------------------------------------------------------------------------------
#  This file is part of the AGrilla program
#  Copyright (c) 2025-present AGRilla developers
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
## The default installation prefix is "/usr/local" for Linux. ## specifying option '-DCMAKE_INSTALL_PREFIX=/new/install/prefix'. Example:
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
set(AGRILLA_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/" )

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
#find_package(wxWidgets REQUIRED COMPONENTS core base html)
#
## Enable static linking explicitly for Windows
#if(WIN32)
#    set(wxWidgets_USE_STATIC_LIBS ON)
#    find_package(wxWidgets REQUIRED COMPONENTS core base html)
#endif()
# Find wxWidgets
if(WIN32)
    # On Windows, to generate the MSI instaler, we build wxWidgets locally to
    # link statically and avoid problem. So we suse its CMake config files
    find_package(wxWidgets CONFIG REQUIRED)

    target_link_libraries(agrilla PRIVATE
        wx::core
        wx::base
        wx::html
    )
else()
    # On Linux/macOS, use system-installed wxWidgets via the FindwxWidgets.cmake module
    find_package(wxWidgets REQUIRED COMPONENTS core base html)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(agrilla PRIVATE ${wxWidgets_LIBRARIES})
endif()

# Generate the header file with the resources installation path
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_BINARY_DIR}/config.h"
)

# generate the header file with the version information
include( ${CMAKE_CURRENT_SOURCE_DIR}/build-version.cmake )

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
    src/dialogs/DlgAbout.cpp
    src/dialogs/DlgAspectRatio.cpp
    src/dialogs/DlgGridOptions.cpp
)

# Add resources for installation
set(RESOURCE_FILES
    res/about.svg
    res/aspect-ratio-locked.svg
    res/aspect-ratio-unlocked.svg
    res/frame-off.svg
    res/frame-on.svg
    res/golden-lines-off.svg
    res/golden-lines-on.svg
    res/grid-off.svg
    res/grid-on.svg
    #res/handlers-on.svg
    #res/handlers-off.svg
    res/options.svg
    res/set-aspect-ratio.svg
    res/shutdown.svg
)

# Add the executable
add_executable(agrilla ${SOURCE_FILES})

# Link with wxWidgets libraries
target_link_libraries(agrilla PRIVATE ${wxWidgets_LIBRARIES})

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

if (UNIX AND NOT APPLE)
    # Install desktop entry, icon and AppStream upstream metadata
    install(FILES res/agrilla.desktop DESTINATION share/applications)
    install(FILES res/agrilla-logo.svg DESTINATION share/icons/hicolor/scalable/apps/)
    install(FILES res/com.github.cecilios.agrilla.appdata.xml DESTINATION share/metainfo)
endif(UNIX AND NOT APPLE)


#///////////////////////////////////////////////////////////////////////////////
# CPack section: installers generation
#///////////////////////////////////////////////////////////////////////////////

# Common settings for all installers
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "Cecilio Salmeron <s.cecilio@gmail.com>")
set(CPACK_PACKAGE_VENDOR "Cecilio Salmeron")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/cecilios/agrilla")
set(CPACK_PACKAGE_MAINTAINER "Cecilio Salmeron <s.cecilio@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "A tool to overlay a grid over any screen region")
set(CPACK_PACKAGE_DESCRIPTION
"AGrilla is a tool to overlay a grid over any screen region, providing a guide
 to help you in your composition and drawing.
 .
 AGrilla helps artists to create accurate and detailed drawings using the grid
 method. It is also of help in composition when using the rule of thirds or the
 golden ratio.
 .
 AGrilla is distributed under the MIT license.
")

# CPack configuration for every platform
if(UNIX AND NOT APPLE)
    # CPack configuration to generate a Linux .deb package
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^x86_64$|^amd64$")  
      set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    else()
      set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    endif() 
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxbase3.2-1t64 (>= 3.2.0), libwxgtk3.2-1t64 (>= 3.2.0)")
    string(CONCAT CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}"
            "_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Cecilio Salmeron <s.cecilio@gmail.com>")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/cecilios/agrilla")
    set(CPACK_STRIP_FILES ON)        #remove debug information, if any, from binaries
    set(CPACK_PACKAGE_EXECUTABLES "agrilla" "AGrilla")      #To create Start Menu shortcut


elseif(WIN32)
#    # CPack configuration to generate a Windows .msi installer
#    set(CPACK_GENERATOR "WIX")
#    string(CONCAT CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}"
#            "_${CPACK_PACKAGE_VERSION}")
#
#    # This is the AGrilla GUID. Do not change!
#    set(CPACK_WIX_PRODUCT_GUID "18b6f21d-86d0-486e-a2d8-09871e97714e")
#    # Generate a GUID for the package version
#    string(UUID CPACK_WIX_UPGRADE_GUID
#           NAMESPACE DNS
#           NAME "${CPACK_PACKAGE_FILE_NAME}"
#           TYPE TIME)
#    message("GUID generado: ${CPACK_WIX_UPGRADE_GUID}")

endif()

# Must be after the last CPACK macro
include(CPack)

message(" ")
message("End of CMakeLists.txt ==================================================")
message(" ")
message("Generation starts:")
