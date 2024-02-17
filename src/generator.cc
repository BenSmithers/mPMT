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

  //  G4ThreeVector discPosition(0.0,0.0,100.0*mm);
  G4double randomPhi = G4UniformRand() * 360.0 * deg;

  G4double xShift = 1.0*m * std::tan(angleRadians);
  G4double yShift = 0.0;
  G4double zShift = 1*m;

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

void MyPrimaryGenerator::SetDiscRad(G4double disc_rad){
  discRadius = disc_rad; 
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


NewGenerator::NewGenerator()
{
  fParticleGun = new G4ParticleGun(1);
  fMessenger = new MyPrimaryGeneratorMessenger<NewGenerator>(this);
  angleDegrees = 50.0;
  angleRadians = angleDegrees * CLHEP::degree;
  particleEnergy = 3.0996;
}


void NewGenerator::GeneratePrimaries(G4Event *anEvent)
{
  
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName="opticalphoton";
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
  
  std::cout << "generating photon" << std::endl;

  G4double target = 250*mm;
  G4double distance = 2.5*m;

  //  G4ThreeVector discPosition(0.0,0.0,100.0*mm);
  G4double alpha = asin(sin(angleRadians)*target/distance);
  G4double beta = angleRadians - alpha;

  G4double randomPhi = G4UniformRand() * 360.0 * deg;
  G4double uniform = G4UniformRand() + G4UniformRand();
  G4double rad = (uniform>1.?2.-uniform:uniform)*discRadius;

  //position.rotateY(angleRadians);

  G4double xShift = distance*sin(beta); // * std::tan(beta);
  G4double yShift = 0.0;
  G4double zShift = distance*cos(beta);

  G4ThreeVector discPosition(xShift, yShift, zShift);

  G4ThreeVector position(rad*cos(randomPhi), rad*sin(randomPhi), 0.0);
  position.rotateY(angleRadians);

  //G4ThreeVector newPosition(discPosition.x() + randomRadius * std::cos(randomPhi), discPosition.y() +  randomRadius * std::sin(randomPhi), discPosition.z());
  G4ThreeVector newPosition = discPosition + position;

  //newPosition.rotateY(angleRadians);

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

void NewGenerator::SetDiscRad(G4double disc_rad){
  discRadius = disc_rad; 
}

void NewGenerator::SetAngle(G4double angle)
{
  angleDegrees = angle;
  angleRadians = angleDegrees * CLHEP::degree;
}

void NewGenerator::SetEnergy(G4double energy)
{
  particleEnergy=energy;
}
