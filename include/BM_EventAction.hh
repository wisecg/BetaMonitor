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
   G4int trigger = 1;
   G4int square = 3;
   G4int window = 2;
   G4int vac = 4;
   // static access method
   static BM_EventAction *Instance();
   BM_HitsCollection *HC_trig_pvt;
   BM_HitsCollection *HC_wind_pvt;
   BM_HitsCollection *HC_sq_pvt;
   BM_HitsCollection *HC_vac_pvt;
   virtual void BeginOfEventAction(const G4Event *event);
   virtual void EndOfEventAction(const G4Event *event);

   void Reset();

private:
   void EvaluateHC(BM_HitsCollection *hits, int det_num); //, G4String particleType);
   static BM_EventAction *fgInstance;
   G4int HC_trig;
   G4int HC_wind;
   G4int HC_sq;
   G4int HC_vac;
   BM_Output *output;
};

#endif // B1EVENTACTION_HH
