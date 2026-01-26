/*                                                             
Author:    Mohit Gola 5th February 2024
*/

#ifndef CONSTRUCTINSITUMPMT_HH
#define CONSTRUCTINSITUMPMT_HH


#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "generator.hh"
#include "G4RunManager.hh"
#include "construction.hh"
#include "constructionPMT.hh"
#include "inSituPMT.hh"
#include "WCSimTuningParameters.hh"
#include <string>
#include <sstream>



class pmtConstruction;
class inSituPMTConstruction;


class ConstructInSitumPMT : public G4VUserDetectorConstruction {
public:
  ConstructInSitumPMT(pmtConstruction* subtractionPMT, G4VSolid* PMTSolid);
  virtual ~ConstructInSitumPMT();

  G4VPhysicalVolume* Construct();

private:
  pmtConstruction* subtractionPMT;
  G4LogicalVolume* logicInSituPMT;
  G4LogicalVolume* PMTLogic;
  G4VSolid* PMTSolid;
  G4double density;
  G4double a;

};

#endif

