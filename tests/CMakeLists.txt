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

