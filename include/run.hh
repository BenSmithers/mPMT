/*
Author:    Mohit Gola 10th July 2023
*/

#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include <G4AnalysisManager.hh>
#include "G4RunManager.hh"

#include "event.hh"

class MySteppingAction;

class MyRunAction : public G4UserRunAction
{
public:
  MyRunAction();
  ~MyRunAction();

  void set_name_template(std::string what) { name_template = what; }

  virtual void BeginOfRunAction(const G4Run *);
  virtual void EndOfRunAction(const G4Run *);

private:
  std::string name_template = "output";
  G4int TotalNumAbsorbed;
  G4int TotalNumReflected;
  G4int TotalNumTransmitted;
};

#endif
