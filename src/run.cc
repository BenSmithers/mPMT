/*
Author:    Mohit Gola 10th July 2023
*/

#include "run.hh"

MyRunAction::MyRunAction()
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
}

MyRunAction::~MyRunAction()
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->CloseFile();
}

void MyRunAction::BeginOfRunAction(const G4Run *run)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  G4RunManager *runManager = G4RunManager::GetRunManager();

  man->OpenFile(name_template + ".root");

  G4int master_ntupleId = man->CreateNtuple("Photons_Master", "Photons_Master");

  man->CreateNtupleIColumn(master_ntupleId, "Scanpoint_ID");
  man->CreateNtupleDColumn(master_ntupleId, "PosX_Initial");
  man->CreateNtupleDColumn(master_ntupleId, "PosY_Initial");
  man->CreateNtupleDColumn(master_ntupleId, "PosZ_Initial");
  man->FinishNtuple(master_ntupleId);

  std::stringstream strRunId;
  runId = run->GetRunID();
  strRunId << runId;

  scanpoint_ntupleId = man->CreateNtuple("Photons_" + strRunId.str(), "Photons");
  const G4UserEventAction *eventAction = runManager->GetUserEventAction();
  MyEventAction *myEventAction = dynamic_cast<MyEventAction *>(const_cast<G4UserEventAction *>(eventAction));
  myEventAction->SetScanpointNtupleID(scanpoint_ntupleId);
  myEventAction->ResetPhotNum();

  man->CreateNtupleIColumn(scanpoint_ntupleId, "Photon_ID");
  man->CreateNtupleIColumn(scanpoint_ntupleId, "Step_Number");
  man->CreateNtupleIColumn(scanpoint_ntupleId, "Step_Status");
  man->CreateNtupleDColumn(scanpoint_ntupleId, "PosX");
  man->CreateNtupleDColumn(scanpoint_ntupleId, "PosY");
  man->CreateNtupleDColumn(scanpoint_ntupleId, "PosZ");
  man->FinishNtuple(scanpoint_ntupleId);

  TotalNumAbsorbed = 0;
  TotalNumReflected = 0;
  TotalNumTransmitted = 0;
}

void MyRunAction::EndOfRunAction(const G4Run *)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  G4RunManager *runManager = G4RunManager::GetRunManager();
  const G4UserEventAction *eventAction = runManager->GetUserEventAction();
  if (eventAction)
  {
    MyEventAction *myEventAction = dynamic_cast<MyEventAction *>(const_cast<G4UserEventAction *>(eventAction));
    if (myEventAction)
    {
      man->FillNtupleIColumn(master_ntupleId, 0, runId);
      man->FillNtupleDColumn(master_ntupleId, 1, myEventAction->GetPosX());
      man->FillNtupleDColumn(master_ntupleId, 2, myEventAction->GetPosY());
      man->FillNtupleDColumn(master_ntupleId, 3, myEventAction->GetPosZ());
      man->AddNtupleRow(master_ntupleId);
      myEventAction->ResetCounters();
    }
  }
  man->Write();
}