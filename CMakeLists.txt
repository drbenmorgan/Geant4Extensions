# - Top level build script for Geant4DevelopmentTools project
#
#-----------------------------------------------------------------------
# Copyright 2012 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012 University of Warwick
#
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
project(Geant4DevelopmentTools)

#-----------------------------------------------------------------------
# Version number
#
set(Geant4_VERSION "10.0.0")

#-----------------------------------------------------------------------
# System/Compiler detection
# This is to give us the correct "G4SYSTEM = OS-COMPILER" combination
# for the old toolchain.
# On UNIX, we need to distinguish between GNU and LLVM...
#
if(NOT WIN32)
  set(GEANT4_SYSTEM ${CMAKE_SYSTEM_NAME})
else()
  set(GEANT4_SYSTEM "WIN32")
endif()

# - Clang isn't detected reliably on CMake < 2.8.2: Will report
# the compiler ID as "GNU". Conversely, on 2.8.2 and higher we need
# to ensure correct clang flags are used (may well be same as GNU,
# but still need to set them).

if(CMAKE_COMPILER_IS_GNUCXX)
  set(GEANT4_COMPILER "g++")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(GEANT4_COMPILER "clang")
elseif(MSVC)
  set(GEANT4_COMPILER "VC")
elseif(CMAKE_CXX_COMPILER MATCHES "icpc.*|icc.*")
  set(GEANT4_COMPILER "icc")
else()
  set(GEANT4_COMPILER "UNSUPPORTED")
endif()

#-----------------------------------------------------------------------
# Configure build product output and installation directories
# TODO : Check which versions of CMake supply this module
# TODO : Check case when user supplies absolute paths
include(GNUInstallDirs)

# - temp: set directory for GMake files to bindir for now
set(CMAKE_INSTALL_GMAKEDIR "${CMAKE_INSTALL_BINDIR}")
set(CMAKE_INSTALL_FULL_GMAKEDIR "${CMAKE_INSTALL_FULL_BINDIR}")

set(GEANT4_BUILD_PREFIX ${PROJECT_BINARY_DIR}/CurrentGeant4Build)
foreach(_d EXECPREFIX BINDIR LIBDIR GMAKEDIR)
  set(GEANT4_BUILD_${_d} ${GEANT4_BUILD_PREFIX}/${CMAKE_INSTALL_${_d}})
endforeach()

#-----------------------------------------------------------------------
# Add options/Find packages that Geant4 may use
# Not completely comprehensive - aim is simply to see how best to
# export information on available options, header and library paths from 
# the CMake layer to the Shell/Make toolchain interfaces
#
option(GEANT4_USE_SYSTEM_CLHEP "Build Geant4 with system CLHEP" OFF)
if(GEANT4_USE_SYSTEM_CLHEP)
  find_package(CLHEP REQUIRED)
  message(STATUS ${CLHEP_INCLUDE_DIR})
  get_filename_component(CLHEP_INCLUDE_DIR ${CLHEP_INCLUDE_DIR} ABSOLUTE)
  if(TARGET ${CLHEP_LIBRARIES})
    # Problem - which config (Release, etc) to select? Especially if we
    # build on a multiconfig generator...
    # take release for now, though can extract all via the 
    # IMPORTED_CONFIGURATIONS property
    get_target_property(CLHEP_LIBDIR ${CLHEP_LIBRARIES} IMPORTED_LOCATION_RELEASE)
    get_filename_component(CLHEP_LIBDIR ${CLHEP_LIBDIR} PATH)
  endif()

  message(STATUS ${CLHEP_INCLUDE_DIR})
  message(STATUS ${CLHEP_LIBDIR})
endif()

option(GEANT4_USE_SYSTEM_EXPAT "Build Geant4 with system expat" ON)

option(GEANT4_USE_SYSTEM_ZLIB "Build Geant4 with system zlib" ON)

option(GEANT4_USE_GDML "Build Geant4 with GDML Support" ON)

find_package(Qt4)

# - DATA
# NB - make has issues with paths containing spaces
set(GEANT4_DATASET_DESCRIPTIONS "G4NDL|G4NEUTRONHPDATA|/foo/bar;G4EMLOW|G4LEDATA|/frob/baz")

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
  Templates/geant4-config.in
  Templates/geant4gmake.sh.in
  Templates/geant4gmake.csh.in
  Templates/Geant4GMake.gmk.in
  Templates/Geant4GMakeTargets.gmk.in)

# - We locate paths relative to GEANT4_BUILD_BINDIR...
file(RELATIVE_PATH GEANT4_BINDIR_TO_PREFIX ${GEANT4_BUILD_BINDIR} ${GEANT4_BUILD_PREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_EXECPREFIX ${GEANT4_BUILD_BINDIR} ${GEANT4_BUILD_EXECPREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_LIBDIR ${GEANT4_BUILD_BINDIR} ${GEANT4_BUILD_LIBDIR})

# - ... and to GEANT4_BUILD_GMAKEDIR
file(RELATIVE_PATH GEANT4_GMAKEDIR_TO_PREFIX ${GEANT4_BUILD_GMAKEDIR} ${GEANT4_BUILD_PREFIX})
file(RELATIVE_PATH GEANT4_GMAKEDIR_TO_EXECPREFIX ${GEANT4_BUILD_GMAKEDIR} ${GEANT4_BUILD_EXECPREFIX})
file(RELATIVE_PATH GEANT4_GMAKEDIR_TO_LIBDIR ${GEANT4_BUILD_GMAKEDIR} ${GEANT4_BUILD_LIBDIR})


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
  Templates/geant4-config.in
  Templates/geant4gmake.sh.in
  Templates/geant4gmake.csh.in
  Templates/Geant4GMake.gmk.in
  Templates/Geant4GMakeTargets.gmk.in)

# - We locate paths relative to CMAKE_INSTALL_BINDIR
file(RELATIVE_PATH GEANT4_BINDIR_TO_PREFIX ${CMAKE_INSTALL_FULL_BINDIR} ${CMAKE_INSTALL_PREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_EXECPREFIX ${CMAKE_INSTALL_FULL_BINDIR} ${CMAKE_INSTALL_PREFIX})
file(RELATIVE_PATH GEANT4_BINDIR_TO_LIBDIR ${CMAKE_INSTALL_FULL_BINDIR} ${CMAKE_INSTALL_FULL_LIBDIR})

# - ... and to GEANT4_BUILD_GMAKEDIR
file(RELATIVE_PATH GEANT4_GMAKEDIR_TO_PREFIX ${CMAKE_INSTALL_FULL_GMAKEDIR} ${CMAKE_INSTALL_PREFIX})
file(RELATIVE_PATH GEANT4_GMAKEDIR_TO_EXECPREFIX ${CMAKE_INSTALL_FULL_GMAKEDIR} ${CMAKE_INSTALL_PREFIX})
file(RELATIVE_PATH GEANT4_GMAKEDIR_TO_LIBDIR ${CMAKE_INSTALL_FULL_GMAKEDIR} ${CMAKE_INSTALL_FULL_LIBDIR})

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

