# - Top level build script for Geant4DevelopmentTools project
#
#----------------------------------------------------------------------------
# Copyright 2012 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012 University of Warwick
#
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
project(Geant4DevelopmentTools)



#-----------------------------------------------------------------------
# General configuration
#
# - OS X vs Linux library search path
set(PLATFORM_LD_LIBRARY_PATH_NAME "LD_LIBRARY_PATH")
if(APPLE AND UNIX)
  set(PLATFORM_LD_LIBRARY_PATH_NAME "DYLD_LIBRARY_PATH")
endif()


#----------------------------------------------------------------------------
# Configure files for the BUILD tree
# Files : geant4make.(c)sh, geant4-config, Geant4Make.gmk
# These allow applications to be built against the current build of Geant4.
# This functionality is primarily for developers, but also allows an
# in place build if user requires.
#
foreach(_template 
    Templates/geant4.sh.in 
    Templates/geant4.csh.in
    Templates/geant4-config.in)
  string(REGEX REPLACE
    ".*/(.*)\\.in$"
    "${PROJECT_BINARY_DIR}/CurrentGeant4Build/bin/\\1"
    _outfile
    ${_template})
  configure_file(${_template} ${_outfile} @ONLY)
endforeach()

#----------------------------------------------------------------------------
# Hmm, geant4-config needs to be executable...
# This works, but it seems messy...
set(LOCAL_GEANT4_BINDIR ${PROJECT_BINARY_DIR}/CurrentGeant4Build/bin)
set(LOCAL_GEANT4CONFIG ${LOCAL_GEANT4_BINDIR}/geant4-config)

# - Copy the script to a temporary directory so we can change the perms
file(COPY ${LOCAL_GEANT4CONFIG}
  DESTINATION ${LOCAL_GEANT4_BINDIR}/.g4tmp
  FILE_PERMISSIONS 
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

# - Remove the original file...
file(REMOVE ${LOCAL_GEANT4CONFIG})

# - Copy the file with changed permissions back...
file(COPY ${LOCAL_GEANT4_BINDIR}/.g4tmp/geant4-config
  DESTINATION ${LOCAL_GEANT4_BINDIR})

# - Remove the tmp directory used for the copy...
file(REMOVE_RECURSE ${LOCAL_GEANT4_BINDIR}/.g4tmp)

# Got to be an easier way than this.... Could just have the original file
# with appropriate permissions, depending on repository policy...


#----------------------------------------------------------------------------
# Configure files for the INSTALL tree
# Files : geant4.(c)sh, geant4make.(c)sh, geant4-config, Geant4Make.gmk
# These allow applications to be built against the installed Geant4.
# We put them in a special "InstallTreeFiles" directory to insulate them.
#
set(INSTALL_TREE_FILES
  Templates/geant4.sh.in
  Templates/geant4.csh.in
  Templates/geant4-config.in)

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
