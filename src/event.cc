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
  //  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->FillNtupleDColumn(0, photonAngle);
  man->FillNtupleIColumn(1, TotalNumAbsorbed);                                                                        
  man->FillNtupleIColumn(2, TotalNumTransmitted);                                                                     
  man->FillNtupleIColumn(3, TotalNumReflected);   
  man->FillNtupleIColumn(4, xpos);   
  man->FillNtupleIColumn(5, ypos);   
  man->FillNtupleIColumn(6, zpos);   
  //  man->AddNtupleRow(0);


  man->AddNtupleRow(0);

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
