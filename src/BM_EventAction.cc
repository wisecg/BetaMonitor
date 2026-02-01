// BM_EventAction.cc - Implementation of BM_EventAction class
#include <stdlib.h>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

#include "BM_SD.hh"
#include "BM_RunAction.hh"
#include "BM_Output.hh"
#include "BM_PrimaryGenerator.hh"
#include "BM_EventAction.hh"
// #include "BM_StepCounter.hh"

using std::swap;

BM_EventAction *BM_EventAction::fgInstance = nullptr;

BM_EventAction *BM_EventAction::Instance()
{
   // Static acces function via G4RunManager
   return fgInstance;
}

BM_EventAction::BM_EventAction() : G4UserEventAction()
{
   fgInstance = this;
}

BM_EventAction::~BM_EventAction()
{
   fgInstance = nullptr;
}

void BM_EventAction::BeginOfEventAction(const G4Event *event)
{
   G4SDManager *sdManager = G4SDManager::GetSDMpointer();
   HC_trig = sdManager->GetCollectionID("Det_trig_HC");
   HC_sq = sdManager->GetCollectionID("Det_sq_HC");
   HC_wind = sdManager->GetCollectionID("Det_wind_HC");
   HC_vac = sdManager->GetCollectionID("Det_vac_HC");

   time_t my_time = time(NULL);
   G4int eventN = event->GetEventID();
   if (eventN % 100000 == 0)
      G4cout << "\n---> Begin event: " << eventN << "  time: " << ctime(&my_time) << G4endl;
}

void BM_EventAction::EvaluateHC(BM_HitsCollection *hc, int eventN)
{
   int n = hc->entries();
   for (int i = 0; i < n; i++)
   {
      BM_Hit *hit = (*hc)[i];
      G4int pid_step = hit->pid();
      G4int eventid_step = eventN;
      G4int trackid_step = hit->trackID();
      G4int parentid_step = hit->parentID();
      G4int stepnumber_step = i + 1;
      // G4bool exited_step = hit->leftVolume();
      G4int volumeid_step = hit->id();
      G4double inenergy_step = hit->inEnergy();
      G4double kineticenergy_step = hit->energy();
      G4double depenergy_step = hit->energyDep();
      G4double x_step = hit->position().x();
      G4double y_step = hit->position().y();
      G4double z_step = hit->position().z();
      G4double px_step = hit->momentum().x();
      G4double py_step = hit->momentum().y();
      G4double pz_step = hit->momentum().z();
      output->setParams(pid_step, eventid_step, trackid_step, parentid_step, volumeid_step, 
                        stepnumber_step, inenergy_step, kineticenergy_step, 
                        depenergy_step, x_step, y_step, z_step, px_step, py_step, pz_step);
      output->Fill();
      
   }

   return;
}       

void BM_EventAction::EndOfEventAction(const G4Event *event)
{
   G4HCofThisEvent *hce = event->GetHCofThisEvent();
   if (!hce)
   {
      G4cout << "No hits collection of this event found.\n";
      return;
   }

   HC_trig_pvt = static_cast<BM_HitsCollection *>(hce->GetHC(HC_trig));
   HC_wind_pvt = static_cast<BM_HitsCollection *>(hce->GetHC(HC_wind));
   HC_sq_pvt = static_cast<BM_HitsCollection *>(hce->GetHC(HC_sq));
   HC_vac_pvt = static_cast<BM_HitsCollection *>(hce->GetHC(HC_vac));

   output = BM_Output::Instance();

   G4int eventN = event->GetEventID();

   EvaluateHC(HC_trig_pvt, eventN);
   EvaluateHC(HC_sq_pvt, eventN);
   EvaluateHC(HC_wind_pvt, eventN);
   EvaluateHC(HC_vac_pvt, eventN);

   return;
}

void BM_EventAction::Reset() {}
