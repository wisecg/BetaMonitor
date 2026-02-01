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

  Tree->Branch("pid", &pid, "pid/I");
  Tree->Branch("eventid", &eventid, "eventid/I");
  Tree->Branch("trackid", &trackid, "trackid/I");
  Tree->Branch("parentid", &parentid, "parentid/I");
  Tree->Branch("volumeid", &volumeid, "volumeid/I");
  Tree->Branch("stepnumber", &stepnumber, "stepnumber/I");
  Tree->Branch("inenergy", &inenergy, "inenergy/D");
  Tree->Branch("kineticenergy", &kineticenergy, "kineticenergy/D");
  Tree->Branch("depenergy", &depenergy, "depenergy/D");
  Tree->Branch("x", &x, "x/D");
  Tree->Branch("y", &y, "y/D");
  Tree->Branch("z", &z, "z/D");
  Tree->Branch("px", &px, "px/D");
  Tree->Branch("py", &py, "py/D");
  Tree->Branch("pz", &pz, "pz/D");

}

void BM_Output::CloseFile()
{
  File->cd();
  Tree->Write();
  File->Close();
}

void BM_Output::Fill()
{
  Tree->Fill();
}

void BM_Output::setParams(int pid_step, int eventid_step, int trackid_step, int parentid_step, int volumeid_step, int stepnumber_step, double inenergy_step, double kineticenergy_step, double depenergy_step, double x_step, double y_step, double z_step, double px_step, double py_step, double pz_step)
{
  pid = pid_step;
  eventid = eventid_step;
  trackid = trackid_step;
  parentid = parentid_step;
  volumeid = volumeid_step;
  stepnumber = stepnumber_step;
  inenergy = inenergy_step;
  kineticenergy =  kineticenergy_step;
  depenergy = depenergy_step;
  x = x_step;
  y = y_step;
  z = z_step;
  px = px_step;
  py = py_step;
  pz = pz_step;
}


