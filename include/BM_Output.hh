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
  void setParams(int pid_hit, int eventid_hit, int trackid_hit, int parentid_hit, 
                 int volumeid_hit, double primaryenergy_hit, double inenergy_hit, 
                 double kineticenergy_hit, double depenergy_hit, bool exited_hit, double x_hit, 
                 double y_hit, double z_hit, double px_hit, double py_hit, double pz_hit);
private:
  static BM_Output *Instance_;

  std::string Name;
  TFile *File;
  TTree *Tree;
  
  int pid, eventid, trackid, parentid, volumeid;
  double primaryenergy, inenergy, kineticenergy, depenergy;
  double x, y, z;
  double px, py, pz;
  bool exited;

};

#endif