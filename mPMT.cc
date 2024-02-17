/*
Author:   Mohit Gola 10th July 2023
*/

#include <iostream>

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4SteppingVerbose.hh"
#include "G4SteppingManager.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "mysteppingaction.hh"
#include "WCSimTuningParameters.hh"
#include "generator.hh"
#include "MyPrimaryGeneratorMessenger.hh"
#include "constructMultiPMT.hh"

int main(int argc, char** argv)
{
  G4RunManager *runManager = new G4RunManager();
  //G4SteppingVerbose::SetInstance(new G4SteppingVerbose);
  runManager->SetVerboseLevel(1);
  WCSimTuningParameters* tuningpars = new WCSimTuningParameters();
  //  G4VSteppingVerbose* steppingVerbose = new G4SteppingVerbose;
  //  G4VSteppingVerbose::SetInstance(steppingVerbose);

  enum DetConfiguration {wfm =1, fwm=2};
  G4int WCSimConfiguration = fwm;

  myDetectorConstruction* myDetector = new myDetectorConstruction(WCSimConfiguration, tuningpars);
  ConstructMultiPMT* multiPMT = new ConstructMultiPMT();
  NewGenerator* generator = new NewGenerator();
  MyActionInitialization<NewGenerator>* actionman = new MyActionInitialization<NewGenerator>(generator);

  runManager->SetUserInitialization(myDetector);
  runManager->SetUserInitialization(multiPMT); 
  runManager->SetUserInitialization(new MyPhysicsList());
  runManager->SetUserInitialization(actionman);

  runManager->Initialize();

  G4UIExecutive *ui = 0;


  if(argc == 1)
    {
      ui =  new G4UIExecutive(argc, argv);
    }

  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  UImanager->ApplyCommand("/control/execute tuning_parameters.mac");
  UImanager->ApplyCommand("/gui/addMenu true");
  UImanager->ApplyCommand("/tracking/verbose 1");  

  
  //  runManager->SetUserAction(generator);

  MyPrimaryGeneratorMessenger<NewGenerator>* generatorMessenger = new MyPrimaryGeneratorMessenger<NewGenerator>(generator);
  //  runManager->SetUserAction(generatorMessenger);

  if(ui)
    {
      UImanager->ApplyCommand("/control/execute vis.mac");
      ui->SessionStart();
      //    delete ui;
    }
  else
    {
      G4String command = "/control/execute ";
      G4String filename = argv[1];
      UImanager->ApplyCommand(command+filename);
      G4cout<<"FileName = "<<filename<<G4endl;

    }
  //  file_exists("tuning_parameters.mac");


  //  ui->SessionStart();

  delete ui;
  delete runManager;
  delete visManager;
  return 0;

}

