/*
Author:    Mohit Gola 10th July 2023
*/

#include "generator.hh"
#include "MyPrimaryGeneratorMessenger.hh"

Laser::Laser()
{
  fParticleGun = new G4ParticleGun(1);
  fMessenger = new MyPrimaryGeneratorMessenger(this);
  angleDegrees = 50.0;
  angleRadians = angleDegrees * CLHEP::degree;
  particleEnergy = 3.0996;
}

void Laser::SetDiscRad(G4double disc_rad)
{
  discRadius = disc_rad;
}

void Laser::SetAngle(G4double angle)
{
  angleDegrees = angle;
  angleRadians = angleDegrees * CLHEP::degree;
}

void Laser::SetEnergy(G4double energy)
{
  particleEnergy = energy;
}

void Laser::SetX(G4double xpos)
{
  this->xpos = xpos;
}

void Laser::SetY(G4double ypos)
{
  this->ypos = ypos;
}

void Laser::SetSpread(G4double spread)
{
  this->spread = spread;
}

void Laser::GeneratePrimaries(G4Event *anEvent)
{

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName = "opticalphoton";
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

  G4ThreeVector initialPosition(xpos, ypos, 500.0 * mm);
  // Define the target Z coordinate
  double targetZ = 44.40 * mm;

  // Calculate the difference in Z coordinates
  double deltaZ = targetZ - initialPosition.z();

  // Calculate the new Y coordinate based on the angle and deltaZ
  double newY = initialPosition.y() + deltaZ * std::tan(angleRadians);

  // Create the new position vector
  // G4ThreeVector newPosition(initialPosition.x(), newY, initialPosition.z());

  G4double random_azi = 2 * M_PI * G4UniformRand();
  G4double random_zen = G4RandGauss::shoot(0.0, spread * M_PI / 180.0);

  // Calculate the direction vector towards the point (0, 0, 35.903)
  // G4ThreeVector direction = (G4ThreeVector(0.0, 0.0, targetZ) - newPosition).unit();
  G4ThreeVector direction = G4ThreeVector(cos(random_azi) * sin(random_zen),
                                          sin(random_azi) * sin(random_zen),
                                          -cos(random_zen));
  // Print the new position and direction

  G4double thisen = 2.481 * eV;

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(direction);
  fParticleGun->SetParticleEnergy(thisen);
  fParticleGun->SetParticlePosition(initialPosition);
  fParticleGun->SetParticlePolarization(G4ThreeVector(0, 1.0, 0));

  G4PrimaryVertex *vertex = new G4PrimaryVertex(initialPosition, 0.0);
  G4PrimaryParticle *primary = new G4PrimaryParticle(particle);

  G4ThreeVector momentun = direction * thisen;

  vertex->SetPrimary(primary);
  G4double checkEnergy = fParticleGun->GetParticleEnergy();

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

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
  G4String particleName = "opticalphoton";
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

  //  G4ThreeVector discPosition(0.0,0.0,100.0*mm);
  G4double randomPhi = G4UniformRand() * 360.0 * deg;

  G4double xShift = 100 * mm * std::tan(angleRadians);
  G4double yShift = 0.0;
  G4double zShift = 100 * mm;

  G4ThreeVector discPosition(xShift, yShift, zShift);

  G4double randomRadius = std::sqrt(G4UniformRand()) * discRadius;
  G4ThreeVector newPosition(discPosition.x() + randomRadius * std::cos(randomPhi), discPosition.y() + randomRadius * std::sin(randomPhi), discPosition.z());

  //  G4ThreeVector direction = G4ThreeVector(std::sin(angleRadians) * std::cos(randomPhi), std::sin(angleRadians) * std::sin(randomPhi), - std::cos(angleRadians));
  // G4ThreeVector direction(randomRadius * std::cos(randomPhi), randomRadius * std::sin(randomPhi), 100.0 * mm);
  G4ThreeVector direction(0.0, 0.0, -1.0);
  direction.rotateY(angleRadians);

  // Set particle properties
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(particleEnergy * eV);
  fParticleGun->SetParticlePosition(newPosition);
  fParticleGun->SetParticleMomentumDirection(direction);

  G4PrimaryVertex *vertex = new G4PrimaryVertex(newPosition, 0.0);
  G4PrimaryParticle *primary = new G4PrimaryParticle(particle);

  vertex->SetPrimary(primary);
  G4double checkEnergy = fParticleGun->GetParticleEnergy();

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

void MyPrimaryGenerator::SetDiscRad(G4double disc_rad)
{
  discRadius = disc_rad;
}

void MyPrimaryGenerator::SetAngle(G4double angle)
{
  angleDegrees = angle;
  angleRadians = angleDegrees * CLHEP::degree;
}

void MyPrimaryGenerator::SetEnergy(G4double energy)
{
  particleEnergy = energy;
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
  G4String particleName = "opticalphoton";
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

  G4double target = 12 * mm;
  G4double distance = 2.5 * m;

  //  G4ThreeVector discPosition(0.0,0.0,100.0*mm);
  G4double alpha = asin(sin(angleRadians) * target / distance);
  G4double beta = angleRadians - alpha;

  G4double randomPhi = G4UniformRand() * 360.0 * deg;
  G4double uniform = G4UniformRand() + G4UniformRand();
  G4double rad = (uniform > 1. ? 2. - uniform : uniform) * discRadius;

  // position.rotateY(angleRadians);

  G4double xShift = distance * sin(beta); // * std::tan(beta);
  G4double yShift = 0.0;
  G4double zShift = distance * cos(beta);

  G4ThreeVector discPosition(xShift, yShift, zShift);

  G4ThreeVector position(rad * cos(randomPhi), rad * sin(randomPhi), 0.0);
  position.rotateY(angleRadians);

  // G4ThreeVector newPosition(discPosition.x() + randomRadius * std::cos(randomPhi), discPosition.y() +  randomRadius * std::sin(randomPhi), discPosition.z());
  G4ThreeVector newPosition = discPosition + position;

  // newPosition.rotateY(angleRadians);

  //  G4ThreeVector direction = G4ThreeVector(std::sin(angleRadians) * std::cos(randomPhi), std::sin(angleRadians) * std::sin(randomPhi), - std::cos(angleRadians));
  // G4ThreeVector direction(randomRadius * std::cos(randomPhi), randomRadius * std::sin(randomPhi), 100.0 * mm);
  G4ThreeVector direction(0.0, 0.0, -1.0);
  direction.rotateY(angleRadians);

  // Set particle properties
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(particleEnergy * eV);
  fParticleGun->SetParticlePosition(newPosition);
  fParticleGun->SetParticleMomentumDirection(direction);

  G4PrimaryVertex *vertex = new G4PrimaryVertex(newPosition, 0.0);
  G4PrimaryParticle *primary = new G4PrimaryParticle(particle);

  vertex->SetPrimary(primary);
  G4double checkEnergy = fParticleGun->GetParticleEnergy();

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

void NewGenerator::SetDiscRad(G4double disc_rad)
{
  discRadius = disc_rad;
}

void NewGenerator::SetAngle(G4double angle)
{
  angleDegrees = angle;
  angleRadians = angleDegrees * CLHEP::degree;
}

void NewGenerator::SetEnergy(G4double energy)
{
  particleEnergy = energy;
}
