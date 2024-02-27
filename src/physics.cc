/*
Author:  Mohit Gola 10th July 2023

*/

#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
  SetVerboseLevel(0);
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  // RegisterPhysics(new G4OpticalPhysics());
  RegisterPhysics(new WCSimOpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{
}

void MyPhysicsList::SetCuts()
{

  G4double gammaCut = 0.6 * mm;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(gammaCut, DBL_MAX);
}

/*
void MyPhysicsList::ConstructParticle()
{
  G4Gamma::GammaDefiniton();
}
*/

void MyPhysicsList::ConstructProcess()
{
  // Call base class ConstructProcess() method first
  G4VModularPhysicsList::ConstructProcess();

  // Get the process manager for gamma particles
  G4ProcessManager *processManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  G4bool isOpBoundaryAdded = false;
  // Create an instance of your custom optical boundary process

  G4ProcessVector *processList = processManager->GetProcessList();
  G4int numProcesses = processList->size();

  // Loop over the processes and print their names
  for (G4int i = 0; i < numProcesses; i++)
  {
    G4VProcess *process = (*processList)[i];
    G4cout << "Process Name: " << process->GetProcessName() << G4endl;
    if (process->GetProcessName() == "OpBoundary")
    {
      isOpBoundaryAdded = true;
      break;
    }
  }

  if (!isOpBoundaryAdded)
  {

    WCSimOpBoundaryProcess *opBoundaryProcess = new WCSimOpBoundaryProcess();
    opBoundaryProcess->SetVerboseLevel(0);
    // Print the process names
    G4cout << "Registering processes for gamma particles:" << G4endl;
    processManager->AddDiscreteProcess(opBoundaryProcess);
  }
}
