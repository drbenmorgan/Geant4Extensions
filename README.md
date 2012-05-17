Geant4DevelopmentTools
======================
A small project to improve the configuration, install and usability of
the tools used for setting up Geant4 and developing applications.

* geant4.(c)sh : Basic environment setup, paths and data.
* geant4make.(c)sh : Old style environment setup for Geant4 GNUmake system.
* geant4-config : Tool for querying paths and available features.
* Geant4Make.gmk : New style pure GNU make setup for Geant4 GNUmake system.

These are all UNIX tools only, we do not support Cygwin/WIN32 here.
CMake is the recommended tool here, though other native setup scripts
could be considered (NMake?). Nevertheless, will want to consider
something better than current manual setup of DLL and data paths, perhaps
a .bat script or registry entries?

Script setup has gotten quite complicated, so the aim is to simplify current
CMake system for generating the scripts and to rationalize the number
of scripts.


geant4.(c)sh
============
The task of this script is to be sourced to set the PATH, 
(DY)LD_LIBRARY_PATH and Geant4 Physics Data variables. It should be 
self-locating to allow a flexible install and easy binary packaging.

In Geant4 9.5.0, these scripts are configured from a single skeleton
file with CMake being used to replace @VAR@ style entries with the relevant 
syntax for C or Bourne shell environment and path setting. Whilst quite
neat, it leads to rather complicated CMake coding in 
Geant4ToolchainBackwardCompatibility.cmake and also expects that C and
Bourne shell scripts will have identical structure. The requirement to
have the script be self-locating means that the structure needs to 
be slightly different in each case.

We therefore wish to have two input files:

* geant4.sh.in for Bourne family shells.
* geant4.csh.in for C family shells.

Whilst this increases the number of files, it should simplify the CMake
setup considerably.

We would like to simplify the data variables to use a single location,
and to allow editing to add or remove data libraries.

On Mac OS X, we need to investigate if DYLD_LIBRARY_PATH is actually
needed anymore due to CMake setting the install name of the Geant4
libraries.


geant4make.(c)sh
================
The task of this script is set the needed environment variables required
to build a Geant4 application using Geant4's GNU makefile system.

As with geant4.(c)sh, we want to have one input file for each shell family.
The possibly tricky part here is that we don't want to set some variables
so it's not simply a case of having something like

export VARIABLE=@CMAKE_VARIABLE_NAME@

because that will still result in the variable being set in the environment.
Geant4's GNU make system only checks if variables are set, not their value.
That means we may misconfigure the GNU make system. Something like the
cmakedefine behaviour is needed...

We want to add support for setting up the third party packages used, such
as Qt. We have all the information needed in CMake, but we will need to
translate it into the variables and form expected by Geant4 GNU make.
The variables are:

* X11 : X11FLAGS, X11LIBS
* Xm  : XMFLAGS, XMLIBS
* OpenGL : OGLFLAGS, OGLLIBS
* Qt :  QTHOME, QTMOC, QTFLAGS, QTLIBS, GLQTLIBS
* CLHEP :
* EXPAT :
* XERCESC : XERCESCROOT, GDMLFLAGS, GDMLLIBS


geant4-config
=============
The task of this shell script is to provide a command line interface which
can be queried to get the binary, include and library paths of the Geant4
install, plus libraries and enabled features.


Geant4Make.gmk
==============
The task of this makefile fragment is to set the needed make variables
needed to build a Geant4 application using Geant4's GNU makefile system.
This mirrors the task of geant4make.(c)sh, but is implemented in pure
GNU make. This reduces the number of files, provides a better integration
with the Geant4 GNU make system and prevents environment pollution.

Configuration of an application may still be done via variables like
G4UI_USE_TCSH through the environment or makefile variables, but the gmk
file will allow checks to be applied so that the user cannot activate a
feature which is not installed.

It will also provide some custom targets to print out the features of the
used Geant4 install.

Its path may be provided through geant4-config.   
