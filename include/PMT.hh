// BM_EventAction.hh - Header for BM_EventAction class
#ifndef PMT_HH
#define PMT_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

// #include "BM_Output.hh"

class PMT
{
public:
   PMT();
   virtual ~PMT();
   G4int itt = 1;
   // static access method
   static PMT *Instance();
   double ProcessPMTHits(double en);

private:
   static PMT *Instance_;
};

#endif