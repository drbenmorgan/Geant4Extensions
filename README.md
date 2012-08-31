SuperB1
=======

A super version of Geant4's B1 example application.

This example demonstrates a very simple application where an energy
deposit is accounted in user actions and a dose in a selected volume
is calculated. 

	
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
The particle's type and the physic processes which will be available
in this example are set in the `QGSP_BIC_EMY` physics list. This physics
list requires data files for low energy electromagnetic processes which
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
in the `main()` function in `exampleB1.cc`.    
The initialisation of the drawing is done via a set of `/vis/` commands
in the macro `vis.mac`. This macro is automatically read from
the main function when the example is used in interactive running mode.

By default, `vis.mac` opens an OpenGL viewer.
The user can switch to other graphics systems by commenting out this line
and instead uncommenting one of the other `/vis/open` statements, such as
`HepRepFile` or `DAWNFILE` (which produce files that can be viewed with the
`HepRApp` and `DAWN` viewers, respectively).

The `DAWNFILE`, `HepRepFile` drivers are always available
(since they require no external libraries), but the `OpenGL` driver 
requires:

* the visualisation & interfaces categories have been compiled
with the environment variable G4VIS_BUILD_OPENGLX_DRIVER.
* `exampleB1.cc` has been compiled with `G4VIS_USE_OPENGLX` (This is best done through Configure or CMake).

For more information on visualization, see the visualization tutorials
for [DAWN](http://geant4.slac.stanford.edu/Presentations/vis/G4DAWNTutorial/G4DAWNTutorial.html), [OpenGL](http://geant4.slac.stanford.edu/Presentations/vis/G4OpenGLTutorial/G4OpenGLTutorial.html) and [DAWN](http://geant4.slac.stanford.edu/Presentations/vis/G4HepRAppTutorial/G4HepRAppTutorial.html).

The tracks are automatically drawn at the end of each event, accumulated
for all events and erased at the beginning of the next run.

User Interfaces
---------------
The user command interface is set via the `G4UIExecutive` class
in the the `main()` function in `exampleB1.cc` 
The selection of the user command interface is then done automatically 
according to the Geant4 configuration. The default command interface, 
called `G4UIterminal`, is done via a standard `G4cin/G4cout`.
On Unix based systems one can use a smarter command interface, `G4UItcsh`. 
It is enough to set the environment variable `G4UI_USE_TCSH` before 
compiling.

How to Run
----------
Compile and link to generate the executable
```shell
$ cd B1
$ make
```

### Interactive Mode ###
Execute `exampleB1` in the 'interactive mode' with visualization:

```shell
$ exampleB1
```

This will start up a terminal or GUI session as selected at build
time. From the resultant prompt, you can type in commands to control
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

### Batch Mode ###
Execute `exampleB1`  in 'batch' mode by passing macro files on the 
command line(without visualization):

```shell
$ exampleB1 run2.mac
```

The output may also be redirected to file in the usual manner:

```shell
$ exampleB1 run2.mac > run2-results.txt
```

