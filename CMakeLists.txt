# - Top level CMake script for SuperB1
#
#-----------------------------------------------------------------------
# Enforce an out-of-source build.
# Should be the first action in the CMakeLists.txt
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(STATUS "SuperB1 requires an out-of-source build.")
  message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR}:")
  message(STATUS "  CMakeCache.txt")
  message(STATUS "  CMakeFiles")
  message(STATUS "Once these files are removed, create a separate directory")
  message(STATUS "and run CMake from there, pointing it to:")
  message(STATUS "  ${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "in-source build detected")
endif()

#-----------------------------------------------------------------------
# Set up the project
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(SuperB1)

#-----------------------------------------------------------------------
# Prepend this project's custom CMake module/template path(s) to 
# CMAKE_MODULE_PATH
#
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

#-----------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by 
# default
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all qt)
else()
  find_package(Geant4 REQUIRED)
endif()

#-----------------------------------------------------------------------
# Set up Geant4 include directories and compile definitions
# Set up include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/Source)

#-----------------------------------------------------------------------
# List sources and headers for this project
# NB: headers are included so they will show up in IDEs like Xcode, VS
#
set(SUPERB1_LIB_SOURCES
  Source/B1DetectorConstruction.cc
  Source/B1DetectorConstruction.hh
  Source/B1EventAction.cc
  Source/B1EventAction.hh
  Source/B1PrimaryGeneratorAction.cc
  Source/B1PrimaryGeneratorAction.hh
  Source/B1RunAction.cc
  Source/B1RunAction.hh
  Source/B1SteppingAction.cc
  Source/B1SteppingAction.hh
  Source/UISessionFactory.hh
  )

#-----------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(SuperB1 Source/SuperB1.cc ${SUPERB1_LIB_SOURCES})
target_link_libraries(SuperB1 ${Geant4_LIBRARIES})

#-----------------------------------------------------------------------
# Copy all scripts to the a Resources directory in the build directory, 
# i.e. the directory in which we build SuperB1. 
# This is so that we can run the executable directly in the build
# directory because it relies on these scripts being in a Resources
# subdirectory of the current working directory.
#
set(SUPERB1_SCRIPTS
  Resources/init.mac
  Resources/init_vis.mac
  Resources/run1.mac
  Resources/run2.mac
  Resources/vis.mac
  )

foreach(_script ${SUPERB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

