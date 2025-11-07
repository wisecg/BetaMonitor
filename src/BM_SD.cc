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
   collectionName.insert(HCName); // like "proton"
   // collectionName.insert(HCName + "_b"); // like "beta"
   // collectionName.insert(HCName + "_b2"); // like "beta"
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
   // BM_TrackInfo* info = (BM_TrackInfo*) (aStep->GetTrack()->GetUserInformation());
   // G4double edep = aStep->GetTotalEnergyDeposit();
   // if(edep==0.) return true;//only log on energy loss

   // BM_Hit* newHit = new BM_Hit();
   //    int nHit = pCollection_->entries();
   //    for(int i=0;i<nHit;i++)
   //   {
   //     newHit = (*pCollection_)[i];
   //     newHit->SetEdep(edep);
   //     newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
   //     newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
   //                                                ->GetCopyNumber());
   //     newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
   //     return true;
   //   }
   //    newHit = new BM_Hit();
   //    newHit->SetEdep(edep);
   //    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
   //    newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
   //                                                ->GetCopyNumber());
   //    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
   G4ParticleDefinition *aPart = aStep->GetTrack()->GetDefinition();
   G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
   G4int id = preStepPoint->GetTouchableHandle()->GetVolume()->GetCopyNo(); // Which volume we hit

   // G4double mass = aPart->GetPDGMass();
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

   BM_Hit *aHit = new BM_Hit(id, pid, myTime, globalPosition, energy, energyDep, momentum, exited, inEnergy, parent);
   // G4String type = aPart->GetParticleSubType();
   // G4int parID = aStep->GetTrack()->GetParentID();
   // G4int myID = info->GetOriginalTrackID();
   pIncident_ = localPosition;
   pCollection_->insert(aHit);
   return true;
}
