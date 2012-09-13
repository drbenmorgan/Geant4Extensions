// SuperB1.cc - Extended ExampleB1 from Geant4
//
// Modifications to this original code are copyright
//
// Copyright (c) 2012 by Ben Morgan <bmorgan.warwick@gmail.com> 
// Copyright (c) 2012 by The University of Warwick
//
// and subject to the same license as the original code as follows
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

//
/// \file SuperB1.cc
/// \brief Main program of the B1 example

// Standard Library

// Third Party
// - Geant4
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BIC_EMY.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "Randomize.hh"

// - Boost
#include "boost/shared_ptr.hpp"

// This Project
#include "B1DetectorConstruction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "B1EventAction.hh"
#include "B1SteppingAction.hh"
#include "UISessionFactory.hh"
#include "CommandLineParser.hh"

int main(int argc,char** argv) {
  // - Parse the command line args
  CommandLineParser parser(argc,argv);
  parser.parse();

  // - Extract the information we need
  std::string sessionName(parser.session_name());

  // - Create interactive session, hardcoded for now
  // We prefer to create the session early so that all output goes
  // to the logging channel of the session
  UISessionFactory uiFactory = BuildUISessionFactory();
  boost::shared_ptr<G4UIsession> ui(uiFactory.CreateProduct(sessionName,argc,argv));

  if(!ui) {
    std::cerr << "[SuperB1::error] session \""<<sessionName<<"\" not recognized" 
              << std::endl;
    exit(EXIT_FAILURE);
  }
 
  // - Choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // - Construct the default run manager
  boost::shared_ptr<G4RunManager> runManager(new G4RunManager);

  // - Set mandatory initialization classes
  // Detector construction
  runManager->SetUserInitialization(new B1DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new QGSP_BIC_EMY;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // Primary generator action
  runManager->SetUserAction(new B1PrimaryGeneratorAction());

  // - Set user action classes
  // Stepping action
  runManager->SetUserAction(new B1SteppingAction());     

  // Event action
  runManager->SetUserAction(new B1EventAction());

  // Run action
  runManager->SetUserAction(new B1RunAction());
     
  // - Initialize G4 kernel
  runManager->Initialize();
  
  // - Initialize visualization, if required, by default, make it quiet 
#ifdef G4VIS_USE
  boost::shared_ptr<G4VisManager> visManager(new G4VisExecutive());
  visManager->SetVerboseLevel(G4VisManager::quiet);
  visManager->Initialize();
#endif

  // - Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {
    // - We're in batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  } else {
    // - We're in interactive mode : configure our UI session depending
    // on whether we have visualization active or not
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute Resources/init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute Resources/init.mac"); 
#endif
    ui->SessionStart();
  }
  
  return EXIT_SUCCESS;
}

