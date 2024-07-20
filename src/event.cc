/*                                                                                                                         
Author:    Mohit Gola 10th July 2023                                                                                       
*/

#include "event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

MyEventAction::MyEventAction()
{
  fEdep = 0.;
  NumAbsorbed=0;
  TotalNumAbsorbed=0;
  TotalNumReflected=0;
  TotalNumTransmitted=0;
  TotalNumDetected=0;
  NumDetected=0;
  NumReflected=0;
  NumTransmitted=0;
  photonAngle = 0.0;
  totalPhotons =0.0;
  //  photonAngleCorrect = 0.0;
  mySteppingAction = nullptr;


}

MyEventAction::~MyEventAction()
{}

void MyEventAction::SetSteppingAction(MySteppingAction* steppingAction)
{
  mySteppingAction = steppingAction;
}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  fEdep =0;
  NumAbsorbed=0;
  NumDetected=0;
  NumReflected=0;
  NumTransmitted=0;
  stepnum=1;
  //  TotalNumAbsorbed=0;
  //  TotalNumReflected=0;
  //  TotalNumTransmitted=0;
  //  TotalNumDetected=0;
  //  TotalNumAbsorbed=0;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  /*
  const std::vector<G4ThreeVector>& photonPositions = mySteppingAction->GetPhotonPositions();
  for(size_t i = 0; i < photonPositions.size(); ++i)
    {
      const G4ThreeVector& position = photonPositions[i];
      G4double x = position.x();
      G4double y = position.y();
      G4double z = position.z();

      //    G4cout<<"X: "<<x<<", Y: "<<y<<", Z: "<<z<<G4endl;
      man->FillNtupleDColumn(0, x);
      man->FillNtupleDColumn(1, y);                                                                        
      man->FillNtupleDColumn(2, z);     
      man->AddNtupleRow(0);
    }
 */
  TotalNumAbsorbed += NumAbsorbed;
  TotalNumReflected += NumReflected;
  TotalNumTransmitted += NumTransmitted;
  TotalNumDetected += NumDetected;

  totalPhotons += NumAbsorbed+NumReflected+NumTransmitted;
  
  G4cout << "Energy Deposited == " << fEdep << G4endl;
  G4cout << "No. of Photons Absorbed == " <<TotalNumAbsorbed<< G4endl;
  G4cout << "No. of Photons Reflected == " <<TotalNumReflected<< G4endl;                                  
  G4cout << "No. of Photons Transmitted == " <<TotalNumTransmitted<< G4endl; 
  G4cout << "No. of Photons Detected == " << TotalNumDetected<< G4endl;
  G4cout << "Total no. of Photons == " << totalPhotons<< G4endl;
  G4cout<<"Photon Angle == "<< photonAngle<<G4endl;
}

void MyEventAction::IncrementNumAbsorbed()
{
  NumAbsorbed++;
}

void MyEventAction::IncrementNumDetected()
{
  NumDetected++;
}

void MyEventAction::IncrementNumReflected()
{
  NumReflected++;
}

void MyEventAction::IncrementNumTransmitted()
{
  NumTransmitted++;
}

G4int MyEventAction::GetNumAbsorbed() const
{
  return NumAbsorbed;
}

G4int MyEventAction::GetNumDetected() const
{
  return NumDetected;
}

G4int MyEventAction::GetNumReflected() const
{
  return NumReflected;
}

G4int MyEventAction::GetNumTransmitted() const
{
  return NumTransmitted;
}

void MyEventAction::ResetCounters()
{
  TotalNumAbsorbed=0;
  TotalNumReflected=0;
  TotalNumTransmitted=0;
  TotalNumDetected=0;
}

void MyEventAction::RecordStep(G4int status, G4ThreeVector steppos, G4int killstatus)
/* 
STATUS: transmission==0, reflection==1, absorption (photocathode)==2, absorption (glass)==3,
        absorption (dynode area)==4, absorption (anywhere else)==5, photon leaves world volume==6
        anything else==7 (should not really be any 7)
KILLSTATUS: if step stops and kills track, killstatus==1, otherwise killstatus==0
*/
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->FillNtupleIColumn(scanpoint_ntupleId, 0, photnum);                                                                        
  man->FillNtupleIColumn(scanpoint_ntupleId, 1, stepnum);                                                                     
  man->FillNtupleIColumn(scanpoint_ntupleId, 2, status);
  man->FillNtupleDColumn(scanpoint_ntupleId, 3, steppos.getX());   
  man->FillNtupleDColumn(scanpoint_ntupleId, 4, steppos.getY());   
  man->FillNtupleDColumn(scanpoint_ntupleId, 5, steppos.getZ());  
  man->AddNtupleRow(scanpoint_ntupleId);
  stepnum++;
  if (killstatus==1)
  {
    photnum++;
  }
}