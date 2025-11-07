// Retreived 2/22/2018 from http://geant4.slac.stanford.edu/Tips/event/1.html

#ifndef BM_TRACKINFO_H
#define BM_TRACKINFO_H

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class BM_TrackInfo : public G4VUserTrackInformation
{
public:
  BM_TrackInfo();
  BM_TrackInfo(const G4Track *aTrack);
  BM_TrackInfo(const G4Track *aTrack, bool beta2);
  BM_TrackInfo(const BM_TrackInfo *aTrackInfo);
  virtual ~BM_TrackInfo();

  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);
  inline int operator==(const BM_TrackInfo &right) const
  {
    return (this == &right);
  }

  void Print() const;

private:
  G4int originalTrackID;
  G4ParticleDefinition *particleDefinition;
  G4ThreeVector originalMomentum;
  G4double originalEnergy;
  G4double originalTime;
  G4bool angleIsRead_;

public:
  inline G4int GetOriginalTrackID() const { return originalTrackID; }
  inline G4ParticleDefinition *GetOriginalParticle() const { return particleDefinition; }
  inline G4ThreeVector GetOriginalMomentum() const { return originalMomentum; }
  inline G4double GetOriginalEnergy() const { return originalEnergy; }
  inline G4double GetOriginalTime() const { return originalTime; }
  G4bool AlreadyReadAngle();
};

extern G4Allocator<BM_TrackInfo> aTrackInfoAllocator;

inline void *BM_TrackInfo::operator new(size_t)
{
  void *aTrackInfo;
  aTrackInfo = (void *)aTrackInfoAllocator.MallocSingle();
  return aTrackInfo;
}

inline void BM_TrackInfo::operator delete(void *aTrackInfo)
{
  aTrackInfoAllocator.FreeSingle((BM_TrackInfo *)aTrackInfo);
}

#endif
