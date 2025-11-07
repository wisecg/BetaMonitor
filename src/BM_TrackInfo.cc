// Based on http://geant4.slac.stanford.edu/Tips/event/1.html

#include "BM_TrackInfo.hh"
#include "G4ios.hh"

G4Allocator<BM_TrackInfo> aTrackInfoAllocator;

BM_TrackInfo::BM_TrackInfo() : originalTrackID(0),
                               particleDefinition(0),
                               originalEnergy(0.),
                               originalTime(0.)
{
  originalMomentum = G4ThreeVector(0., 0., 0.);
  angleIsRead_ = FALSE;
}

BM_TrackInfo::BM_TrackInfo(const G4Track *aTrack)
{
  originalTrackID = aTrack->GetTrackID();
  particleDefinition = aTrack->GetDefinition();
  originalMomentum = aTrack->GetMomentum();
  originalEnergy = aTrack->GetTotalEnergy();
  originalTime = aTrack->GetGlobalTime();
}

BM_TrackInfo::BM_TrackInfo(const G4Track *aTrack, bool beta2)
{
  originalTrackID = 3;
  particleDefinition = aTrack->GetDefinition();
  originalMomentum = aTrack->GetMomentum();
  originalEnergy = aTrack->GetTotalEnergy();
  originalTime = aTrack->GetGlobalTime();
}

BM_TrackInfo::BM_TrackInfo(const BM_TrackInfo *aTrackInfo)
{
  originalTrackID = aTrackInfo->originalTrackID;
  particleDefinition = aTrackInfo->particleDefinition;
  originalMomentum = aTrackInfo->originalMomentum;
  originalEnergy = aTrackInfo->originalEnergy;
  originalTime = aTrackInfo->originalTime;
}

BM_TrackInfo::~BM_TrackInfo() {}

void BM_TrackInfo::Print() const
{
  G4cout << "Original Track ID: " << originalTrackID << G4endl;
}

G4bool BM_TrackInfo::AlreadyReadAngle()
{
  G4bool ret = angleIsRead_;
  angleIsRead_ = TRUE;
  return ret;
}
