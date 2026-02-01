// BM_SD.cc - Implementation of BM_SD class: BetaMon sensitive detector

#include "BM_SD.hh"

#include <stdio.h>

#include "G4ios.hh"

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"

#include "BM_TrackInfo.hh"
#include "BM_Detector.hh"
#include "BM_Output.hh"

BM_SD::BM_SD(G4String name, G4String HCName) : G4VSensitiveDetector(name),
                                               pCollection_(NULL),
                                               pIncident_(0.),

                                               printed_(false)
{
   collectionName.insert(HCName); 
}

BM_SD::~BM_SD() {}

void BM_SD::Initialize(G4HCofThisEvent *hce)
{
   // SensitiveDetectorName = G4VSensitiveDetector(name);
   pCollection_ = new BM_HitsCollection(SensitiveDetectorName, collectionName[0]);
   G4int phcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); //
   hce->AddHitsCollection(phcID, pCollection_);
}

G4bool BM_SD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{

   
   G4ParticleDefinition *aPart = aStep->GetTrack()->GetDefinition();
   G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
   G4int id = preStepPoint->GetTouchableHandle()->GetVolume()->GetCopyNo(); // Which volume we hit

   G4int pid = aPart->GetPDGEncoding();
   G4double myTime = preStepPoint->GetGlobalTime();
   G4ThreeVector globalPosition = preStepPoint->GetPosition(); // global position
   G4ThreeVector localPosition = globalPosition - preStepPoint->GetPhysicalVolume()->GetObjectTranslation();
   G4ThreeVector momentum = aStep->GetTrack()->GetMomentum();
   localPosition.setZ(localPosition.z());
   G4double energy = aStep->GetTrack()->GetKineticEnergy();
   G4double inEnergy = preStepPoint->GetKineticEnergy();
   G4double energyDep = aStep->GetTotalEnergyDeposit();
   G4bool exited = aStep->IsLastStepInVolume();
   G4int parent = aStep->GetTrack()->GetParentID();
   G4int trackid = aStep->GetTrack()->GetTrackID();  

   BM_Hit *aHit = new BM_Hit(id, pid, myTime, globalPosition, energy, energyDep, momentum, exited, inEnergy, parent, trackid);

   pIncident_ = localPosition;
   pCollection_->insert(aHit);
   return true;
}
