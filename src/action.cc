/*
Author:    Mohit Gola 10th July 2023
*/

#include "action.hh"

template <class gentype>
MyActionInitialization<gentype>::MyActionInitialization(gentype *_gengen)
{
  generator = _gengen;
}

template <class gentype>
MyActionInitialization<gentype>::MyActionInitialization(gentype *_gengen, std::string _name_template)
{
  generator = _gengen;
  name_template = _name_template;
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

  MyRunAction *runAction = new MyRunAction();
  runAction->set_name_template(name_template);
  SetUserAction(runAction);
}

template MyActionInitialization<NewGenerator>::MyActionInitialization(NewGenerator *);
template MyActionInitialization<NewGenerator>::MyActionInitialization(NewGenerator *, std::string);
template void MyActionInitialization<NewGenerator>::Build() const;

template MyActionInitialization<MyPrimaryGenerator>::MyActionInitialization(MyPrimaryGenerator *);
template MyActionInitialization<MyPrimaryGenerator>::MyActionInitialization(MyPrimaryGenerator *, std::string);
template void MyActionInitialization<MyPrimaryGenerator>::Build() const;

template MyActionInitialization<Laser>::MyActionInitialization(Laser *);
template MyActionInitialization<Laser>::MyActionInitialization(Laser *, std::string);
template void MyActionInitialization<Laser>::Build() const;
