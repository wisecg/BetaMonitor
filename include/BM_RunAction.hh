/// \file BM_RunAction.hh
/// \brief Definition of the BM_RunAction class

#ifndef BM_RUNACTION_HH
#define BM_RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class BM_RunAction : public G4UserRunAction
{
public:
   BM_RunAction();
   virtual ~BM_RunAction();

   virtual void BeginOfRunAction(const G4Run *);
   virtual void EndOfRunAction(const G4Run *);
};

#endif // BM_RUNACTION_HH
