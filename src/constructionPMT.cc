/*                               
Author:    Mohit Gola 7th February 2024     
*/


#include "constructionPMT.hh"
#include "G4UserLimits.hh"
#include "G4AssemblyVolume.hh"
#include "G4VPhysicalVolume.hh"

pmtConstruction::pmtConstruction()
{}

pmtConstruction::~pmtConstruction()
{}


G4VSolid *pmtConstruction::ConstructionPMT()
{
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");


  //////////////////////////////////////////////////////////////
  G4double pmtGlassThickness = 2.0*mm;
  G4double pmtGlassRadius = 53.0*mm;
  G4double pmtCylRadius = 40.15*mm;
  G4double pmtTubeRadius = 26.1*mm;
  G4double pmtCylHeight = 18.0*mm;
  G4double pmtConeHeight = 20.0*mm;

  //////////////////////////////
  
  G4double zPlanesCylinder[4] = {-30., 10., 28., 52.7};
  G4double rInnerCylinder[4] = {0, 0, 0, 0};
  G4double rOuterCylinder[4] = {21.1, 41.15, 42.15, 47.5};
  G4Polycone* solidWorld = new G4Polycone("solidWorld",
                                          0,
                                          2 * M_PI,
                                          4,
                                          zPlanesCylinder,
                                          rInnerCylinder,
                                          rOuterCylinder);


  //  G4Box *solidWorld = new G4Box("solidWorld", 50*mm, 50*mm, 53*mm);
  //  G4Box *solidWorld = new G4Box("solidWorld", .25*m, .25*m, .25*m);                                                      

  G4LogicalVolume *logicWorld = new G4LogicalVolume(//solidWorld,            
                                                    solidWorld, 
                                                    Air,
                                                    "logicWorld");

  G4VisAttributes* invisible = new G4VisAttributes(false);
  logicWorld->SetVisAttributes(invisible);

  G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
                                                   G4ThreeVector(0.,0.,0.),
                                                   logicWorld,
                                                   "physWorld",
                                                   0,
                                                   false,
                                                   0,
                                                   true);


  
  ///////////////////////////Making Outer PMT/////////////////////
  G4Cons *coneSolid = new G4Cons("coneSolid",
                                 pmtTubeRadius-pmtGlassThickness,
                                 pmtTubeRadius,
                                 pmtCylRadius-pmtGlassThickness,
                                 pmtCylRadius,
                                 pmtConeHeight/2.*mm,
                                 0.*deg, 360.*deg);

  G4Tubs *tubeSolid = new G4Tubs("tubeSolid",
				 pmtCylRadius-pmtGlassThickness,
				 pmtCylRadius,
				 pmtCylHeight/2.*mm,
				 0.*deg, 360.*deg);

  G4VSolid *combineSolid1 = new G4UnionSolid("tubeCombined",
					     coneSolid,
					     tubeSolid,
					     0,
					     G4ThreeVector(0.,0.,19.0*mm));
  
  G4Box *solidCutOff = new G4Box("cutOffBox",
                                 pmtGlassRadius+1.*cm,
                                 pmtGlassRadius+1.*cm,
                                 34.597*mm);

  G4Sphere *pmtGlassSolid = new G4Sphere("pmtGlassSolid",
                                         pmtGlassRadius-pmtGlassThickness,
                                         pmtGlassRadius,
                                         0.0*deg, 360.0*deg,
                                         0.0, 90*deg);

  G4VSolid *pmtBulb = new G4SubtractionSolid("pmtGlass",
					     pmtGlassSolid,
					     solidCutOff);
  
  G4Transform3D transform(G4Translate3D(0.0, 0.0, -6.597*mm));

  combineSolid2 = new G4UnionSolid("OuterPMT",
				   combineSolid1,
				   pmtBulb,
				   transform);
  ////////////////////////Adding Absorber on the back of the PMT/////////////
  G4Tubs *absSolid = new G4Tubs("absSolid",
                                 0.*mm,
                                 pmtTubeRadius,
                                 0.5*mm,
                                 0.*deg, 360.*deg);

  G4VSolid *combineSolid3 = new G4UnionSolid("CombinedSolid3",
                                             combineSolid2,
                                             absSolid,
                                             0,
                                             G4ThreeVector(0.,0.,-9.5*mm));

  //////////////////Adding reflector on top of the PMT///////////////////////
  G4Cons *refSolid = new G4Cons("refSolid",
				36.96*mm,
				//0.*mm,
				37.76*mm,
				45.*mm,
				//0.*mm,
				45.8*mm,
				6.5*mm,
				0.*deg, 360.*deg);
  
  G4VSolid *combineSolid4 = new G4UnionSolid("CombinedSolid4",
                                             combineSolid3,
                                             refSolid,
                                             0,
                                             G4ThreeVector(0.,0.,40.*mm));

  
  pmtConstructionLogic = new G4LogicalVolume(combineSolid3,
					     Air,
					     "pmtConstructionLogic");

  //  return combineSolid4;
  return solidWorld;

}

G4VPhysicalVolume *pmtConstruction::Construct()
{

  return nullptr;
}

