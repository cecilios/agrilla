cmake_minimum_required(VERSION 3.10)
project(agrilla VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define variables needed for config.h
set(AGRILLA_RES_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/agrilla/res")
set(AGRILLA_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}/" )    #AGRILLA_INSTALL_ROOT is needed for config.h
set(AGRILLA_BUILD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/" )

#set shared folder
if(UNIX AND NOT APPLE)
    set(SHARED_DIR ${AGRILLA_INSTALL_ROOT}/share/${CMAKE_PROJECT_NAME})
elseif(APPLE)
    set(SHARED_DIR ${AGRILLA_INSTALL_ROOT}/Resources )
elseif(WIN32)
    set(SHARED_DIR ${AGRILLA_INSTALL_ROOT} )
endif()

message(STATUS "SHARED_DIR = ${SHARED_DIR}" )


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

# Add headers for installation
set(HEADER_FILES
    include/DlgAspectRatio.h
    include/DlgGridOptions.h
    include/MainFrame.h
    include/TheApp.h
    include/ToolBar.h
)

# Add resources for installation
set(RESOURCE_FILES
    res/grid-on.svg
    res/grid-off.svg
    res/golden-lines-on.svg
    res/golden-lines-off.svg
    res/locked-aspect-ratio.svg
    res/unlocked-aspect-ratio.svg
    res/options.svg
    res/shutdown.svg
    res/handlers-on.svg
    res/handlers-off.svg
)

# Add the executable
add_executable(agrilla ${SOURCE_FILES})

# Link with wxWidgets libraries
target_link_libraries(agrilla
    ${wxWidgets_LIBRARIES}
)

# Installation rules
install(TARGETS agrilla DESTINATION bin)
install(FILES ${HEADER_FILES} DESTINATION include/agrilla)
install(FILES ${RESOURCE_FILES} DESTINATION share/agrilla/res)
