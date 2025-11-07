// Retreived 2/22/2018 from http://geant4.slac.stanford.edu/Tips/event/1.html

#include "BM_TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"

BM_TrackingAction::BM_TrackingAction() {}
BM_TrackingAction::~BM_TrackingAction() {}

void BM_TrackingAction::PreUserTrackingAction(const G4Track *aTrack)
{
  if (aTrack->GetUserInformation() == 0)
  {
    if (aTrack->GetParentID() == 0)
    {
      BM_TrackInfo *anInfo = new BM_TrackInfo(aTrack);
      G4Track *theTrack = (G4Track *)aTrack;
      theTrack->SetUserInformation(anInfo);
    }
    else
    {
      if (aTrack->GetParticleDefinition()->GetParticleName() == "e+")
      {
        G4cout << "We just made a new positron" << G4endl;
        BM_TrackInfo *anInfo = new BM_TrackInfo(aTrack, TRUE);
      }
    }
  }
}

void BM_TrackingAction::PostUserTrackingAction(const G4Track *aTrack)
{
  G4TrackVector *secondaries = fpTrackingManager->GimmeSecondaries();
  if (secondaries)
  {
    BM_TrackInfo *info = (BM_TrackInfo *)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    for (size_t i = 0; i < nSeco; i++)
    {
      BM_TrackInfo *infoNew = new BM_TrackInfo(info);
      (*secondaries)[i]->SetUserInformation(infoNew);
    }
  }
}
