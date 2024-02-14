/*                                                                                                                         
Author:    Mohit Gola 10th July 2023                                                                                       
*/

#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
// #include "G4OpBoundaryProcess.hh"
//#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4ParticleTable.hh"
#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"
#include "G4Gamma.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4OpticalPhoton.hh"

//#include "WCSimOpticalPhysics.hh"
#include "WCSimOpBoundaryProcess.hh"
#include "WCSimOpticalPhysicsMessenger.hh"

class MyPhysicsList : public G4VModularPhysicsList
{
public:
  MyPhysicsList();
  ~MyPhysicsList();
  virtual void SetCuts();
  //  virtual void ConstructPhysics();
  //  virtual void ConstructParticle();
  virtual void ConstructProcess();


};


#endif
