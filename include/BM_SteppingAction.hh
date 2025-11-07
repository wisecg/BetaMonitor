// BM_SteppingAction.hh - Definition of BM_SteppingAction class

#ifndef BM_STEPPINGACTION_HH
#define BM_STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;

class BM_SteppingAction : public G4UserSteppingAction
{
public:
  BM_SteppingAction();
  virtual ~BM_SteppingAction();

  static BM_SteppingAction *Instance();
  virtual void UserSteppingAction(const G4Step *);
  void Reset();
  void SetVolume(G4LogicalVolume *volume) {}

private:
  static BM_SteppingAction *fgInstance;
};

#endif // BM_STEPPINGACTION_HH
