// BM_EventAction.cc - Implementation of BM_EventAction class
#include <stdlib.h>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <set>
#include <vector>

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

   G4double min_time, max_time;

   G4int pid_hit;
   G4int eventid_hit;
   G4int trackid_hit;
   G4int parentid_hit;
   G4bool exited_hit;
   G4int volumeid_hit;
   G4double inenergy_hit;
   G4double kineticenergy_hit;
   G4double depenergy_hit = 0.0;
   G4double primaryenergy_hit = -999;
   G4double x_hit;
   G4double y_hit;
   G4double z_hit;
   G4double px_hit;
   G4double py_hit;
   G4double pz_hit;

   std::set<G4int> uniqueTrackIDs;

   int n = hc->entries();
   if (n == 0)
      return;
   
   // Loop through hits collection and get unique trackIDs
   for (int i = 0; i < n; i++)
   {
      BM_Hit *hit = (*hc)[i];
      uniqueTrackIDs.insert(hit->trackID());
   }

   // Loop through unique trackIDs and get hit information for each trackID (trackwise logging)
   for (auto trackID : uniqueTrackIDs)
   {
      trackid_hit = trackID;
      min_time = 1.e54;
      max_time = 0.0;
      depenergy_hit = 0.0;

      for (int i = 0; i < n; i++)
      {
         BM_Hit *hit = (*hc)[i];
         if (hit->trackID() == trackID)
         {
            depenergy_hit += hit->energyDep(); // sum deposited energy across all hits for a given trackID.
            if (hit->time() < min_time) //gets earliest hit for the variables contained below.
            {
               min_time = hit->time();
               pid_hit = hit->pid();
               eventid_hit = eventN;
               parentid_hit = hit->parentID();
               volumeid_hit = hit->id();
               inenergy_hit = hit->inEnergy();
               primaryenergy_hit = hit->primaryEnergy();
               x_hit = hit->position().x();
               y_hit = hit->position().y();
               z_hit = hit->position().z();
               px_hit = hit->momentum().x();
               py_hit = hit->momentum().y();
               pz_hit = hit->momentum().z();
            }

            if (hit->time() > max_time)
               max_time = hit->time();
               kineticenergy_hit = hit->energy();
               exited_hit = hit->exited();
         }
      }
      output->setParams(pid_hit, eventid_hit, trackid_hit, parentid_hit, volumeid_hit, 
                        primaryenergy_hit, inenergy_hit, kineticenergy_hit, 
                        depenergy_hit, exited_hit, x_hit, y_hit, z_hit, px_hit, py_hit, pz_hit);
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
