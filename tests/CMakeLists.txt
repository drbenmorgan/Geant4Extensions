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
  ${CHECKBASHISMS_EXECUTABLE} ${PROJECT_BINARY_DIR}/InstallTreeFiles/geant4.sh)

#-----------------------------------------------------------------------
# Check bashisms in Build tree files
#
add_test(CheckBashisms-geant4-config_build
  ${CHECKBASHISMS_EXECUTABLE} ${GEANT4_BUILD_BINDIR}/geant4-config)

add_test(CheckBashisms-geant4_sh_build
  ${CHECKBASHISMS_EXECUTABLE} ${GEANT4_BUILD_BINDIR}/geant4.sh)

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
set_property(TEST TestCshSourceInScriptInBinDir 
  PROPERTY FAIL_REGULAR_EXPRESSION "^geant4\\.csh:ERROR: could not locate Geant4 installation")

add_test(NAME TestCshSourceInScriptWithArgs
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${PROJECT_SOURCE_DIR}/tests/testSourceGeant4WithArg.csh)
set_property(TEST TestCshSourceInScriptWithArgs 
  PROPERTY FAIL_REGULAR_EXPRESSION "^geant4\\.csh:ERROR: could not locate Geant4 installation")

#-----------------------------------------------------------------------
# Test the Geant4GMake system
#
# simple usage
configure_file(testGeant4GMake.gmk.in testGeant4GMake.gmk @ONLY)
add_test(NAME Geant4GMakeUsage
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_MAKE_PROGRAM} -f tests/testGeant4GMake.gmk
  )

# fail on repeated inclusion
configure_file(testGeant4GMakeMultipleInclusion.gmk.in testGeant4GMakeMultipleInclusion.gmk @ONLY)
add_test(NAME Geant4GMakeMultipleInclusion-All
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_MAKE_PROGRAM} -f tests/testGeant4GMakeMultipleInclusion.gmk
  )
set_property(TEST Geant4GMakeMultipleInclusion-All PROPERTY WILL_FAIL ON)

add_test(NAME Geant4GMakeMultipleInclusion-Target
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_MAKE_PROGRAM} -f tests/testGeant4GMakeMultipleInclusion.gmk print_config
  )
set_property(TEST Geant4GMakeMultipleInclusion-Target PROPERTY WILL_FAIL ON)


