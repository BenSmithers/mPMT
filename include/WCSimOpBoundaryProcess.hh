// Modified from G4OpBoundaryProcess.hh of geant4.10 to implement photocathode physics, which is a thin film of semiconductor alloy coated on glass
// Model the reflection/transmission/absorption processes by the coated layer
//
// CoatedDielectricDielectric_Model1() :
// Copy from geant4.11, which is based on https://ieeexplore.ieee.org/document/9875513
// Model the alloy as a thin layer with real refractive index, then calculate reflection and transmission probability
// Cannot handle total internal reflection when n1<n2
//
// CoatedDielectricDielectric_Model2() :
// Implementation based on https://arxiv.org/abs/physics/0408075v1
// Model the alloy as a thin layer with real and imaginary refractive indices, then calculate absorption, reflection and transmission probability
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//
////////////////////////////////////////////////////////////////////////
// Optical Photon Boundary Process Class Definition
////////////////////////////////////////////////////////////////////////
//
// File:        G4OpBoundaryProcess.hh
// Description: Discrete Process -- reflection/refraction at
//                                  optical interfaces
// Version:     1.1
// Created:     1997-06-18
// Modified:    2005-07-28 add G4ProcessType to constructor
//              1999-10-29 add method and class descriptors
//              1999-10-10 - Fill NewMomentum/NewPolarization in
//                           DoAbsorption. These members need to be
//                           filled since DoIt calls
//                           aParticleChange.SetMomentumChange etc.
//                           upon return (thanks to: Clark McGrew)
//              2006-11-04 - add capability of calculating the reflectivity
//                           off a metal surface by way of a complex index
//                           of refraction - Thanks to Sehwook Lee and John
//                           Hauptman (Dept. of Physics - Iowa State Univ.)
//              2009-11-10 - add capability of simulating surface reflections
//                           with Look-Up-Tables (LUT) containing measured
//                           optical reflectance for a variety of surface
//                           treatments - Thanks to Martin Janecek and
//                           William Moses (Lawrence Berkeley National Lab.)
//              2013-06-01 - add the capability of simulating the transmission
//                           of a dichronic filter
//              2017-02-24 - add capability of simulating surface reflections
//                           with Look-Up-Tables (LUT) developed in DAVIS
// Mofified:    Mohit Gola 10th July 2023
// Author:      Peter Gumplinger
//              adopted from work by Werner Keil - April 2/96
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#ifndef WCSimOpBoundaryProcess_h
#define WCSimOpBoundaryProcess_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "geomdefs.hh"
#include "Randomize.hh"

#include "G4RandomTools.hh"
#include "G4RandomDirection.hh"

#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4OpticalPhoton.hh"
#include "G4TransportationManager.hh"

// Class Description:
// Discrete Process -- reflection/refraction at optical interfaces.
// Class inherits publicly from G4VDiscreteProcess.
// Class Description - End:

/////////////////////
// Class Definition
/////////////////////

enum WCSimOpBoundaryProcessStatus
{
   Undefined = 1,
   Transmission,
   FresnelRefraction,
   FresnelReflection,
   TotalInternalReflection,
   LambertianReflection,
   LobeReflection,
   SpikeReflection,
   BackScattering,
   Absorption,
   Detection,
   NotAtBoundary,
   SameMaterial,
   StepTooSmall,
   NoRINDEX,
   PolishedLumirrorAirReflection,
   PolishedLumirrorGlueReflection,
   PolishedAirReflection,
   PolishedTeflonAirReflection,
   PolishedTiOAirReflection,
   PolishedTyvekAirReflection,
   PolishedVM2000AirReflection,
   PolishedVM2000GlueReflection,
   EtchedLumirrorAirReflection,
   EtchedLumirrorGlueReflection,
   EtchedAirReflection,
   EtchedTeflonAirReflection,
   EtchedTiOAirReflection,
   EtchedTyvekAirReflection,
   EtchedVM2000AirReflection,
   EtchedVM2000GlueReflection,
   GroundLumirrorAirReflection,
   GroundLumirrorGlueReflection,
   GroundAirReflection,
   GroundTeflonAirReflection,
   GroundTiOAirReflection,
   GroundTyvekAirReflection,
   GroundVM2000AirReflection,
   GroundVM2000GlueReflection,
   Dichroic,
   CoatedDielectricReflection, // new status code for thin film processes
   CoatedDielectricRefraction,
   CoatedDielectricFrustratedTransmission
};

