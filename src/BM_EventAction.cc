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
   stepwise = false; // default value
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
   // use the class member variable stepwise

   G4double eDep = 0., eDepPhot = 0., eDepAnn = 0., eDepPos = 0., eDepElec = 0., eDepOther = 0.;
   G4double InEn = 0., finEn = 0., time = 1.e54, depth = 0.;

   G4int pid = 999, pIDNew = 999, pidtemp = 999, pidpre = 999;
   G4int exitPosCounter = 0, exitElecCounter = 0, exitPhotCounter = 0, exitAnnCounter = 0, exitOthCounter = 0, parent = 0;

   G4ThreeVector averagePos(999., 999., 999.);
   G4ThreeVector firstPos(999., 999., 999.);
   G4ThreeVector anniPos(999., 999., 999.);
   G4ThreeVector EscPos(999., 999., 999.);

   G4bool exited = false, exitedPos = false, exitedElec = false, exitedPhot = false, exitedOth = false;

   G4int pid_step;
   G4int eventid_step;
   G4int trackid_step;
   G4int parentid_step;
   G4int stepnumber_step;
   // G4bool exited_step = hit->leftVolume();
   G4int volumeid_step;
   G4double inenergy_step;
   G4double kineticenergy_step;
   G4double depenergy_step = 0.0;
   G4double x_step;
   G4double y_step;
   G4double z_step;
   G4double px_step;
   G4double py_step;
   G4double pz_step;

   // 22 photon, 11 electron, -11 positron
   int n = hc->entries();
   if (n == 0)
      return;

   if (stepwise == false)
   {
      for (int i = 0; i < n; i++)
      {
         BM_Hit *hit = (*hc)[i];
         // if (!hit) continue; // skip null pointer
         if (hit->time() < time)
         {
            time = hit->time();
            pid_step = hit->pid();
            eventid_step = eventN;
            trackid_step = hit->trackID();
            parentid_step = hit->parentID();
            volumeid_step = hit->id();
            trackid_step = hit->trackID();
            parentid_step = hit->parentID();
            inenergy_step = hit->inEnergy();

            x_step = hit->position().x();
            y_step = hit->position().y();
            z_step = hit->position().z();
            px_step = hit->momentum().x();
            py_step = hit->momentum().y();
            pz_step = hit->momentum().z();
         }

         // Accumulate deposited energy and position
         depenergy_step += hit->energyDep();
         // averagePos += hit->position() / n;

         // kinetic energy of last hit
         kineticenergy_step = hit->energy();
         stepnumber_step = i + 1;
      }
      output->setParams(pid_step, eventid_step, trackid_step, parentid_step, volumeid_step, 
                           stepnumber_step, inenergy_step, kineticenergy_step, 
                           depenergy_step, x_step, y_step, z_step, px_step, py_step, pz_step);
      output->Fill();
   }
   else
   {
      for (int i = 0; i < n; i++)
      {
         BM_Hit *hit = (*hc)[i];
         // if (!hit) continue; // skip null pointer
         pid_step = hit->pid();
         eventid_step = eventN;
         trackid_step = hit->trackID();
         parentid_step = hit->parentID();
         stepnumber_step = i + 1;
         // G4bool exited_step = hit->leftVolume();
         volumeid_step = hit->id();
         inenergy_step = hit->inEnergy();
         kineticenergy_step = hit->energy();
         depenergy_step = hit->energyDep();
         x_step = hit->position().x();
         y_step = hit->position().y();
         z_step = hit->position().z();
         px_step = hit->momentum().x();
         py_step = hit->momentum().y();
         pz_step = hit->momentum().z();
         output->setParams(pid_step, eventid_step, trackid_step, parentid_step, volumeid_step, 
                           stepnumber_step, inenergy_step, kineticenergy_step, 
                           depenergy_step, x_step, y_step, z_step, px_step, py_step, pz_step);
         output->Fill();
      }     
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
