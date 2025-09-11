/*
Author:    Mohit Gola 10th July 2023
*/

#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "G4EventManager.hh"
#include "G4UIcmdWithAString.hh"

#include "run.hh"
#include "generator.hh"
#include "mysteppingaction.hh"
#include "event.hh"

#include "G4UIcommand.hh"

#include "globals.hh"

class MyRunAction;

template <class gentype>
class ActionMessenger;

template <class gentype>
class MyActionInitialization : public G4VUserActionInitialization
{
public:
  MyActionInitialization(gentype *_gengen);
  MyActionInitialization(gentype *_gengen, G4String _name_template);
  void SetNameTemplate(G4String new_name);
  void Build() const;

private:
  gentype *generator;
  G4String name_template = "output";

  MyRunAction *runAction;

  ActionMessenger<gentype> *acton;

  //  UserSteppingAction* fSteppingAction;
};

template <class gentype>
class ActionMessenger : public G4UImessenger
{
public:
  ActionMessenger();

  ActionMessenger(MyActionInitialization<gentype> *other);
  void SetNewValue(G4UIcommand *command, G4String newValue);

private:
  MyActionInitialization<gentype> *myactioninit;

  G4UIcmdWithAString *newname;
};

#endif