enum BoundaryMeta
{
   BTransmission = 1,
   BReflection = 2,
   BAbsorption = 3,
   BDontCount = 4,
   BNoRINDEX = 5,
   BOther = 6
};

BoundaryMeta meta_status(WCSimOpBoundaryProcessStatus what);

class WCSimOpBoundaryProcess : public G4VDiscreteProcess
{

public:
   ////////////////////////////////
   // Constructors and Destructor
   ////////////////////////////////

   WCSimOpBoundaryProcess(const G4String &processName = "OpBoundary",
                          G4ProcessType type = fOptical);
   ~WCSimOpBoundaryProcess();

private:
   WCSimOpBoundaryProcess(const WCSimOpBoundaryProcess &right);

   //////////////
   // Operators
   //////////////

   WCSimOpBoundaryProcess &operator=(const WCSimOpBoundaryProcess &right);

public:
   ////////////
   // Methods
   ////////////

   G4bool IsApplicable(const G4ParticleDefinition &aParticleType);
   // Returns true -> 'is applicable' only for an optical photon.

   G4double GetMeanFreePath(const G4Track &,
                            G4double,
                            G4ForceCondition *condition);
   // Returns infinity; i. e. the process does not limit the step,
   // but sets the 'Forced' condition for the DoIt to be invoked at
   // every step. However, only at a boundary will any action be
   // taken.

   G4VParticleChange *PostStepDoIt(const G4Track &aTrack,
                                   const G4Step &aStep);
   // This is the method implementing boundary processes.

   WCSimOpBoundaryProcessStatus GetStatus() const;
   // Returns the current status.

   void SetInvokeSD(G4bool);
   // Set flag for call to InvokeSD method.

private:
   G4bool G4BooleanRand(const G4double prob) const;

   G4ThreeVector GetFacetNormal(const G4ThreeVector &Momentum,
                                const G4ThreeVector &Normal) const;

   void DielectricMetal();
   void DielectricDielectric();

   void DielectricLUT();
   void DielectricLUTDAVIS();
   void DielectricDichroic();

   void ChooseReflection();
   void DoAbsorption();
   void DoReflection();

   G4double GetIncidentAngle();
   // Returns the incident angle of optical photon

   G4double GetReflectivity(G4double E1_perp,
                            G4double E1_parl,
                            G4double incidentangle,
                            G4double RealRindex,
                            G4double ImaginaryRindex);
   // Returns the Reflectivity on a metalic surface

   void CalculateReflectivity(void);

   // Implementation of photocathode physics
   void CoatedDielectricDielectric_Model1();
   G4double GetReflectivityThroughThinLayer(G4double sinTL, G4double E1_perp,
                                            G4double E1_parl, G4double wavelength,
                                            G4double costh1, G4double costh2);
   void CoatedDielectricDielectric_Model2();

   void BoundaryProcessVerbose(void) const;

   // Invoke SD for post step point if the photon is 'detected'
   G4bool InvokeSD(const G4Step *step);

private:
   G4double thePhotonMomentum;

   G4ThreeVector OldMomentum;
   G4ThreeVector OldPolarization;

   G4ThreeVector NewMomentum;
   G4ThreeVector NewPolarization;

   G4ThreeVector theGlobalNormal;
   G4ThreeVector theFacetNormal;

   G4Material *Material1;
   G4Material *Material2;

   G4OpticalSurface *OpticalSurface;

   G4MaterialPropertyVector *PropertyPointer;
   G4MaterialPropertyVector *PropertyPointer1;
   G4MaterialPropertyVector *PropertyPointer2;

   G4double Rindex1;
   G4double Rindex2;

   G4double cost1, cost2, sint1, sint2;

   WCSimOpBoundaryProcessStatus theStatus;

