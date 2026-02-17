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
  void FillPrimaryInput();

  // Decay function

  // Detection functions
  void setParams(int pid_hit, int eventid_hit, int trackid_hit, int parentid_hit, 
                 int volumeid_hit, double primaryenergy_hit, double inenergy_hit, 
                 double kineticenergy_hit, double depenergy_hit, bool exited_hit, double x_hit, 
                 double y_hit, double z_hit, double px_hit, double py_hit, double pz_hit);
  void setPrimaryInputParams(int pid, int eventid, double x, double y, double z, double px, double py, double pz, double primaryenergy);
private:
  static BM_Output *Instance_;

  std::string Name;
  TFile *File;
  TTree *Tree;
  TTree *primaryInputTree;

  // Output tree variables
  int pid, eventid, trackid, parentid, volumeid;
  double primaryenergy, inenergy, kineticenergy, depenergy;
  double x, y, z;
  double px, py, pz;
  bool exited;

  // Primary input tree variables
  int primaryInput_pid, primaryInput_eventid;
  double primaryInput_x, primaryInput_y, primaryInput_z;
  double primaryInput_px, primaryInput_py, primaryInput_pz;
  double primaryInput_primaryenergy;
};

#endif