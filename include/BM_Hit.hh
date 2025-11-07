// BM_Hit.hh - Header file for BM_Hit class: BetaMon hit for sensitive detectors

#ifndef BM_HIT_HH
#define BM_HIT_HH

#include "nat_units.hh"

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4VHitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"

class BM_Hit : public G4VHit
{
public:                        // Constructor
   BM_Hit(G4int id, G4int pid, // G4double mass, G4int charge,
          G4double time, G4ThreeVector position, G4double energy, G4double energyDep, G4ThreeVector momentum,
          G4bool exited, G4double inEn, G4int parentID);
   virtual ~BM_Hit();
   inline void *operator new(size_t);
   inline void operator delete(void *);

private: // Variables
   // "m" for "member"
   G4int id_;
   G4int pid_;
   G4int parentid_;
   // G4double mass_;
   // G4int charge_; // because it could be a secondary electron
   G4double time_;
   G4ThreeVector position_; // absolute position
   G4ThreeVector momentum_;
   // G4double theta_;
   G4double inEnergy_;
   G4double energy_;
   G4double energyDep_;
   G4bool exited_;

public: // Accessors
   G4int id() const { return id_; }
   G4int pid() const { return pid_; }
   // G4double mass() const {return mass_;}
   // G4int charge() const {return charge_;}
   G4double time() const { return time_; }
   G4ThreeVector position() const { return position_; } // globally
   G4ThreeVector momentum() const { return momentum_; }
   // G4double theta() const {return theta_;}
   G4double inEnergy() const { return inEnergy_; }
   G4double energy() const { return energy_; } // kinetic
   G4double energyDep() const { return energyDep_; }
   G4int parentID() const { return parentid_; }
   G4bool leftVolume() const { return exited_; };
   // Set methods
   //     void SetTrackID  (G4int track)      { fTrackID = track; };
   //     void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
   //     void SetEdep     (G4double de)      { fEdep = de; };
   //     void SetPos      (G4ThreeVector xyz){ fPos = xyz; };

   //     // Get methods
   //     G4int GetTrackID() const     { return fTrackID; };
   //     G4int GetChamberNb() const   { return fChamberNb; };
   //     G4double GetEdep() const     { return fEdep; };
   //     G4ThreeVector GetPos() const { return fPos; };

   //   private:

   //       G4int         fTrackID;
   //       G4int         fChamberNb;
   //       G4double      fEdep;
   //       G4ThreeVector fPos;
   void Print();
};

extern G4ThreadLocal G4Allocator<BM_Hit> *BM_HitAllocator;

typedef G4THitsCollection<BM_Hit> BM_HitsCollection;

inline void *BM_Hit::operator new(size_t) // Look into changing this
{
   if (!BM_HitAllocator)
      BM_HitAllocator = new G4Allocator<BM_Hit>;
   return (void *)BM_HitAllocator->MallocSingle();
}

inline void BM_Hit::operator delete(void *hit)
{
   BM_HitAllocator->FreeSingle((BM_Hit *)hit);
}

#endif // BM_HIT_HH
