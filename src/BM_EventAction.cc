// BM_EventAction.cc - Implementation of BM_EventAction class

#include "BM_EventAction.hh"

#include <stdlib.h>
#include <utility>

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

#include "BM_SD.hh"
#include "BM_RunAction.hh"
#include "BM_Output.hh"
#include "BM_PrimaryGenerator.hh"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include "g4root.hh"

// #include "BM_StepCounter.hh"

using std::swap;
// public:{
// int trigger = 1;
// int square = 3;
// int window = 2;
// }

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
      G4cout << "\n---> Begin of event: " << eventN << ctime(&my_time) << G4endl;
}

void BM_EventAction::EvaluateHC(BM_HitsCollection *hc, int det_num) //, G4String particleType)
{

   int n = hc->entries(); // error thrown?
   G4double eDep = 0.;
   G4double eDepPhot = 0.;
   G4double eDepAnn = 0.;
   G4double eDepPos = 0.;
   G4double eDepElec = 0.;
   G4double eDepOther = 0.;
   G4double InEn = 0.;
   G4double finEn = 0.;
   G4double bsEDep = 0.;
   G4double time = 1.e54;
   G4double depth = 0.;
   G4int pid = 999;
   G4int pIDNew = 999;
   G4int pidtemp = 999;
   G4int pidpre = 999;
   G4ThreeVector averagePos(999., 999., 999.);
   G4ThreeVector firstPos(999., 999., 999.);
   G4ThreeVector anniPos(999., 999., 999.);
   G4ThreeVector EscPos(999., 999., 999.);
   G4bool exited = false;
   G4bool exitedPos = false;
   G4bool exitedElec = false;
   G4bool exitedPhot = false;
   G4bool exitedOth = false;
   G4int exitPosCounter = 0;
   G4int exitElecCounter = 0;
   G4int exitPhotCounter = 0;
   G4int exitAnnCounter = 0;
   G4int exitOthCounter = 0;
   G4int parent = 0;
   // G4ThreeVector bsPos(0., 0., 0.);
   // G4int bsStep = -1;
   // G4double zmom = 0.;
   // G4double bsTime = 1.e54;
   // G4double hit2 = 1;
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

         // Log the depth of hit if deeper
         // than  previous hit
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

   G4int threadnum = G4Threading::G4GetThreadId();
   //  G4cout << threadnum <<" + " << det_num <<G4endl;
   std::this_thread::sleep_for(std::chrono::nanoseconds{5000 * threadnum + threadnum}); //* threadnum*16+threadnum
                                                                                        //  G4cout << threadnum <<" wait complete "<<G4endl;
   bool pHit = (eDep > 0) ? true : false;
   switch (det_num)
   {
   case 1:
      switch (threadnum)
      {
      case 0:
         output->setTrigParams0(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 1:
         output->setTrigParams1(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 2:
         output->setTrigParams2(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 3:
         output->setTrigParams3(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime
         break;
      case 4:
         output->setTrigParams4(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 5:
         output->setTrigParams5(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 6:
         output->setTrigParams6(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 7:
         output->setTrigParams7(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 8:
         output->setTrigParams8(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 9:
         output->setTrigParams9(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 10:
         output->setTrigParams10(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 11:
         output->setTrigParams11(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 12:
         output->setTrigParams12(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 13:
         output->setTrigParams13(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 14:
         output->setTrigParams14(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 15:
         output->setTrigParams15(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      }
      // analysisManager->FillNtupleIColumn(0, 0, pHit);
      // analysisManager->FillNtupleDColumn(0, 1, eDep);
      //  G4cout << "eDep " << eDep << G4endl;
      // analysisManager->FillNtupleDColumn(0, 2, averagePos.x());
      // analysisManager->FillNtupleDColumn(0, 3, averagePos.y());
      // analysisManager->FillNtupleDColumn(0, 4, averagePos.z());
      // analysisManager->FillNtupleDColumn(0, 5, time);
      // output->setTrigParams(pHit, eDep,
      //               averagePos.x(), averagePos.y(), averagePos.z(),
      //               time);//, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
      break;
   case 2:
      switch (threadnum)
      {
      case 0:
         output->setWindParams0(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 1:
         output->setWindParams1(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 2:
         output->setWindParams2(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 3:
         output->setWindParams3(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime
         break;
      case 4:
         output->setWindParams4(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 5:
         output->setWindParams5(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 6:
         output->setWindParams6(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 7:
         output->setWindParams7(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 8:
         output->setWindParams8(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 9:
         output->setWindParams9(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 10:
         output->setWindParams10(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 11:
         output->setWindParams11(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 12:
         output->setWindParams12(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 13:
         output->setWindParams13(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 14:
         output->setWindParams14(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 15:
         output->setWindParams15(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      }
      // analysisManager->FillNtupleIColumn(0, 6, pHit);
      // analysisManager->FillNtupleDColumn(0, 7, eDep);
      // G4cout << "eDep " << eDep << G4endl;
      // analysisManager->FillNtupleDColumn(0, 8, averagePos.x());
      // analysisManager->FillNtupleDColumn(0, 9, averagePos.y());
      // analysisManager->FillNtupleDColumn(0, 10, averagePos.z());
      // analysisManager->FillNtupleDColumn(0, 11, time);
      // output->setWindParams(pHit, eDep,
      //               averagePos.x(), averagePos.y(), averagePos.z(),
      //               time);//, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
      break;
   case 3:
      switch (threadnum)
      {
      case 0:
         output->setSQParams0(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bsTime);
         break;
      case 1:
         output->setSQParams1(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 2:
         output->setSQParams2(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 3:
         output->setSQParams3(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime
         break;
      case 4:
         output->setSQParams4(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 5:
         output->setSQParams5(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 6:
         output->setSQParams6(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 7:
         output->setSQParams7(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 8:
         output->setSQParams8(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 9:
         output->setSQParams9(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                              averagePos.x(), averagePos.y(), averagePos.z(),
                              time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 10:
         output->setSQParams10(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 11:
         output->setSQParams11(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 12:
         output->setSQParams12(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 13:
         output->setSQParams13(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 14:
         output->setSQParams14(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 15:
         output->setSQParams15(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      }
   case 4:
      switch (threadnum)
      {
      case 0:
         output->setVacParams0(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 1:
         output->setVacParams1(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 2:
         output->setVacParams2(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 3:
         output->setVacParams3(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime
         break;
      case 4:
         output->setVacParams4(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 5:
         output->setVacParams5(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 6:
         output->setVacParams6(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                               averagePos.x(), averagePos.y(), averagePos.z(),
                               time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 7:
         output->setTrigParams7(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 8:
         output->setTrigParams8(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 9:
         output->setTrigParams9(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                averagePos.x(), averagePos.y(), averagePos.z(),
                                time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 10:
         output->setTrigParams10(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 11:
         output->setTrigParams11(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 12:
         output->setTrigParams12(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 13:
         output->setTrigParams13(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 14:
         output->setTrigParams14(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      case 15:
         output->setTrigParams15(pHit, pid, pIDNew, eDep, eDepPhot, eDepPos, eDepElec, eDepOther, eDepAnn, exitAnnCounter,
                                 averagePos.x(), averagePos.y(), averagePos.z(),
                                 time, InEn, firstPos.x(), firstPos.y(), firstPos.z(), anniPos.x(), anniPos.y(), anniPos.z(), EscPos.x(), EscPos.y(), EscPos.z(), finEn, exited, exitPhotCounter, exitPosCounter, exitElecCounter, exitOthCounter); //, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
         break;
      }
      // analysisManager->FillNtupleIColumn(0, 12, pHit);
      // analysisManager->FillNtupleDColumn(0, 13, eDep);
      // G4cout << "eDep " << eDep << G4endl;
      // analysisManager->FillNtupleDColumn(0, 14, averagePos.x());
      // analysisManager->FillNtupleDColumn(0, 15, averagePos.y());
      // analysisManager->FillNtupleDColumn(0, 16, averagePos.z());
      // analysisManager->FillNtupleDColumn(0, 17, time);
      // output->setSQParams(pHit, eDep,
      //               averagePos.x(), averagePos.y(), averagePos.z(),
      //               time);//, bs, bsPos.x(), bsPos.y(), bsPos.z(), bsTime);
      // break;
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
   // G4cout << "analyzing hits" << G4endl;
   // G4cout << "this event step count: " << BM_StepCounter::Instance()->Read() << G4endl;
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

   // G4int threadtemp = G4Threading::G4GetThreadId();

   output = BM_Output::Instance();
   // G4cout << "event 1" << G4endl;
   EvaluateHC(HC_trig_pvt, trigger); //, "nan");//error
   EvaluateHC(HC_sq_pvt, square);    //, "nan");
   EvaluateHC(HC_wind_pvt, window);  //, "nan");
   // G4cout << "event 2" << G4endl;
   // analysisManager->AddNtupleRow();
   output->Fill();
   return;
}

void BM_EventAction::Reset() {}
