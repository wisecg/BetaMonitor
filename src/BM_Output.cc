// BM_Output.cc - Implementation of BM_Output class to write data to TTree for ROOT analysis
// UPDATE Nov 2025 - removed hardcoded multithreaded output.

#include <string>

#include "G4RootAnalysisManager.hh"
#include "G4GenericMessenger.hh"
#include <TFile.h>
#include <TTree.h>

#include "BM_Output.hh"

BM_Output *BM_Output::Instance_ = nullptr;

BM_Output::BM_Output()
{
  fMessenger = std::make_unique<G4GenericMessenger>(this, "/bm/", "BetaMon controls");
  fMessenger->DeclareMethod("outputFile", &BM_Output::SetFilename,
                            "Set output ROOT filename.");
}

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
  primaryInputTree = new TTree("primaryInput", "Primary Input Data");


  Tree->Branch("pid", &pid, "pid/I");
  Tree->Branch("eventid", &eventid, "eventid/I");
  Tree->Branch("trackid", &trackid, "trackid/I");
  Tree->Branch("parentid", &parentid, "parentid/I");
  Tree->Branch("volumeid", &volumeid, "volumeid/I");
  Tree->Branch("primaryenergy", &primaryenergy, "primaryenergy/D");
  Tree->Branch("inenergy", &inenergy, "inenergy/D");
  Tree->Branch("kineticenergy", &kineticenergy, "kineticenergy/D");
  Tree->Branch("depenergy", &depenergy, "depenergy/D");
  Tree->Branch("x", &x, "x/D");
  Tree->Branch("y", &y, "y/D");
  Tree->Branch("z", &z, "z/D");
  Tree->Branch("px", &px, "px/D");
  Tree->Branch("py", &py, "py/D");
  Tree->Branch("pz", &pz, "pz/D");
  Tree->Branch("exited", &exited, "exited/O");
  Tree->Branch("pathlen", &pathlen, "pathlen/D");

  // Branches for primaryInputTree
  primaryInputTree->Branch("pid", &primaryInput_pid, "pid/I");
  primaryInputTree->Branch("eventid", &primaryInput_eventid, "eventid/I");
  primaryInputTree->Branch("x", &primaryInput_x, "x/D");
  primaryInputTree->Branch("y", &primaryInput_y, "y/D");
  primaryInputTree->Branch("z", &primaryInput_z, "z/D");
  primaryInputTree->Branch("px", &primaryInput_px, "px/D");
  primaryInputTree->Branch("py", &primaryInput_py, "py/D");
  primaryInputTree->Branch("pz", &primaryInput_pz, "pz/D");
  primaryInputTree->Branch("primaryenergy", &primaryInput_primaryenergy, "primaryenergy/D");

}

void BM_Output::CloseFile()
{
  File->cd();
  Tree->Write();
  if (primaryInputTree) primaryInputTree->Write();
  File->Close();
}

void BM_Output::Fill()
{
  Tree->Fill();
}

void BM_Output::setParams(int pid_hit, int eventid_hit, int trackid_hit, int parentid_hit, int volumeid_hit, 
                          double primaryenergy_hit, double inenergy_hit, double kineticenergy_hit, 
                          double depenergy_hit, bool exited_hit, double x_hit, double y_hit, double z_hit, double px_hit, 
                          double py_hit, double pz_hit, double pathlen_hit)
{
  pid = pid_hit;
  eventid = eventid_hit;
  trackid = trackid_hit;
  parentid = parentid_hit;
  volumeid = volumeid_hit;
  primaryenergy = primaryenergy_hit;
  inenergy = inenergy_hit;
  kineticenergy =  kineticenergy_hit;
  depenergy = depenergy_hit;
  exited = exited_hit;
  x = x_hit;
  y = y_hit;
  z = z_hit;
  px = px_hit;
  py = py_hit;
  pz = pz_hit;
  pathlen = pathlen_hit;
}

void BM_Output::FillPrimaryInput()
{
  if (primaryInputTree) primaryInputTree->Fill();
}

void BM_Output::setPrimaryInputParams(int pid, int eventid, double x, double y, double z, double px, double py, double pz, double primaryenergy)
{
  primaryInput_pid = pid;
  primaryInput_eventid = eventid;
  primaryInput_x = x;
  primaryInput_y = y;
  primaryInput_z = z;
  primaryInput_px = px;
  primaryInput_py = py;
  primaryInput_pz = pz;
  primaryInput_primaryenergy = primaryenergy;
}



