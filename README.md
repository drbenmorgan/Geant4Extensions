SuperB1
=======
A super version of Geant4's B1 example application.

This example demonstrates a very simple application where an energy
deposit is accounted in user actions and a dose in a selected volume
is calculated.


Requirements
------------
### Core ###
* Linux or Mac OS X system (Windows should also work)
* C++ compiler (GNU, Clang, MSVC recommended)
* [CMake](http://www.cmake.org) build tool, version 2.8 or higher.
* CMake compatible build system (Make, Xcode, Visual Studio)
* [Geant4](http://geant4.cern.ch) 9.5 or higher, with at least the Low 
Energy Electromagnetic and Photon Evaporation data libraries available.
* [Boost](http://www.boost.org) 1.47.0 or higher, with at least shared_ptr
and program_options available.

### Optional ###
* Geant4 9.5 or higher with [Qt4](http://qt.nokia.com/) support (to enable 
GUI interface to SuperB1).


How to Configure, Compile and Run SuperB1
-----------------------------------------
Create a directory in which to build `SuperB1`, and run CMake, pointing
it to your Geant4 installation (the directory in which the 
[Geant4Config.cmake](http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch03s02.html#sect.Geant4ConfigCmake) file resides, and the source code for SuperB1 (the directory in which this README 
is located):

```shell
$ mkdir SuperB1-build
$ cd SuperB1-build
$ cmake -DGeant4_DIR=/path/to/Geant4ConfigDir /your/sourcedir
```

If you have Boost installed in a location unknown to CMake, then you
will have to add the `Boost_DIR` argument

```shell
$ cmake -DBoost_DIR=/path/to/boost-install -DGeant4_DIR=/path/to/Geant4ConfigDir /your/sourcedir
```

If configuration completes without error, CMake will have generated a
Makefile in the build directory. You can then build the `SuperB1` 
application by running

```shell
$ make -jN
```

where `N` is the number of parallel build jobs. On successful compilation
you should have an executable named `SuperB1` in your build directory.
This can be run directly without any further setup or installation.

At present, `SuperB1` is not installed and so functions much as old-style
Geant4 applications, where the `G4WORKDIR` directory was used to store
build products.

The above description assumes a Unix style system, where CMake will 
generate Unix Makefiles by default. If you prefer working with another 
build tool, CMake is capable of generating projects for a wide range 
of IDEs and command line tools. Please see the [CMake Documentation on 
Generators](http://cmake.org/cmake/help/v2.8.9/cmake.html#section_Generators) for help on supported build tools and creating projects for
these tools using CMake. Note that available generators are platform and
CMake version dependent!

### Running SuperB1 in Interactive Mode ###
You can get help on available command line options for `SuperB1` with
the `-h` option:

```shell
$ ./SuperB1 -h
SuperB1 Options:
  -h [ --help ]              print help message
  -s [ --session ] arg (=qt) start interactive session
```

To execute `SuperB1` in interactive mode with visualization:

```shell
$ ./SuperB1
```

This will start up the application with the default terminal or GUI 
session (shown by the default argument to the `-s` argument).
From the resultant prompt, you can type in commands to control
the application, e.g. those from `run1.mac`, line by line:  

```
Idle> /control/verbose 2
Idle> /tracking/verbose 1
Idle> /run/beamOn 10 
...
Idle> exit
```

You can also write commands into a script (also called 'macros' in Geant4
parlance) and use the `/control/execute` command to run the script:

```
Idle> /control/execute run1.mac
....
Idle> exit
```

### Running SuperB1 in Batch Mode ###
* THIS DOES NOT WORK AT THE MOMENT...

Execute `SuperB1` in 'batch' mode (i.e. without interaction) by passing 
macro files to it via the command line:

```shell
$ ./SuperB1 run2.mac
```

The output may also be redirected to file in the usual manner:

```shell
$ ./SuperB1 run2.mac > run2-results.txt
```

	
Geometry Definition
-------------------	
The geometry is constructed in the `B1DetectorConstruction` class.
The setup consists of a an envelope of box shape containing two
volumes: a spherical cone and a trapezoid.

In this example we use  some common materials materials for medical
applications. The envelope is made of water and the two inner volumes
are made from tissue and bone materials.  

The materials are created with the help of the `G4NistManager` class,
which allows to build a material from the NIST database using their
names. All available materials can be found in the Geant4 User's Guide
for Application Developers: [Appendix 10: Geant4 Materials Database](http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/apas10.html).

Physics List
------------
The particle types and physics processes which will be available
in this example are set in the `QGSP_BIC_EMY` physics list. This physics
list requires data files for low energy electromagnetic processes whose
path is defined via the `G4LEDATA` environment variable.

In addition the build-in interactive command:

```shell
/process/activate processName
/process/inactivate processName
```

allows the activation/inactivation of processes one by one.

Primary Generator
-----------------
The primary generator is defined in the `B1PrimaryGeneratorAction` class.
The default kinematic is a 6 MeV gamma, randomly distributed in front
of the envelope across 80% of the (X,Y) envelope size. 
This default setting can be changed via the Geant4 built-in commands 
of the `G4ParticleGun` class.

Detector Response
-----------------
This example demonstrates a simple scoring implemented directly
in the user action classes. Alternative ways of scoring via
Geant4 classes can be found in the other examples.

Here, the energy deposit in a selected volume is accumulated step by step
in `B1SteppingAction` and the event energy deposit deposit is done event by 
event in `B1EventAction`. The dose is then computed in 
`B1RunAction::EndOfRunAction()` and and its value is printed on the screen. 


Visualization
-------------
The visualization manager is set via the `G4VisExecutive` class
in the `main()` function in `SuperB1.cc`.    
The initialisation of the drawing is done via a set of `/vis/` commands
in the macro `vis.mac`. This macro is automatically read from
the main function when the example is used in interactive running mode.

By default, `vis.mac` opens an OpenGL viewer.
The user can switch to other graphics systems by commenting out this line
and instead uncommenting one of the other `/vis/open` statements, such as
`HepRepFile` or `DAWNFILE` (which produce files that can be viewed with the
`HepRApp` and `DAWN` viewers, respectively).

The `DAWNFILE`, `HepRepFile` drivers are always available
(since they require no external libraries). The `OpenGL` driver 
requires that the Geant4 install used to build `SuperB1` has 
`OpenGL` support

For more information on visualization, see the visualization tutorials
for [DAWN](http://geant4.slac.stanford.edu/Presentations/vis/G4DAWNTutorial/G4DAWNTutorial.html), [OpenGL](http://geant4.slac.stanford.edu/Presentations/vis/G4OpenGLTutorial/G4OpenGLTutorial.html) and [DAWN](http://geant4.slac.stanford.edu/Presentations/vis/G4HepRAppTutorial/G4HepRAppTutorial.html).

The tracks are automatically drawn at the end of each event, accumulated
for all events and erased at the beginning of the next run.


User Interfaces
---------------
The user command interface is controlled by the `UISessionFactory` class
in the the `main()` function in `SuperB1.cc`.
This factory class returns an instance of G4UIsession determined by an
input string, which in `SuperB1` is provided by a command line argument.
The default command interface, 
called `G4UIterminal`, is done via a standard `G4cin/G4cout`.
On Unix based systems one can use a smarter command interface, `G4UItcsh`.
When Geant4 is built with Qt4 support, a full GUI interface is available.

By default, `SuperB1` is configured so that it will have support for
all available interfaces supplied by the install of Geant4 it is linked 
to.

