/*                                                                                                                         
Author:    Mohit Gola 10th July 2023                                                                                      
*/


#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "G4Event.hh"
#include "G4OpticalPhoton.hh"
#include <G4AnalysisManager.hh>
#include "G4DynamicParticle.hh"
#include "MyPrimaryGeneratorMessenger.hh"


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  MyPrimaryGenerator();
  ~MyPrimaryGenerator();

  virtual void GeneratePrimaries(G4Event*);
  G4ThreeVector calculateNewPositionAndAngle(const G4ThreeVector& initialPosition, double initialAngle,
				    G4ThreeVector& newPosition, double& newAngle);
  void SetAngle(G4double angle);
  void SetEnergy(G4double energy);
  void SetDiscRad(G4double discrad);
private:
  G4ParticleGun *fParticleGun;
  G4double angleDegrees;
  G4double angleRadians;
  G4double discRadius = 250*mm;
  G4double particleEnergy;
  MyPrimaryGeneratorMessenger<MyPrimaryGenerator>* fMessenger;
};

class NewGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  NewGenerator();

  virtual void GeneratePrimaries(G4Event*);
  G4ThreeVector calculateNewPositionAndAngle(const G4ThreeVector& initialPosition, double initialAngle,
				    G4ThreeVector& newPosition, double& newAngle);
  void SetAngle(G4double angle);
  void SetEnergy(G4double energy);
  void SetDiscRad(G4double discrad);
private:
  G4ParticleGun *fParticleGun;
  G4double angleDegrees;
  G4double angleRadians;
  G4double discRadius = 250*mm;
  G4double particleEnergy;
  MyPrimaryGeneratorMessenger<NewGenerator>* fMessenger;
};


#endif
