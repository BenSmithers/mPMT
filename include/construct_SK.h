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
    const static G4int NEWNUMWAV = 100;

    G4double ENERGY_COATED_SK[NUMSK] =
        {
            1.000 * eV, 2.786 * eV, 3.061 * eV, 3.306 * eV, 3.679 * eV, 9.000 * eV};
    // Real refractive index of photocathode film
    G4double COATEDRINDEX_glasscath_SK[NUMSK] =
        {3.4, 3.4, 3.1, 2.8, 2.4, 2.4};
    // Imaginary refractive index of photocathode film
    G4double COATEDRINDEXIM_glasscath_SK[NUMSK] =
        {1.7, 1.7, 1.6, 1.5, 1.4, 1.4};

    G4double COATEDTHICKNESS_glasscath_SK = 15.5 * nm;    // 20.5 * nm;
    G4double COATEDTHICKNESS_glasscath_KCsCb = 10.5 * nm; // 15.5 * nm;
    G4double COATEDTHICKNESS_glasscath_RbCsCb = 23.4 * nm;
    G4int COATEDFRUSTRATEDTRANSMISSION_glasscath = 1;

    G4double ENERGY_COATED_WAV[NUMWAV] = {
        1.000 * eV, 1.823 * eV, 1.864 * eV, 1.907 * eV, 1.953 * eV,
        2.000 * eV, 2.049 * eV, 2.101 * eV, 2.156 * eV, 2.214 * eV,
        2.275 * eV, 2.339 * eV, 2.407 * eV, 2.480 * eV, 2.556 * eV,
        2.638 * eV, 2.725 * eV, 2.818 * eV, 2.917 * eV, 3.024 * eV,
        3.139 * eV, 3.263 * eV, 9.000 * eV};

    G4double NEW_ENERGY_COATED_WAV[NEWNUMWAV] = {
        1.8000 * eV,
        1.8152 * eV,
        1.8303 * eV,
        1.8455 * eV,
        1.8606 * eV,
        1.8758 * eV,
        1.8909 * eV,
        1.9061 * eV,
        1.9212 * eV,
        1.9364 * eV,
        1.9515 * eV,
        1.9667 * eV,
        1.9818 * eV,
        1.9970 * eV,
        2.0121 * eV,
        2.0273 * eV,
        2.0424 * eV,
        2.0576 * eV,
        2.0727 * eV,
        2.0879 * eV,
        2.1030 * eV,
        2.1182 * eV,
        2.1333 * eV,
        2.1485 * eV,
        2.1636 * eV,
        2.1788 * eV,
        2.1939 * eV,
        2.2091 * eV,
        2.2242 * eV,
        2.2394 * eV,
        2.2545 * eV,
        2.2697 * eV,
        2.2848 * eV,
        2.3000 * eV,
        2.3152 * eV,
        2.3303 * eV,
        2.3455 * eV,
        2.3606 * eV,
        2.3758 * eV,
        2.3909 * eV,
        2.4061 * eV,
        2.4212 * eV,
        2.4364 * eV,
        2.4515 * eV,
        2.4667 * eV,
        2.4818 * eV,
        2.4970 * eV,
        2.5121 * eV,
        2.5273 * eV,
        2.5424 * eV,
        2.5576 * eV,
        2.5727 * eV,
        2.5879 * eV,
        2.6030 * eV,
        2.6182 * eV,
        2.6333 * eV,
        2.6485 * eV,
        2.6636 * eV,
        2.6788 * eV,
        2.6939 * eV,
        2.7091 * eV,
        2.7242 * eV,
        2.7394 * eV,
        2.7545 * eV,
        2.7697 * eV,
        2.7848 * eV,
        2.8000 * eV,
        2.8152 * eV,
        2.8303 * eV,
        2.8455 * eV,
        2.8606 * eV,
        2.8758 * eV,
        2.8909 * eV,
        2.9061 * eV,
        2.9212 * eV,
        2.9364 * eV,
        2.9515 * eV,
        2.9667 * eV,
        2.9818 * eV,
        2.9970 * eV,
        3.0121 * eV,
        3.0273 * eV,
        3.0424 * eV,
        3.0576 * eV,
        3.0727 * eV,
        3.0879 * eV,
        3.1030 * eV,
        3.1182 * eV,
        3.1333 * eV,
        3.1485 * eV,
        3.1636 * eV,
        3.1788 * eV,
        3.1939 * eV,
        3.2091 * eV,
        3.2242 * eV,
        3.2394 * eV,
        3.2545 * eV,
        3.2697 * eV,
        3.2848 * eV,
        3.3000 * eV,
    };
    G4double COATEDRINDEX_glasscath_KCsCb[NEWNUMWAV] = {
        2.9634,
        2.9632,
        2.9631,
        2.9630,
        2.9629,
        2.9597,
        2.9568,
        2.9566,
        2.9565,
        2.9564,
        2.9596,
        2.9633,
        2.9671,
        2.9704,
        2.9737,
        2.9767,
        2.9805,
        2.9868,
        2.9947,
        3.0050,
        3.0175,
        3.0309,
        3.0427,
        3.0546,
        3.0727,
        3.0907,
        3.1080,
        3.1253,
        3.1425,
        3.1594,
        3.1759,
        3.1991,
        3.2229,
        3.2394,
        3.2585,
        3.2638,
        3.2486,
        3.2178,
        3.1768,
        3.1329,
        3.0956,
        3.0588,
        3.0389,
        3.0191,
        2.9999,
        2.9978,
        2.9958,
        2.9942,
        2.9977,
        3.0012,
        3.0046,
        3.0068,
        3.0086,
        3.0103,
        3.0046,
        2.9943,
        2.9840,
        2.9649,
        2.9431,
        2.9212,
        2.8929,
        2.8630,
        2.8332,
        2.8034,
        2.7737,
        2.7440,
        2.7225,
        2.7019,
        2.6866,
        2.6744,
        2.6622,
        2.6501,
        2.6313,
        2.6107,
        2.5901,
        2.5618,
        2.5294,
        2.4969,
        2.4644,
        2.4315,
        2.3985,
        2.3656,
        2.3377,
        2.3151,
        2.2926,
        2.2700,
        2.2474,
        2.2175,
        2.1858,
        2.1546,
        2.1238,
        2.0930,
        2.0622,
        2.0316,
        2.0017,
        1.9719,
        1.9420,
        1.9121,
        1.8823,
        1.8524,
    };
    G4double COATEDRINDEXIM_glasscath_KCsCb[NEWNUMWAV] = {
        0.3233,
        0.3274,
        0.3315,
        0.3357,
        0.3393,
        0.3393,
        0.3393,
        0.3393,
        0.3393,
        0.3393,
        0.3451,
        0.3535,
        0.3614,
        0.3691,
        0.3723,
        0.3764,
        0.3782,
        0.3853,
        0.3964,
        0.4090,
        0.4208,
        0.4309,
        0.4422,
        0.4551,
        0.4722,
        0.4897,
        0.5075,
        0.5256,
        0.5419,
        0.5578,
        0.5840,
        0.6169,
        0.6629,
        0.7134,
        0.7779,
        0.8324,
        0.8862,
        0.9408,
        0.9879,
        1.0200,
        1.0430,
        1.0599,
        1.0667,
        1.0672,
        1.0623,
        1.0613,
        1.0637,
        1.0670,
        1.0748,
        1.0934,
        1.1215,
        1.1607,
        1.2020,
        1.2431,
        1.2826,
        1.3242,
        1.3611,
        1.3832,
        1.4003,
        1.4139,
        1.4276,
        1.4404,
        1.4520,
        1.4636,
        1.4749,
        1.4861,
        1.4956,
        1.5032,
        1.5032,
        1.5032,
        1.5032,
        1.5046,
        1.5131,
        1.5251,
        1.5418,
        1.5659,
        1.5940,
        1.6244,
        1.6517,
        1.6750,
        1.6925,
        1.7039,
        1.7124,
        1.7159,
        1.7117,
        1.7073,
        1.7006,
        1.6939,
        1.6901,
        1.6867,
        1.6834,
        1.6834,
        1.6834,
        1.6837,
        1.6862,
        1.6887,
        1.6912,
        1.6937,
        1.6962,
        1.6987,
    };

    G4double COATEDRINDEX_glasscath_RbCsCb[NUMWAV] = {3.13, 3.13, 3.14, 3.14, 3.15, 3.18, 3.22, 3.28, 3.39, 3.32, 3.23, 3.21, 3.22, 3.16, 2.99, 2.81, 2.63, 2.50, 2.40, 2.30, 2.22, 2.07, 2.07};
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