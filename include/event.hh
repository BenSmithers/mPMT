/*
Author:    Mohit Gola 10th July 2023
*/

#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
// #include "G4AnalysisManager.hh"
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

  virtual void BeginOfEventAction(const G4Event *);
  virtual void EndOfEventAction(const G4Event *);

  void AddEdep(G4double edep) { fEdep += edep; }
  void IncrementNumAbsorbed();
  void IncrementNumDetected();
  void IncrementNumReflected();
  void IncrementNumTransmitted();
  void RecordStep(G4int status, G4ThreeVector steppos, G4int killstatus);

  G4int GetNumAbsorbed() const;
  G4int GetNumDetected() const;
  G4int GetNumReflected() const;
  G4int GetNumTransmitted() const;
  G4double GetPosX() { return xpos; }
  G4double GetPosY() { return ypos; }
  G4double GetPosZ() { return zpos; }

  void SetPhotonAngle(G4double Angle) { photonAngle = Angle; }
  void SetPosX(G4double PosX) { xpos = PosX; }
  void SetPosY(G4double PosY) { ypos = PosY; }
  void SetPosZ(G4double PosZ) { zpos = PosZ; }
  void SetScanpointNtupleID(G4double ScanpointNtupleID) { scanpoint_ntupleId = ScanpointNtupleID; }
  void ResetPhotNum() { photnum = 1; }
  void ResetCounters();
  void SetSteppingAction(MySteppingAction *steppingAction);

private:
  G4int NumAbsorbed;
  G4int NumDetected;
  G4int NumReflected;
  G4int NumTransmitted;
  G4double fEdep;
  G4int TotalNumAbsorbed;
  G4int TotalNumReflected;
  G4int TotalNumTransmitted;
  G4int master_ntupleId;
  G4int scanpoint_ntupleId;
  G4int photnum;
  G4int stepnum;

  //  std::map<G4double, G4int> TotalNumAbsorbed;
  //  std::map<G4double, G4int> TotalNumReflected;
  //  std::map<G4double, G4int> TotalNumTransmitted;

  G4int TotalNumDetected;
  G4double photonAngle;
  G4double xpos;
  G4double ypos;
  G4double zpos;
  G4double photonAngleCorrect;
  G4int totalPhotons;

  MySteppingAction *mySteppingAction;
};

#endif
