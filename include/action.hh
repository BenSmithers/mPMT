/*                                                                                                                        
Author:    Mohit Gola 10th July 2023                                                                                      
*/

#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "G4EventManager.hh"

#include "generator.hh"
#include "mysteppingaction.hh"
#include "event.hh"
#include "run.hh"

template<class gentype>
class MyActionInitialization : public G4VUserActionInitialization
{
  public:

    MyActionInitialization(gentype* _gengen);
    void Build() const;
  
  private:
    gentype* generator; 

    //  UserSteppingAction* fSteppingAction;
};


#endif
