#-------------------------------------------------------------------------------------
# This is part of CMake configuration file for building makefiles and installfiles
# for the AGrilla project
#-------------------------------------------------------------------------------------
# This module creates the include file "version.h"
#
#-------------------------------------------------------------------------------------

set( AGRILLA_VERSION_MAJOR 1 )
set( AGRILLA_VERSION_MINOR 0 )
set( AGRILLA_VERSION_PATCH 0 )

# build version string for installer name
set( AGRILLA_PACKAGE_VERSION "${AGRILLA_VERSION_MAJOR}.${AGRILLA_VERSION_MINOR}.${AGRILLA_VERSION_PATCH}" )
set( AGRILLA_VERSION "${AGRILLA_PACKAGE_VERSION}" )
set( AGRILLA_VERSION_LONG "${AGRILLA_VERSION}-nogit" )

message (STATUS "  AGrilla version        = '${AGRILLA_VERSION}'")
message (STATUS "  AGrilla version string = '${AGRILLA_PACKAGE_VERSION}'")

if (EXISTS ${AGRILLA_ROOT_DIR}/.git)
  find_package (Git)
  if (NOT GIT_FOUND)
    message(SEND_ERROR "Git package not found." )
  else()
    # get sha1 and dirty status directly from git
    execute_process(COMMAND "${GIT_EXECUTABLE}" log -1 --format=%h
      WORKING_DIRECTORY "${AGRILLA_ROOT_DIR}"
      OUTPUT_VARIABLE AGRILLA_VERSION_BUILD
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND "${GIT_EXECUTABLE}" describe --tags --long --dirty
      WORKING_DIRECTORY "${AGRILLA_ROOT_DIR}"
      OUTPUT_VARIABLE AGRILLA_VERSION_GIT
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message (STATUS "  version git    = '${AGRILLA_VERSION_GIT}'")
    if ( ${AGRILLA_VERSION_GIT} MATCHES "-dirty$" )
      set (AGRILLA_VERSION_BUILD "${AGRILLA_VERSION_BUILD}-dirty")
    endif()
    if (NOT ".${AGRILLA_VERSION_BUILD}" STREQUAL "." )
      set(AGRILLA_VERSION_LONG "${AGRILLA_PACKAGE_VERSION}+${AGRILLA_VERSION_BUILD}")
    else()
      set(AGRILLA_VERSION_LONG "${AGRILLA_PACKAGE_VERSION}")
    endif()
  endif()
endif()
message (STATUS "  AGrilla version long   = '${AGRILLA_VERSION_LONG}'" )

# define a header file to pass version information to source code
configure_file(
    "${AGRILLA_ROOT_DIR}/version.h.in"
    "${CMAKE_BINARY_DIR}/version.h"
)
