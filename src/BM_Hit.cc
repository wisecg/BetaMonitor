// BM_Hit.cc - Implementation of BM_Hit class: BetaMon sensitive detector hit

#include "BM_Hit.hh"

#include <cmath>

#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<BM_Hit> *BM_HitAllocator;

BM_Hit::BM_Hit(G4int id, G4int pid, G4double myTime,
               G4ThreeVector position, G4double primaryEnergy, G4double energy, G4double energyDep, G4ThreeVector momentum,
               G4bool exited, G4double inEnergy, G4int parentID, G4int trackID, G4double steplen) : G4VHit()
{
   id_ = id;
   pid_ = pid;
   time_ = myTime;
   position_ = position;
   inEnergy_ = inEnergy;
   energy_ = energy;
   energyDep_ = energyDep;
   exited_ = exited;
   parentid_ = parentID;
   trackid_ = trackID;
   momentum_ = momentum;
   primaryEnergy_ = primaryEnergy;
   steplen_ = steplen;
}

BM_Hit::~BM_Hit() {}

void BM_Hit::Print()
{
   //    // G4String partType = "none";
   //    // if ((940 > mass_ / MeV) && (mass_ / MeV > 938))
   //    //    partType = "proton";
   //    // else if (mass_ / MeV > 0.510)
   //    //    partType = "beta";

   //    // G4cout <<
   //    //  //  "\nparticle = " << partType <<
   //    //    "\ntime = " << time_ / ns <<
   //    //    "\nposition (cm) = " << position_ / cm <<
   //    //   // "\ntheta (rad) = " << theta_ <<
   //    //    //"\nmass = " << mass_ / MeV <<
   //    //    "\nenergy_before = " << (energy_ + energyDep_) / MeV <<
   //    //    "\nenergyDep = " << energyDep_ / MeV << "\n" << G4endl;
   return;
}