   G4OpticalSurfaceModel theModel;

   G4OpticalSurfaceFinish theFinish;

   G4double theReflectivity;
   G4double theEfficiency;
   G4double theTransmittance;

   G4double theSurfaceRoughness;

   G4double prob_sl, prob_ss, prob_bs;

   G4int iTE, iTM;

   G4double kCarTolerance;

   size_t idx, idy;
   G4Physics2DVector *DichroicVector;

   // Used by CoatedDielectricDielectric_Model1() and CoatedDielectricDielectric_Model2()
   G4double fCoatedRindex, fCoatedRindexIm, fCoatedThickness;
   //        G4bool fCoatedFrustratedTransmission = true;
   G4bool fCoatedFrustratedTransmission;

   G4bool fInvokeSD;
   // public:
   //   WCSimOpBoundaryProcess() : fCoatedFrustratedTransmission(true){}
};

////////////////////
// Inline methods
////////////////////

inline G4bool WCSimOpBoundaryProcess::G4BooleanRand(const G4double prob) const
{
   /* Returns a random boolean variable with the specified probability */

   return (G4UniformRand() < prob);
}

inline G4bool WCSimOpBoundaryProcess::IsApplicable(const G4ParticleDefinition &
                                                       aParticleType)
{
   return (&aParticleType == G4OpticalPhoton::OpticalPhoton());
}

inline WCSimOpBoundaryProcessStatus WCSimOpBoundaryProcess::GetStatus() const
{
   return theStatus;
}

inline void WCSimOpBoundaryProcess::SetInvokeSD(G4bool flag)
{
   fInvokeSD = flag;
}

inline void WCSimOpBoundaryProcess::ChooseReflection()
{
   G4double rand = G4UniformRand();
   if (rand >= 0.0 && rand < prob_ss)
   {
      theStatus = SpikeReflection;
      theFacetNormal = theGlobalNormal;
   }
   else if (rand >= prob_ss &&
            rand <= prob_ss + prob_sl)
   {
      theStatus = LobeReflection;
   }
   else if (rand > prob_ss + prob_sl &&
            rand < prob_ss + prob_sl + prob_bs)
   {
      theStatus = BackScattering;
   }
   else
   {
      theStatus = LambertianReflection;
   }
}

inline void WCSimOpBoundaryProcess::DoAbsorption()
{
   theStatus = Absorption;
   // G4cout << "ABSORPTION OCCURED!!!!!!!!!" << G4endl;
   if (G4BooleanRand(theEfficiency))
   {

      // EnergyDeposited =/= 0 means: photon has been detected
      theStatus = Detection;
      aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
   }
   else
   {
      aParticleChange.ProposeLocalEnergyDeposit(0.0);
   }

   NewMomentum = OldMomentum;
   NewPolarization = OldPolarization;

   //              aParticleChange.ProposeEnergy(0.0);
   aParticleChange.ProposeTrackStatus(fStopAndKill);
}

inline void WCSimOpBoundaryProcess::DoReflection()
{
   if (theStatus == LambertianReflection)
   {

      NewMomentum = G4LambertianRand(theGlobalNormal);
      theFacetNormal = (NewMomentum - OldMomentum).unit();
   }
   else if (theFinish == ground)
   {

      theStatus = LobeReflection;
      if (PropertyPointer1 && PropertyPointer2)
      {
      }
      else
      {
         theFacetNormal =
             GetFacetNormal(OldMomentum, theGlobalNormal);
      }
      G4double PdotN = OldMomentum * theFacetNormal;
      NewMomentum = OldMomentum - (2. * PdotN) * theFacetNormal;
   }
   else
   {

      theStatus = SpikeReflection;
      theFacetNormal = theGlobalNormal;
      G4double PdotN = OldMomentum * theFacetNormal;
      NewMomentum = OldMomentum - (2. * PdotN) * theFacetNormal;
   }
   G4double EdotN = OldPolarization * theFacetNormal;
   NewPolarization = -OldPolarization + (2. * EdotN) * theFacetNormal;
}

#endif /* WCSimOpBoundaryProcess_h */
