// BM_EventAction.hh - Header for BM_EventAction class

#ifndef B1EVENTACTION_HH
#define B1EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "BM_Hit.hh"
#include "BM_Output.hh"

class B1SteppingAction;

class BM_EventAction : public G4UserEventAction
{
public:
   BM_EventAction();
   virtual ~BM_EventAction();
   // static access method
   static BM_EventAction *Instance();
   BM_HitsCollection *HC_bScintillator_pvt;
   BM_HitsCollection *HC_windowFoil_pvt;
   BM_HitsCollection *HC_aScintillator_pvt;
   BM_HitsCollection *HC_vacuum_pvt;
   virtual void BeginOfEventAction(const G4Event *event);
   virtual void EndOfEventAction(const G4Event *event);

   void Reset();

private:
   void EvaluateHC(BM_HitsCollection *hits, int det_num);
   static BM_EventAction *fgInstance;
   G4int HC_bScintillator;
   G4int HC_windowFoil;
   G4int HC_aScintillator;
   G4int HC_vacuum;
   BM_Output *output;
};

#endif // B1EVENTACTION_HH
