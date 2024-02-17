/*                                                                                                                         
Author:    Mohit Gola 10th July 2023                                                                                       
*/

#include "action.hh"


template<class gentype>
MyActionInitialization<gentype>::MyActionInitialization(gentype* _gengen){
  generator = _gengen;
}

template<class gentype>
void MyActionInitialization<gentype>::Build() const
{
  SetUserAction(generator);

  MyEventAction *eventAction = new MyEventAction();
  MySteppingAction *steepingAction = new MySteppingAction();

  steepingAction->SetEventAction(eventAction);
  eventAction->SetSteppingAction(steepingAction);

  G4EventManager::GetEventManager()->SetUserAction(eventAction);

  G4RunManager* runManager = G4RunManager::GetRunManager();
  runManager->SetUserAction(steepingAction);
  runManager->SetUserAction(eventAction);

  MyRunAction *runAction = new MyRunAction();
  SetUserAction(runAction);
}


template MyActionInitialization<NewGenerator>::MyActionInitialization(NewGenerator*);
template void MyActionInitialization<NewGenerator>::Build() const;

template MyActionInitialization<MyPrimaryGenerator>::MyActionInitialization(MyPrimaryGenerator*);
template void MyActionInitialization<MyPrimaryGenerator>::Build() const;
