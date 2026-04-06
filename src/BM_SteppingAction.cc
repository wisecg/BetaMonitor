// BM_SteppingAction.cc - Implementation of BM_SteppingAction class

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "BM_SteppingAction.hh"
#include "BM_Detector.hh"
#include "BM_Output.hh"
// #include "BM_StepCounter.hh"

BM_SteppingAction *BM_SteppingAction::fgInstance = nullptr;

BM_SteppingAction *BM_SteppingAction::Instance()
{
   return fgInstance;
}

BM_SteppingAction::BM_SteppingAction() : G4UserSteppingAction()
{
   fgInstance = this;
}

BM_SteppingAction::~BM_SteppingAction()
{
   fgInstance = nullptr;
}

void BM_SteppingAction::UserSteppingAction(const G4Step *step)
{
   G4Track* track = step->GetTrack();
   if (track->GetParentID() > 0 && track->GetCreatorProcess()) {
      G4String processname = track->GetCreatorProcess()->GetProcessName();
      // print processname
      // G4cout << "Process name: " << processname << G4endl;
      // only for the first step of the secondary track, which should be right after creation
      if ((processname == "RadioactiveDecay") && track->GetCurrentStepNumber() == 1) {
         G4double decayProductEnergy = track->GetKineticEnergy();
         const G4ParticleDefinition* particle = track->GetParticleDefinition();
         G4ThreeVector position = track->GetPosition();
         G4ThreeVector momentumDirection = track->GetMomentumDirection();
         BM_Output* output = BM_Output::Instance();
         G4int primary_pid = particle->GetPDGEncoding();
         G4int primary_eventid = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
         output->setPrimaryInputParams(
            primary_pid,
            primary_eventid,
            position.x(), position.y(), position.z(),
            momentumDirection.x(), momentumDirection.y(), momentumDirection.z(),
            decayProductEnergy
         );
         output->FillPrimaryInput();
      }
   }

   G4double z = track->GetParticleDefinition()->GetAtomicNumber();
   if (z == 16)
   {
      G4cout << "Killing track for a sulfur ion" << G4endl;
      track->SetTrackStatus(fKillTrackAndSecondaries);
   }
   else if (z == 17)
   {
      G4cout << "Counting chlorine ion step" << G4endl;
   }
   // // BM_StepCounter::Instance()->Increment();
}

void BM_SteppingAction::Reset() {}
