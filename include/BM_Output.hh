// BM_Output.hh - Header file for BM_Output class to write data to TTree for ROOT analysis

#ifndef BM_OUTPUT_HH
#define BM_OUTPUT_HH
// #include "PMT.hh"
#include <string>

class TFile;
class TTree;

class BM_Output
{
public:
  BM_Output();
  virtual ~BM_Output();
  static BM_Output *Instance();

  void SetFilename(G4String outname);
  G4String GetFilename();
  void OpenFile();
  void CloseFile();
  void Fill();

  // Decay function

  // Detection functions
  void setParams(int pid_step, int eventid_step, int trackid_step, int parentid_step, 
                 int volumeid_step, int stepnumber_step, double inenergy_step, 
                 double kineticenergy_step, double depenergy_step, double x_step, 
                 double y_step, double z_step, double px_step, double py_step, double pz_step);
private:
  static BM_Output *Instance_;

  std::string Name;
  TFile *File;
  TTree *Tree;
  
  int pid, eventid, trackid, parentid, volumeid, stepnumber;
  double inenergy, kineticenergy, depenergy;
  double x, y, z;
  double px, py, pz;

};

#endif