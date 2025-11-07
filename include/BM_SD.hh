// BM_SD.hh - Header file for BM_SD class: BetaMon Sensitive Detector

#ifndef BM_SD_HH
#define BM_SD_HH

#include "BM_Hit.hh"
#include "G4VSensitiveDetector.hh"

#include "G4Step.hh"

class BM_SD : public G4VSensitiveDetector
{
public:
   BM_SD(G4String name, G4String HCName);
   virtual ~BM_SD();
   BM_HitsCollection *pCollection_;

private:
   G4ThreeVector pIncident_;
   // G4String name;
   bool printed_;

public:
   virtual void Initialize(G4HCofThisEvent *HCE);
   virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *);
};

#endif // BM_SD_HH
