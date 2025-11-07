/// \file electromagnetic/TestEm3/include/StepMaxMessenger.hh
/// \brief Definition of the StepMaxMessenger class
//
// $Id: StepMaxMessenger.hh 66241 2012-12-13 18:34:42Z gunter $
//

#ifndef StepMaxMessenger_h
#define StepMaxMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class StepMax;
class G4UIdirectory;
class G4UIcommand;

class StepMaxMessenger : public G4UImessenger
{
public:
  StepMaxMessenger(StepMax *);
  ~StepMaxMessenger();

  void SetNewValue(G4UIcommand *, G4String);

private:
  StepMax *fStepMax;

  G4UIdirectory *fStepMaxDir;
  G4UIcommand *fStepMaxCmd;
};

#endif
