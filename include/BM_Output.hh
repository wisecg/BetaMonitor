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
  void setTrigParams0(int iD, double en, double inen, double inx, double iny, double inz);
  void setWindParams0(int iD, double en, double inen, double inx, double iny, double inz);
  void setSQParams0(int iD, double en, double inen, double inx, double iny, double inz);  
  void setVacParams0(int iD, double en, double inen, double inx, double iny, double inz);  

private:
  static BM_Output *Instance_;

  std::string Name;
  TFile *File;
  TTree *Tree;
  
  int sqID, trigID, windID, vacID;

  double sqEn, sqInEn, sqInx, sqIny, sqInz;
  double trigEn, trigInEn, trigInx, trigIny, trigInz;
  double windEn, windInEn, windInx, windIny, windInz;
  double vacEn, vacInEn, vacInx, vacIny, vacInz;
};

#endif