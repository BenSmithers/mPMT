/*                                                                                                                        
Author:    Mohit Gola 10th July 2023                                                                                     
*/

#ifndef MYSTEPPINGACTION_HH
#define MYSTEPPINGACTION_HH

#include <typeinfo>
#include "G4UserSteppingAction.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4VUserPhysicsList.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Track.hh"
#include "event.hh"
#include "G4OpticalPhoton.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4LogicalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "construct_SK.h"

//#include "tls.hh"
//#include "WCSimOpticalPhysics.hh"
//include "WCSimOpBoundaryProcess.hh"
#include "G4OpticalPhysics.hh"

#include "G4OpticalPhysicsMessenger.hh"
#include <cmath>
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "event.hh"
//#inlcude <CLHEP/Units/SystemOfUnits.h>

class MyEventAction;

class MySteppingAction : public G4UserSteppingAction
{
public:
  MySteppingAction();
  ~MySteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
  G4bool IsTrackCounted(G4int trackID);
  
  double calculateIncidenceAngle(const G4ThreeVector& Momentum, const G4ThreeVector& Normal);
  const std::vector<G4ThreeVector>& GetPhotonPositions() const;
  void SetEventAction(MyEventAction* eventAction);
private:
  MyEventAction *fEventAction;
  std::map<G4int, std::string> trackStatus;
  G4OpticalSurface* OpticalSurface;
  std::vector<G4ThreeVector> photonPositions;
};

#endif
