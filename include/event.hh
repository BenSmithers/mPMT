/*                                                                                                                      
Author:    Mohit Gola 10th July 2023                                                                                      
*/


#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
//#include "G4AnalysisManager.hh"
#include "G4UIcommand.hh"

#include <G4AnalysisManager.hh>
#include <cmath>
#include <map>

#include "run.hh"
#include "mysteppingaction.hh"

class MySteppingAction;

class MyEventAction : public G4UserEventAction
{
public:
  MyEventAction();
  ~MyEventAction();

  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  void AddEdep(G4double edep) {fEdep += edep; }
  void IncrementNumAbsorbed();
  void IncrementNumDetected();
  void IncrementNumReflected();
  void IncrementNumTransmitted();

  G4int GetNumAbsorbed() const;
  G4int GetNumDetected() const;
  G4int GetNumReflected() const;
  G4int GetNumTransmitted() const;

  void SetPhotonAngle(G4double Angle) {photonAngle = Angle;}
  void ResetCounters();
  void SetSteppingAction(MySteppingAction* steppingAction);
private:
  G4int NumAbsorbed;
  G4int NumDetected;
  G4int NumReflected;
  G4int NumTransmitted;
  G4double fEdep;
  G4int TotalNumAbsorbed;
  G4int TotalNumReflected;
  G4int TotalNumTransmitted;

  //  std::map<G4double, G4int> TotalNumAbsorbed;
  //  std::map<G4double, G4int> TotalNumReflected;
  //  std::map<G4double, G4int> TotalNumTransmitted;

  G4int TotalNumDetected;
  G4double photonAngle;
  G4double photonAngleCorrect;
  G4int totalPhotons;

  MySteppingAction* mySteppingAction;
};

#endif
