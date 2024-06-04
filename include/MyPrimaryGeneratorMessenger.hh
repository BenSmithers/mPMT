/*
Author:    Mohit Gola 10th July 2023
*/

#ifndef MYPRIMGENMESS_H
#define MYPRIMGENMESS_H

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

template <class gentype>
class MyPrimaryGeneratorMessenger : public G4UImessenger
{
public:
  MyPrimaryGeneratorMessenger(gentype *generator);

  void SetNewValue(G4UIcommand *command, G4String newValue);

private:
  G4UIdirectory *fGeneratorDir;
  G4UIcommand *fSetAngleCmd;
  G4UIcommand *fSetEnergyCmd;
  G4UIcommand *fSetDiscCmd;
  G4UIcommand *fSetXCmd;
  G4UIcommand *fSetYCmd;
  G4UIcommand *fSetSpreadCmd;
  gentype *fGenerator;
};

#endif