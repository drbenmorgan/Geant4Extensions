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

#-----------------------------------------------------------------------
# Check bashisms in Build tree files
#
add_test(CheckBashisms-geant4-config_build
  ${CHECKBASHISMS_EXECUTABLE} ${GEANT4_BUILD_BINDIR}/geant4-config)

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

# Datasets
configure_file(testGeant4GMakeDataset.gmk.in testGeant4GMakeDataset.gmk @ONLY)
add_test(NAME Geant4GMakeDataset
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_MAKE_PROGRAM} -f tests/testGeant4GMakeDataset.gmk
  )

# Option Activation
configure_file(testGeant4GMakeOptionActivation.gmk.in testGeant4GMakeOptionActivation.gmk @ONLY)
add_test(NAME Geant4GMakeOptionActivation
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_MAKE_PROGRAM} -f tests/testGeant4GMakeOptionActivation.gmk
  )

# Actual Application
configure_file(testGeant4GMakeExampleApplication.gmk.in testGeant4GMakeExampleApplication.gmk @ONLY)
add_test(NAME Geant4GMakeExampleApplication
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_MAKE_PROGRAM} -f tests/testGeant4GMakeExampleApplication.gmk
  )


