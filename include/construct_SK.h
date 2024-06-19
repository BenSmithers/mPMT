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

class skDetCon : public G4VUserDetectorConstruction
{
public:
    skDetCon(G4int DetConfig, WCSimTuningParameters *WCSimTuningPars, bool is_sk);
    ~skDetCon();

    G4VPhysicalVolume *Construct();

    void DefineMaterials();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; } // or this...
    G4VPhysicalVolume *GetphysWorld() const { return physical_world; }
    G4LogicalVolume *GetWorldVolume() const { return logicWorld; }
    G4LogicalVolume *GetLogicmPMT() const { return physical_bulb; }

    G4Material *Vacuum;
    G4Material *Water;
    G4Material *Air;
    G4Material *Aluminum;
    G4Material *FacePlate;
    G4Material *SiO2;
    G4Material *B2O3;
    G4Material *Na2O;
    G4Material *PMTGlass;
    G4Material *Al2O3;
    G4Material *absorberMaterial;

private:
    const static G4int NUMSK = 6;
    G4double ENERGY_COATED_SK[NUMSK], COATEDRINDEX_glasscath_SK[NUMSK], COATEDRINDEXIM_glasscath_SK[NUMSK];
    G4double COATEDTHICKNESS_glasscath_SK, COATEDTHICKNESS_glasscath_KCsCb, COATEDTHICKNESS_glasscath_RbCsCb;
    G4int COATEDFRUSTRATEDTRANSMISSION_glasscath;

    const static G4int NUMWAV = 23;
    G4double ENERGY_COATED_WAV[NUMWAV], COATEDRINDEX_glasscath_KCsCb[NUMWAV], COATEDRINDEXIM_glasscath_KCsCb[NUMWAV], COATEDRINDEX_glasscath_RbCsCb[NUMWAV], COATEDRINDEXIM_glasscath_RbCsCb[NUMWAV];

    G4double density;
    G4double a;
    bool is_sk;

    G4VPhysicalVolume *DefineVolumes();
    G4LogicalVolume *fScoringVolume; // what we use for scoring. Things that are absorbed here should bre counted

    G4VPhysicalVolume *physical_world; // mother volume of all volumes
    G4LogicalVolume *logicWorld;
    G4LogicalVolume *physical_bulb;

    G4UserLimits fStepLimit;

    WCSimTuningParameters *WCSimTuningParams;
    G4int myConfiguration;
};

#endif