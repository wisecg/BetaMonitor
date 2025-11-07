/// \file electromagnetic/TestEm3/include/StepMax.hh
/// \brief Definition of the StepMax class
//
// $Id: StepMax.hh 98762 2016-08-09 14:08:07Z gcosmo $
//

#ifndef StepMax_h
#define StepMax_h 1

#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"

// #include "DetectorConstruction.hh"

const G4int kMaxAbsor = 10; // from TestEm3's DetectorConstruction.hh

class StepMaxMessenger;

class StepMax : public G4VDiscreteProcess
{
public:
  StepMax(const G4String &processName = "UserStepMax");
  ~StepMax();

  G4bool IsApplicable(const G4ParticleDefinition &);

  void SetStepMax(G4int, G4double);

  G4double GetStepMax(G4int k) { return fStepMax[k]; };

  G4double PostStepGetPhysicalInteractionLength(const G4Track &track,
                                                G4double previousStepSize,
                                                G4ForceCondition *condition);

  G4VParticleChange *PostStepDoIt(const G4Track &, const G4Step &);

  G4double GetMeanFreePath(const G4Track &, G4double, G4ForceCondition *)
  {
    return DBL_MAX;
  };

private:
  G4double fStepMax[kMaxAbsor];

  StepMaxMessenger *fMess;
};

inline G4double StepMax::PostStepGetPhysicalInteractionLength(const G4Track &aTrack,
                                                              G4double, G4ForceCondition *condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;

  G4double limit = DBL_MAX;
  G4int n = aTrack.GetVolume()->GetCopyNo();
  if (n < kMaxAbsor)
    limit = fStepMax[n];
  return limit;
}

inline G4VParticleChange *StepMax::PostStepDoIt(const G4Track &aTrack, const G4Step &)
{
  // do nothing
  aParticleChange.Initialize(aTrack);
  return &aParticleChange;
}

#endif
