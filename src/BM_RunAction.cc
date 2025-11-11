// BM_RunAction.cc - Implementation of BM_RunAction class
#include "nat_units.hh"
#include "FileReader.hh"
#include "g4root.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "BM_RunAction.hh"
#include "BM_PrimaryGenerator.hh"
#include "BM_EventAction.hh"
#include "BM_SteppingAction.hh"
#include "BM_Output.hh"

BM_RunAction::BM_RunAction() : G4UserRunAction()
{

  auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  // analysisManager->SetNtupleRowWise(false);
  // analysisManager->SetFirstNtupleId(0);
  // analysisManager->SetNtupleMerging(true);
}

BM_RunAction::~BM_RunAction()
{
  delete G4AnalysisManager::Instance();
  // delete BM_Output::Instance();// Probably?
  // G4cout<<"run 1"<<G4endl;
}

void BM_RunAction::BeginOfRunAction(const G4Run *aRun)
{
  // G4cout<<"run 2"<<G4endl;
  //  auto analysisManager = G4AnalysisManager::Instance();
  //  analysisManager->SetFileName("Bi");
  //  analysisManager->OpenFile();
  //  analysisManager->CreateNtuple("Bi", "Hits");
  //  analysisManager->CreateNtupleIColumn("detRT_Hit");
  //  analysisManager->CreateNtupleDColumn("detRT_En");
  //  analysisManager->CreateNtupleDColumn("detRT_x");
  //  analysisManager->CreateNtupleDColumn("detRT_y");
  //  analysisManager->CreateNtupleDColumn("detRT_z");
  //  analysisManager->CreateNtupleDColumn("detRT_t");
  //  analysisManager->CreateNtupleIColumn("detRB_Hit");
  //  analysisManager->CreateNtupleDColumn("detRB_En");
  //  analysisManager->CreateNtupleDColumn("detRB_x");
  //  analysisManager->CreateNtupleDColumn("detRB_y");
  //  analysisManager->CreateNtupleDColumn("detRB_z");
  //  analysisManager->CreateNtupleDColumn("detRB_t");
  //  analysisManager->CreateNtupleIColumn("detSQ_Hit");
  //  analysisManager->CreateNtupleDColumn("detSQ_En");
  //  analysisManager->CreateNtupleDColumn("detSQ_x");
  //  analysisManager->CreateNtupleDColumn("detSQ_y");
  //  analysisManager->CreateNtupleDColumn("detSQ_z");
  //  analysisManager->CreateNtupleDColumn("detSQ_t");
  //  analysisManager->FinishNtuple();
  BM_Output::Instance()->OpenFile();
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4cout << "CALLING GETANEVENT CLINT" << G4endl;
  FileReader::Instance()->GetAnEvent();
  // initialize event cumulative quantities
  BM_EventAction::Instance()->Reset();
  // initialize our event generator
}

void BM_RunAction::EndOfRunAction(const G4Run *aRun)
{
  // auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->Write();
  // analysisManager->CloseFile();
  BM_Output::Instance()->CloseFile();
}
