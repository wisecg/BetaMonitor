#include "ActionInitialization.hh"
#include "BM_PrimaryGenerator.hh"
#include "BM_RunAction.hh"
#include "BM_EventAction.hh"
#include "BM_SteppingAction.hh"

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new BM_RunAction());
}


void ActionInitialization::Build() const
{
  SetUserAction(new BM_PrimaryGenerator());
  SetUserAction(new BM_EventAction());
  SetUserAction(new BM_SteppingAction());
}

