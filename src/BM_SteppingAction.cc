// BM_SteppingAction.cc - Implementation of BM_SteppingAction class

#include "BM_SteppingAction.hh"

#include "BM_Detector.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
// #include "BM_StepCounter.hh"

BM_SteppingAction *BM_SteppingAction::fgInstance = nullptr;

BM_SteppingAction *BM_SteppingAction::Instance()
{
   return fgInstance;
}

BM_SteppingAction::BM_SteppingAction()
    : G4UserSteppingAction()
{
   fgInstance = this;
}

BM_SteppingAction::~BM_SteppingAction()
{
   fgInstance = nullptr;
}

void BM_SteppingAction::UserSteppingAction(const G4Step *step)
{
   G4Track *track = step->GetTrack();
   G4double z = track->GetParticleDefinition()->GetAtomicNumber();
   if (z == 16)
   {
      // G4cout << "Killing track for a sulfur ion" << G4endl;
      track->SetTrackStatus(fKillTrackAndSecondaries);
   }
   else if (z == 17)
   {
      // G4cout << "Counting chlorine ion step" << G4endl;
   }
   // BM_StepCounter::Instance()->Increment();
}

void BM_SteppingAction::Reset() {}
