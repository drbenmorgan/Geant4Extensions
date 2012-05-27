# - Define tests for Geant4DevelopmentTools
#
#-----------------------------------------------------------------------
# Copyright 2012 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012 University of Warwick

#-----------------------------------------------------------------------
# Basic configuration
#
set(CHECKBASHISMS_EXECUTABLE ${PROJECT_SOURCE_DIR}/tests/checkbashisms.pl)


#-----------------------------------------------------------------------
# Check bashisms in InstallTreeFiles
#
add_test(CheckBashisms-geant4-config_install
  ${CHECKBASHISMS_EXECUTABLE} ${PROJECT_BINARY_DIR}/InstallTreeFiles/geant4-config)

add_test(CheckBashisms-geant4_sh_install
  ${CHECKBASHISMS_EXECUTABLE} ${PROJECT_BINARY_DIR}/InstallTreeFiles/geant.sh)

#-----------------------------------------------------------------------
# Check bashisms in Build tree files
#
add_test(CheckBashisms-geant4-config_build
  ${CHECKBASHISMS_EXECUTABLE} ${GEANT4_BUILD_BINDIR}/geant4-config)

add_test(CheckBashisms-geant4_sh_build
  ${CHECKBASHISMS_EXECUTABLE} ${GEANT4_BUILD_BINDIR}/geant.sh)

#-----------------------------------------------------------------------
# Check csh scripts can be sourced from other scripts
#
add_test(NAME TestCshSourceInScriptFails
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${PROJECT_SOURCE_DIR}/tests/testSourceGeant4.csh)
set_property(TEST TestCshSourceInScriptFails 
  PROPERTY PASS_REGULAR_EXPRESSION "^geant4\\.csh:ERROR: could not locate Geant4 installation")

add_test(NAME TestCshSourceInScriptInBinDir
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${PROJECT_SOURCE_DIR}/tests/testSourceGeant4InDir.csh)

add_test(NAME TestCshSourceInScriptWithArgs
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${PROJECT_SOURCE_DIR}/tests/testSourceGeant4WithArg.csh)
