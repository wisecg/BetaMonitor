// Retreived 2/22/2018 from http://geant4.slac.stanford.edu/Tips/event/1.html

#ifndef BM_TRACKINGACTION_HH
#define BM_TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "BM_TrackInfo.hh"

class BM_TrackingAction : public G4UserTrackingAction
{
public:
  BM_TrackingAction();
  virtual ~BM_TrackingAction();
  void PreUserTrackingAction(const G4Track *);
  void PostUserTrackingAction(const G4Track *);
};

#endif
