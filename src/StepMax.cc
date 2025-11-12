/// \file electromagnetic/TestEm3/src/StepMax.cc
/// \brief Implementation of the StepMax class
// $Id: StepMax.cc 98762 2016-08-09 14:08:07Z gcosmo $

#include "StepMax.hh"
#include "StepMaxMessenger.hh"

StepMax::StepMax(const G4String& processName)
 : G4VDiscreteProcess(processName),fMess(0)
{
  for (G4int k=0; k<kMaxAbsor; k++) fStepMax[k] = DBL_MAX;
  fMess = new StepMaxMessenger(this);
}

StepMax::~StepMax() { delete fMess; }

G4bool StepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0. && !particle.IsShortLived());
}

void StepMax::SetStepMax(G4int k,G4double step) {fStepMax[k] = step;}
