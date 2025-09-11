/*
Author:    Mohit Gola 10th July 2023
*/

#include "action.hh"

template <class gentype>
MyActionInitialization<gentype>::MyActionInitialization(gentype *_gengen)
{
  generator = _gengen;
  acton = new ActionMessenger<gentype>(this);
  runAction = new MyRunAction();
}

template <class gentype>
MyActionInitialization<gentype>::MyActionInitialization(gentype *_gengen, G4String _name_template)
{
  generator = _gengen;
  name_template = _name_template;
  acton = new ActionMessenger<gentype>(this);
  runAction = new MyRunAction();
}

template <class gentype>
void MyActionInitialization<gentype>::Build() const
{
  SetUserAction(generator);

  MyEventAction *eventAction = new MyEventAction();
  MySteppingAction *steepingAction = new MySteppingAction();

  steepingAction->SetEventAction(eventAction);
  eventAction->SetSteppingAction(steepingAction);

  G4EventManager::GetEventManager()->SetUserAction(eventAction);

  G4RunManager *runManager = G4RunManager::GetRunManager();
  runManager->SetUserAction(steepingAction);
  runManager->SetUserAction(eventAction);

  runAction->set_name_template(name_template);
  SetUserAction(runAction);
}

template <class gentype>
void MyActionInitialization<gentype>::SetNameTemplate(G4String other)
{
  runAction->set_name_template(other);
}

template <class gentype>
ActionMessenger<gentype>::ActionMessenger(MyActionInitialization<gentype> *other) : myactioninit(other)
{
  newname = new G4UIcmdWithAString("/action/name_template", this);
  newname->SetGuidance("Set the template outfile name");
  newname->SetParameterName("newname", true);
  newname->SetDefaultValue("output2");
}

template <class gentype>
void ActionMessenger<gentype>::SetNewValue(G4UIcommand *command, G4String newValue)
{
  if (command == newname)
  {
    myactioninit->SetNameTemplate(newValue);
    printf("Chaning output file name");
    // printf("Setting output file to %s", newValue);
  }
}

template ActionMessenger<NewGenerator>::ActionMessenger(MyActionInitialization<NewGenerator> *other);
template void ActionMessenger<NewGenerator>::SetNewValue(G4UIcommand *command, G4String newValue);

template ActionMessenger<MyPrimaryGenerator>::ActionMessenger(MyActionInitialization<MyPrimaryGenerator> *other);
template void ActionMessenger<MyPrimaryGenerator>::SetNewValue(G4UIcommand *command, G4String newValue);

template ActionMessenger<Laser>::ActionMessenger(MyActionInitialization<Laser> *other);
template void ActionMessenger<Laser>::SetNewValue(G4UIcommand *command, G4String newValue);

template ActionMessenger<PrecisionGen>::ActionMessenger(MyActionInitialization<PrecisionGen> *other);
template void ActionMessenger<PrecisionGen>::SetNewValue(G4UIcommand *command, G4String newValue);

template MyActionInitialization<NewGenerator>::MyActionInitialization(NewGenerator *);
template MyActionInitialization<NewGenerator>::MyActionInitialization(NewGenerator *, G4String);
template void MyActionInitialization<NewGenerator>::Build() const;
template void MyActionInitialization<NewGenerator>::SetNameTemplate(G4String);

template MyActionInitialization<MyPrimaryGenerator>::MyActionInitialization(MyPrimaryGenerator *);
template MyActionInitialization<MyPrimaryGenerator>::MyActionInitialization(MyPrimaryGenerator *, G4String);
template void MyActionInitialization<MyPrimaryGenerator>::Build() const;
template void MyActionInitialization<MyPrimaryGenerator>::SetNameTemplate(G4String);

template MyActionInitialization<Laser>::MyActionInitialization(Laser *);
template MyActionInitialization<Laser>::MyActionInitialization(Laser *, G4String);
template void MyActionInitialization<Laser>::Build() const;
template void MyActionInitialization<Laser>::SetNameTemplate(G4String);

template MyActionInitialization<PrecisionGen>::MyActionInitialization(PrecisionGen *);
template MyActionInitialization<PrecisionGen>::MyActionInitialization(PrecisionGen *, G4String);
template void MyActionInitialization<PrecisionGen>::Build() const;
template void MyActionInitialization<PrecisionGen>::SetNameTemplate(G4String);