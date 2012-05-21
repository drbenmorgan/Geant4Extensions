# - Top level build script for Geant4DevelopmentTools project
#
#-----------------------------------------------------------------------
# Copyright 2012 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012 University of Warwick
#
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
project(Geant4DevelopmentTools)

#-----------------------------------------------------------------------
# Configure build product output and installation directories
# TODO : Check which versions of CMake supply this module
# TODO : Check case when user supplies absolute paths
include(GNUInstallDirs)

set(GEANT4_BUILD_PREFIX ${PROJECT_BINARY_DIR}/CurrentGeant4Build)
foreach(_d EXECPREFIX BINDIR LIBDIR)
  set(GEANT4_BUILD_${_d} ${GEANT4_BUILD_PREFIX}/${CMAKE_INSTALL_${_d}})
endforeach()

#-----------------------------------------------------------------------
# General script configuration variables
#
# - OS X vs Linux library search path variable name
set(PLATFORM_LD_LIBRARY_PATH_NAME "LD_LIBRARY_PATH")
if(APPLE AND UNIX)
  set(PLATFORM_LD_LIBRARY_PATH_NAME "DYLD_LIBRARY_PATH")
endif()

#-----------------------------------------------------------------------
# Configure files for the BUILD tree
# Files : geant4make.(c)sh, geant4-config, Geant4Make.gmk
# These allow applications to be built against the current build of Geant4.
# This functionality is primarily for developers, but also allows an
# in place build if user requires.
#
set(BUILD_TREE_FILES
  Templates/geant4.sh.in
  Templates/geant4.csh.in
  Templates/geant4-config.in)

# - We locate paths relative to GEANT4_BUILD_BINDIR
file(RELATIVE_PATH GEANT4_BINDIR_TO_PREFIX ${GEANT4_BUILD_BINDIR} ${GEANT4_BUILD_PREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_EXECPREFIX ${GEANT4_BUILD_BINDIR} ${GEANT4_BUILD_EXECPREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_LIBDIR ${GEANT4_BUILD_BINDIR} ${GEANT4_BUILD_LIBDIR})

foreach(_template ${BUILD_TREE_FILES})
  string(REGEX REPLACE
    ".*/(.*)\\.in$"
    "${GEANT4_BUILD_BINDIR}/\\1"
    _outfile
    ${_template})
  configure_file(${_template} ${_outfile} @ONLY)
endforeach()

#-----------------------------------------------------------------------
# Hmm, geant4-config needs to be executable...
# This works, but it seems messy...
set(GEANT4_BUILD_BINDIR ${PROJECT_BINARY_DIR}/CurrentGeant4Build/bin)
set(LOCAL_GEANT4CONFIG ${GEANT4_BUILD_BINDIR}/geant4-config)

# - Copy the script to a temporary directory so we can change the perms
file(COPY ${LOCAL_GEANT4CONFIG}
  DESTINATION ${GEANT4_BUILD_BINDIR}/.g4tmp
  FILE_PERMISSIONS 
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

# - Remove the original file...
file(REMOVE ${LOCAL_GEANT4CONFIG})

# - Copy the file with changed permissions back...
file(COPY ${GEANT4_BUILD_BINDIR}/.g4tmp/geant4-config
  DESTINATION ${GEANT4_BUILD_BINDIR})

# - Remove the tmp directory used for the copy...
file(REMOVE_RECURSE ${GEANT4_BUILD_BINDIR}/.g4tmp)

# Got to be an easier way than this.... Could just have the original file
# with appropriate permissions, depending on repository policy...


#-----------------------------------------------------------------------
# Configure files for the INSTALL tree
# Files : geant4.(c)sh, geant4make.(c)sh, geant4-config, Geant4Make.gmk
# These allow applications to be built against the installed Geant4.
# We put them in a special "InstallTreeFiles" directory to insulate them.
#
set(INSTALL_TREE_FILES
  Templates/geant4.sh.in
  Templates/geant4.csh.in
  Templates/geant4-config.in)

# - We locate paths relative to CMAKE_INSTALL_BINDIR
file(RELATIVE_PATH GEANT4_BINDIR_TO_PREFIX ${CMAKE_INSTALL_FULL_BINDIR} ${CMAKE_INSTALL_PREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_EXECPREFIX ${CMAKE_INSTALL_FULL_BINDIR} ${CMAKE_INSTALL_PREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_LIBDIR ${CMAKE_INSTALL_FULL_BINDIR} ${CMAKE_INSTALL_FULL_LIBDIR})

foreach(_template ${INSTALL_TREE_FILES})
  # Strip out the filename and prepend the output directory
  string(REGEX REPLACE 
    ".*/(.*)\\.in$" 
    "${PROJECT_BINARY_DIR}/InstallTreeFiles/\\1" 
    _outfile 
    ${_template})
  configure_file(${_template} ${_outfile} @ONLY)
endforeach()

# - Install them....

#-----------------------------------------------------------------------
# Testing
#
enable_testing()
add_subdirectory(tests)

