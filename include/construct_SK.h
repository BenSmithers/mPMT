#ifndef CONSTRUCTION_SK
#define CONSTRUCTION_SK

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Polycone.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4UnionSolid.hh"
#include "G4Hype.hh"
#include "G4MultiUnion.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Transform3D.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SubtractionSolid.hh"
#include "G4BooleanSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "mysteppingaction.hh"
#include "G4UserLimits.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "action.hh"
#include "G4Region.hh"
#include "G4VUserRegionInformation.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include <cmath>
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "WCSimTuningParameters.hh"
#include "WCSimTuningMessenger.hh"



class ConstructThing : public G4VUserDetectorConstruction {
public:
  ConstructThing();
  virtual ~ConstructThing();

  

private:
  G4double a;
  G4double density;
};


class myDetectorConstruction : public G4VUserDetectorConstruction{
    public: 
        myDetectorConstruction(G4int DetConfig, WCSimTuningParameters* WCSimTuningPars);
        ~myDetectorConstruction();
        static myDetectorConstruction* GetInstance() {return instance;}

        G4VPhysicalVolume *Construct();

        G4LogicalVolume* GetScoringVolume() const {return fScoringVolume;}

    
    private:
        static myDetectorConstruction* instance;
        G4double density;
        G4double a;

        G4LogicalVolume *fScoringVolume;

        WCSimTuningParameters* WCSimTuningParams;
        G4int myConfiguration;


};


#endif