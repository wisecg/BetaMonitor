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
   // HC_vac = sdManager->GetCollectionID("Det_vac_HC");

   time_t my_time = time(NULL);
   G4int eventN = event->GetEventID();
   if (eventN % 100000 == 0)
      G4cout << "\n---> Begin event: " << eventN << "  time: " << ctime(&my_time) << G4endl;
}

void BM_EventAction::EvaluateHC(BM_HitsCollection *hc, int det_num)
{
   int n = hc->entries(); // error thrown?

   G4double eDep = 0., eDepPhot = 0., eDepAnn = 0., eDepPos = 0., eDepElec = 0., eDepOther = 0.;
   G4double InEn = 0., finEn = 0., time = 1.e54, depth = 0.;

   G4int pid = 999, pIDNew = 999, pidtemp = 999, pidpre = 999;
   G4int exitPosCounter = 0, exitElecCounter = 0, exitPhotCounter = 0, exitAnnCounter = 0, exitOthCounter = 0, parent = 0;

   G4ThreeVector averagePos(999., 999., 999.);
   G4ThreeVector firstPos(999., 999., 999.);
   G4ThreeVector anniPos(999., 999., 999.);
   G4ThreeVector EscPos(999., 999., 999.);

   G4bool exited = false, exitedPos = false, exitedElec = false, exitedPhot = false, exitedOth = false;

   if (n > 0)
   {
      for (int i = 0; i < n; i++)
      {
         BM_Hit *hit = (*hc)[i];
         pidtemp = hit->pid();
         if (i >= 1)
         {
            BM_Hit *prehit = (*hc)[i - 1];
            pidpre = prehit->pid();
            if (pidtemp == 22)
            {
               if (pidpre == -11)
               {
                  anniPos = hit->position();
               }
            }
         }
         if (hit->time() < time)
         {
            time = hit->time();
            InEn = hit->inEnergy();
            pid = hit->pid();
         }
         G4double z = hit->position().z();

         // Log the depth of hit if deeper than previous hit
         if (z * z > depth * depth)
         {
            depth = z;
         }

         // Accumulate deposited energy and position
         eDep += hit->energyDep();
         averagePos += hit->position() / n;
         
         // If this is the first hit, log the position
         if (i == 0)
         {
            firstPos = hit->position();
         }
         switch (pidtemp)
         {
         case -11:
            eDepPos += hit->energyDep();
            exitedPos = hit->leftVolume();
            if (exitedPos == true)
            {
               exitPosCounter++;
            }
            continue;
         case 11:
            eDepElec += hit->energyDep();
            exitedElec = hit->leftVolume();
            if (exitedElec == true)
            {
               exitElecCounter++;
            }
            continue;
         case 22:
            parent = hit->parentID();
            eDepPhot += hit->energyDep();
            exitedPhot = hit->leftVolume();

            if (parent == 1)
            {
               eDepAnn += hit->energyDep();
               if (exitedPhot == true)
               {
                  exitAnnCounter++;
               }
            }
            if (exitedPhot == true)
            {
               exitPhotCounter++;
            }
            continue;
         default:
            pIDNew = pidtemp;
            eDepOther += hit->energyDep();
            exitedOth = hit->leftVolume();
            if (exitedOth == true)
            {
               exitOthCounter++;
            }
         }
         finEn = hit->energy();
         exited = hit->leftVolume();
         EscPos = hit->position();
      }
   }

   bool pHit = (eDep > 0) ? true : false;
   switch (det_num)
   {
   case 1:
      output->setTrigParams0(pid, eDep, InEn, firstPos.x(), firstPos.y(), firstPos.z());
      // analysisManager->FillNtupleIColumn(0, 0, pHit);  // save this block as an example of how output was handled in the past
      // analysisManager->FillNtupleDColumn(0, 1, eDep);
      // analysisManager->FillNtupleDColumn(0, 2, averagePos.x());
      // analysisManager->FillNtupleDColumn(0, 3, averagePos.y());
      // analysisManager->FillNtupleDColumn(0, 4, averagePos.z());
      // analysisManager->FillNtupleDColumn(0, 5, time);
      // output->setTrigParams(pHit, eDep,
      //               averagePos.x(), averagePos.y(), averagePos.z(),
      //               time);
      break;
   case 2:
      output->setWindParams0(pid, eDep, InEn, firstPos.x(), firstPos.y(), firstPos.z());
      break;
   case 3:
      output->setSQParams0(pid, eDep, InEn, firstPos.x(), firstPos.y(), firstPos.z());
      break;
   case 4:
      output->setVacParams0(pid, eDep, InEn, firstPos.x(), firstPos.y(), firstPos.z());
      break;
   }

   // else if (particleType == "e-")
   // {
   //   bool b1Hit = (eDep > 0)?true:false;
   //   switch(det_num)
   //   {
   //     case 1:
   //     output->setTrigParams(b1Hit, eDep, averagePos.x(), averagePos.y(), averagePos.z(), time, 0., bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
   //     break;
   //     case 2:
   //     output->setWindParams(b1Hit, eDep, averagePos.x(), averagePos.y(), averagePos.z(), time, 0., bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
   //     break;
   //     case 3:
   //     output->setSQParams(b1Hit, eDep, averagePos.x(), averagePos.y(), averagePos.z(), time, 0., bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
   //     break;
   //   }
   // }

   return;
}

void BM_EventAction::EndOfEventAction(const G4Event *event)
{
   // G4cout << "Event step count: " << BM_StepCounter::Instance()->Read() << G4endl;
   
   // auto analysisManager = G4AnalysisManager::Instance();
   G4HCofThisEvent *hce = event->GetHCofThisEvent();
   if (!hce)
   {
      G4cout << "No hits collection of this event found.\n";
      return;
   }
   //  BM_HitsCollection* HC_trig_pvt;
   //  BM_HitsCollection* HC_wind_pvt;
   //  BM_HitsCollection* HC_sq_pvt;

   HC_trig_pvt = static_cast<BM_HitsCollection *>(hce->GetHC(HC_trig));
   HC_wind_pvt = static_cast<BM_HitsCollection *>(hce->GetHC(HC_wind));
   HC_sq_pvt = static_cast<BM_HitsCollection *>(hce->GetHC(HC_sq));

   output = BM_Output::Instance();

   EvaluateHC(HC_trig_pvt, trigger);
   EvaluateHC(HC_sq_pvt, square);
   EvaluateHC(HC_wind_pvt, window);
   // analysisManager->AddNtupleRow();

   output->Fill();
   return;
}

void BM_EventAction::Reset() {}
