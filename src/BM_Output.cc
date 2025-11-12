// BM_Output.cc - Implementation of BM_Output class to write data to TTree for ROOT analysis
// UPDATE Nov 2025 - removed hardcoded multithreaded output.

#include <string>

#include "g4root.hh"
#include <TFile.h>
#include <TTree.h>

#include "BM_Output.hh"
#include "PMT.hh"

BM_Output *BM_Output::Instance_ = nullptr;

BM_Output::BM_Output() {}

BM_Output::~BM_Output() {}

BM_Output *BM_Output::Instance()
{
  if (!Instance_)
  {
    Instance_ = new BM_Output();
  }
  return Instance_;
}

// Writeout functions
void BM_Output::SetFilename(G4String outname)
{
  Name = outname;
}

G4String BM_Output::GetFilename()
{
  return Name;
}

void BM_Output::OpenFile()
{
  File = new TFile(Name.c_str(), "RECREATE");
  Tree = new TTree("simData", "Simulation Data");

  Tree->Branch("detSQ_ID", &sqID, "sqID/I");
  Tree->Branch("detSQ_En", &sqEn, "sqEn/D");
  Tree->Branch("detSQ_InEn", &sqInEn, "sqInEn/D");
  Tree->Branch("detSQ_Inx", &sqInx, "sqInx/D");
  Tree->Branch("detSQ_Iny", &sqIny, "sqIny/D");
  Tree->Branch("detSQ_Inz", &sqInz, "sqInz/D");

  Tree->Branch("detTrig_ID", &trigID, "trigID/I");
  Tree->Branch("detTrig_En", &trigEn, "trigEn/D");
  Tree->Branch("detTrig_InEn", &trigInEn, "trigInEn/D");
  Tree->Branch("detTrig_Inx", &trigInx, "trigInx/D");
  Tree->Branch("detTrig_Iny", &trigIny, "trigIny/D");
  Tree->Branch("detTrig_Inz", &trigInz, "trigInz/D");

  Tree->Branch("detWind_ID", &windID, "windID/I");
  Tree->Branch("detWind_En", &windEn, "windEn/D");
  Tree->Branch("detWind_InEn", &windInEn, "windInEn/D");
  Tree->Branch("detWind_Inx", &windInx, "windInx/D");
  Tree->Branch("detWind_Iny", &windIny, "windIny/D");
  Tree->Branch("detWind_Inz", &windInz, "windInz/D");

  Tree->Branch("detVac_ID", &vacID, "vacID/I");
  Tree->Branch("detVac_En", &vacEn, "vacEn/D");
  Tree->Branch("detVac_InEn", &vacInEn, "vacInEn/D");
  Tree->Branch("detVac_Inx", &vacInx, "vacInx/D");
  Tree->Branch("detVac_Iny", &vacIny, "vacIny/D");
  Tree->Branch("detVac_Inz", &vacInz, "vacInz/D");
}

void BM_Output::CloseFile()
{
  File->cd();
  Tree->Write();
  File->Close();
}

void BM_Output::Fill()
{
  // // review data before filling.
  // if (sqEn > 0)
  //   printf("1 %i %f %f %f %f %f\n", sqID, sqEn, sqInEn, sqInx, sqIny, sqInz);
  // printf("2 %i %f %f %f %f %f\n", trigID, trigEn, trigInEn, trigInx, trigIny, trigInz);
  // printf("3 %i %f %f %f %f %f\n", windID, windEn, windInEn, windInx, windIny, windInz);
  // printf("4 %i %f %f %f %f %f\n", vacID, vacEn, vacInEn, vacInx, vacIny, vacInz);
  Tree->Fill();
}

void BM_Output::setSQParams0(int iD, double en, double inen, double inx, double iny, double inz)
{
  sqEn = en;
  sqInEn = inen;
  sqInx = inx;
  sqIny = iny;
  sqInz = inz;
  sqID = iD;
}

void BM_Output::setTrigParams0(int iD, double en, double inen, double inx, double iny, double inz)
{
  trigEn = en;
  trigInEn = inen;
  trigInx = inx;
  trigIny = iny;
  trigInz = inz;
  trigID = iD;
}

void BM_Output::setWindParams0(int iD, double en, double inen, double inx, double iny, double inz)
{
  windEn = en;
  windInEn = inen;
  windInx = inx;
  windIny = iny;
  windInz = inz;
  windID = iD;
}

void BM_Output::setVacParams0(int iD, double en, double inen, double inx, double iny, double inz)
{
  vacEn = en;
  vacInEn = inen;
  vacInx = inx;
  vacIny = iny;
  vacInz = inz;
  vacID = iD;
}
