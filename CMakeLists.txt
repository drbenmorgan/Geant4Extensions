# - Top level build script for Geant4DevelopmentTools project
#
#----------------------------------------------------------------------------
# Copyright 2012 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012 University of Warwick
#
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
project(Geant4DevelopmentTools)


#----------------------------------------------------------------------------
# Configure files for the BUILD tree
# Files : geant4make.(c)sh, geant4-config, Geant4Make.gmk
# These allow applications to be built against the current build of Geant4.
# This functionality is primarily for developers, but also allows an
# in place build if user requires.
#
foreach(_template Templates/geant4.sh.in Templates/geant4.csh.in)
  string(REGEX REPLACE
    ".*/(.*)\\.in$"
    "${PROJECT_BINARY_DIR}/CurrentGeant4Build/bin/\\1"
    _outfile
    ${_template})
  configure_file(${_template} ${_outfile} @ONLY)
endforeach()


#----------------------------------------------------------------------------
# Configure files for the INSTALL tree
# Files : geant4.(c)sh, geant4make.(c)sh, geant4-config, Geant4Make.gmk
# These allow applications to be built against the installed Geant4.
# We put them in a special "InstallTreeFiles" directory to insulate them.
#
set(INSTALL_TREE_FILES
  Templates/geant4.sh.in
  Templates/geant4.csh.in)

foreach(_template ${INSTALL_TREE_FILES})
  # Strip out the filename and prepend the output directory
  string(REGEX REPLACE 
    ".*/(.*)\\.in$" 
    "${PROJECT_BINARY_DIR}/InstallTreeFiles/\\1" 
    _outfile 
    ${_template})
  configure_file(${_template} ${_outfile} @ONLY)
endforeach()


