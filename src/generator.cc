/*                                                                                                                         
Author:    Mohit Gola 10th July 2023                                                                                       
*/

#include "generator.hh"
#include "MyPrimaryGeneratorMessenger.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
  fParticleGun = new G4ParticleGun(1);
  fMessenger = new MyPrimaryGeneratorMessenger(this);
  angleDegrees = 50.0;
  angleRadians = angleDegrees * CLHEP::degree;
  particleEnergy = 3.0996;
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
  delete fMessenger;
  delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
  
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName="opticalphoton";
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
  
  std::cout << "generating photon" << std::endl;

  G4double discRadius = 250. * mm; 
  //  G4ThreeVector discPosition(0.0,0.0,100.0*mm);
  G4double randomPhi = G4UniformRand() * 360.0 * deg;

  G4double xShift = (700.0 * mm) * std::tan(angleRadians);
  G4double yShift = 0.0;
  G4double zShift = 1.0*m;

  G4ThreeVector discPosition(xShift, yShift, zShift);

  G4double randomRadius = std::sqrt(G4UniformRand()) * discRadius;
  G4ThreeVector newPosition(discPosition.x() + randomRadius * std::cos(randomPhi), discPosition.y() +  randomRadius * std::sin(randomPhi), discPosition.z());

  //  G4ThreeVector direction = G4ThreeVector(std::sin(angleRadians) * std::cos(randomPhi), std::sin(angleRadians) * std::sin(randomPhi), - std::cos(angleRadians));
  //G4ThreeVector direction(randomRadius * std::cos(randomPhi), randomRadius * std::sin(randomPhi), 100.0 * mm);
  G4ThreeVector direction(0.0,0.0,-1.0);
  direction.rotateY(angleRadians);
  
  // Set particle properties                                                                                               
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(particleEnergy*eV);
  fParticleGun->SetParticlePosition(newPosition);
  fParticleGun->SetParticleMomentumDirection(direction);

  G4PrimaryVertex *vertex = new G4PrimaryVertex(newPosition, 0.0);
  G4PrimaryParticle *primary = new G4PrimaryParticle(particle);

  vertex->SetPrimary(primary);
  G4double checkEnergy = fParticleGun->GetParticleEnergy();
  G4cout << "PARTICLE ENERGY IS:  " << checkEnergy << G4endl;
  G4cout << "GENERATED THE OPTICAL PHOTON!!!" << G4endl;

  fParticleGun->GeneratePrimaryVertex(anEvent);


}
void MyPrimaryGenerator::SetAngle(G4double angle)
{
  angleDegrees = angle;
  angleRadians = angleDegrees * CLHEP::degree;
}

void MyPrimaryGenerator::SetEnergy(G4double energy)
{
  particleEnergy=energy;
}
