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
const G4int NUMSK = 6;
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
    const static G4int NUMWAV = 23;

    G4double ENERGY_COATED_SK[NUMSK] =
        {
            1.000 * eV, 2.786 * eV, 3.061 * eV, 3.306 * eV, 3.679 * eV, 9.000 * eV};
    // Real refractive index of photocathode film
    G4double COATEDRINDEX_glasscath_SK[NUMSK] =
        {3.4, 3.4, 3.1, 2.8, 2.4, 2.4};
    // Imaginary refractive index of photocathode film
    G4double COATEDRINDEXIM_glasscath_SK[NUMSK] =
        {1.7, 1.7, 1.6, 1.5, 1.4, 1.4};

    G4double COATEDTHICKNESS_glasscath_SK = 20.5 * nm;
    G4double COATEDTHICKNESS_glasscath_KCsCb = 15.5 * nm;
    G4double COATEDTHICKNESS_glasscath_RbCsCb = 23.4 * nm;
    G4int COATEDFRUSTRATEDTRANSMISSION_glasscath = 1;

    G4double ENERGY_COATED_WAV[NUMWAV] = {
        1.000 * eV, 1.823 * eV, 1.864 * eV, 1.907 * eV, 1.953 * eV,
        2.000 * eV, 2.049 * eV, 2.101 * eV, 2.156 * eV, 2.214 * eV,
        2.275 * eV, 2.339 * eV, 2.407 * eV, 2.480 * eV, 2.556 * eV,
        2.638 * eV, 2.725 * eV, 2.818 * eV, 2.917 * eV, 3.024 * eV,
        3.139 * eV, 3.263 * eV, 9.000 * eV};
    G4double COATEDRINDEX_glasscath_KCsCb[NUMWAV] =
        {
            2.96, 2.96, 2.95, 2.95, 2.95,
            2.96, 2.98, 3.01, 3.06, 3.12,
            3.20, 3.26, 3.09, 3.00, 3.00,
            3.00, 2.87, 2.70, 2.61, 2.38,
            2.18, 1.92, 1.92};
    G4double COATEDRINDEXIM_glasscath_KCsCb[NUMWAV] =
        {
            0.33, 0.33, 0.34, 0.34, 0.35,
            0.37, 0.38, 0.42, 0.46, 0.53,
            0.63, 0.86, 1.05, 1.06, 1.11,
            1.34, 1.44, 1.50, 1.53, 1.71,
            1.69, 1.69, 1.69};

    G4double COATEDRINDEX_glasscath_RbCsCb[NUMWAV] = {
        3.13, 3.13, 3.14, 3.14, 3.15,
        3.18, 3.22, 3.28, 3.39, 3.32,
        3.23, 3.21, 3.22, 3.16, 2.99,
        2.81, 2.63, 2.50, 2.40, 2.30,
        2.22, 2.07, 2.07};
    G4double COATEDRINDEXIM_glasscath_RbCsCb[NUMWAV] =
        {
            0.35, 0.35, 0.37, 0.37, 0.38,
            0.40, 0.43, 0.46, 0.59, 0.76,
            0.86, 0.90, 1.04, 1.21, 1.37,
            1.41, 1.40, 1.35, 1.27, 1.21,
            1.17, 1.22, 1.22};

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