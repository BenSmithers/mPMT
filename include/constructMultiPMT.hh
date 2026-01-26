/*
Author:    Mohit Gola 10th July 2023
*/

#ifndef CONSTRUCTMULTIPMT_HH
#define CONSTRUCTMULTIPMT_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RunManager.hh"

class myDetectorConstruction;

class ConstructMultiPMT : public G4VUserDetectorConstruction
{
public:
  ConstructMultiPMT(myDetectorConstruction *detectorConstruction, G4LogicalVolume *logicPMT);
  virtual ~ConstructMultiPMT();

  G4VPhysicalVolume *Construct();

  G4VPhysicalVolume *GetphysWorld() const { return physWorld; }
  G4LogicalVolume *GetWorldVolume() { return logicmPMT; }
  G4LogicalVolume *GetLogicmPMT() const { return logicmPMT; }
  //  G4LogicalVolume* ConstructPMTCopy();
private:
  myDetectorConstruction *detectorConstruction;
  G4VPhysicalVolume *physWorld;
  G4LogicalVolume *logicmPMT;
  G4LogicalVolume *logicPMT;
  G4double a;
  G4double density;
};

#endif
