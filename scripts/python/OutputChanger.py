f = open("BM_Output.txt", "w")#Output File Name
h = open("BM_Outputhh.txt", "w")#Output File Name
Win = True#det_Wind (Formerly det_RT), used for the window
Trig = True#det_Trig (formerly det_RB), used for a triggering detector
Vac = True# Vacuum
#Comment unwanted parameters
mainDict = {'detMain_En':{'Output':'"detSQ_En",   &sqEn_0, "sqEn/D"','Setter':'sqEn_0 = en;','Bool':False,'Int':False,'Double':True, 'Header':'sqEn_0'},#Deposited Energy
            # 'detMain_x':{'Output':'"detSQ_x",     &sqx_0, "sqx/D"','Setter':'sqx_0 = x','Bool':False,'Int':False,'Double':True, 'Header':'sqx_0' },#average position
            # 'detMain_y':{'Output':'"detSQ_y",     &sqy_0, "sqy/D"','Setter':'sqy_0 = y','Bool':False,'Int':False,'Double':True, 'Header':'sqy_0' },#average position
            # 'detMain_z':{'Output':'"detSQ_z",     &sqz_0, "sqz/D"','Setter':'sqz_0 = z','Bool':False,'Int':False,'Double':True, 'Header':'sqz_0' },#average position
            'detMain_InEn':{'Output':'"detSQ_InEn",   &sqInEn_0, "sqInEn/D"','Setter':'sqInEn_0 = inen;','Bool':False,'Int':False,'Double':True, 'Header': 'sqInEn_0'},#Initial Energy
            'detMain_Inx':{'Output':'"detSQ_Inx",     &sqInx_0, "sqInx/D"','Setter':'sqInx_0 = inx;','Bool':False,'Int':False,'Double':True, 'Header': 'sqInx_0'},#Initial Position
            'detMain_Iny':{'Output':'"detSQ_Iny",     &sqIny_0, "sqIny/D"','Setter':'sqIny_0 = iny;','Bool':False,'Int':False,'Double':True, 'Header': 'sqIny_0'},#Initial Position
            'detMain_Inz':{'Output':'"detSQ_Inz",     &sqInz_0, "sqInz/D"','Setter':'sqInz_0 = inz;','Bool':False,'Int':False,'Double':True, 'Header': 'sqInz_0'},#Initial Position
            'detMain_ID':{'Output':'"detSQ_ID",       &sqID_0, "sqID/I"','Setter':'sqID_0 = iD;','Bool':False,'Int':True,'Double':False, 'Header':'sqID_0' },#Initial PiD
            # 'detMain_EnPos':{'Output':'"detSQ_EnPos",   &sqEnPos_0, "sqEnPos/D"','Setter':'sqEnPos_0 = enPos;','Bool':False,'Int':False,'Double':True, 'Header': 'sqEnPos_0'},#Positron Deposited Energy
            # 'detMain_EnEle':{'Output':'"detSQ_EnElec", &sqEnElec_0, "sqEnElec/D"','Setter':'sqEnElec_0= enElec;','Bool':False,'Int':False,'Double':True, 'Header':'sqEnElec_0' },#Electron Deposited Energy
            # 'detMain_EnPho':{'Output':'"detSQ_EnPhot", &sqEnPhot_0, "sqEnPhot/D"','Setter':'sqEnPhot_0 = enPhot;','Bool':False,'Int':False,'Double':True, 'Header': 'sqEnPhot_0'},#Photon Deposited Energy
            # 'detMain_EnOth':{'Output':'"detSQ_EnOth",   &sqEnOth_0, "sqEnOth/D"','Setter':'sqEnOth_0 = enOther;','Bool':False,'Int':False,'Double':True, 'Header': 'sqEnOth_0'},#Other Deposited Energy
            # 'detMain_IDNew':{'Output':'"detSQ_IDNew",   &sqIDNew_0, "sqIDNew/I"','Setter':'sqIDNew_0 = IDNew;','Bool':False,'Int':True,'Double':False, 'Header': 'sqIDNew_0'},#Other PiD
            # 'detMain_exitAnn':{'Output':'"detSQ_exitAnn",   &sqAnnExit_0, "sqAnnExit/I"','Setter':'sqAnnExit_0 = exitAnnCounter;','Bool':False,'Int':True,'Double':False, 'Header': 'sqAnnExit_0'},#Annihilation Photon leaving volume
            # 'detMain_exitPho':{'Output':'"detSQ_exitPhot",   &sqPhotExit_0, "sqPhotExit/I"','Setter':'sqPhotExit_0 = exitPhotCounter;','Bool':False,'Int':True,'Double':False, 'Header': 'sqPhotExit_0'},#Photon leaving volume
            # 'detMain_exitEle':{'Output':'"detSQ_exitElec",   &sqElecExit_0, "sqElecExit/I"','Setter':'sqElecExit_0 = exitElecCounter;','Bool':False,'Int':True,'Double':False, 'Header': 'sqElecExit_0'},#Electron leaving volume
            # 'detMain_exitPos':{'Output':'"detSQ_exitPos",   &sqPosExit_0, "sqPosExit/I"','Setter':'sqPosExit_0 = exitPosCounter;','Bool':False,'Int':True,'Double':False, 'Header': 'sqPosExit_0'},#Positron leaving volume
            # 'detMain_exitOth':{'Output':'"detSQ_exitOth",   &sqOthExit_0, "sqOthExit/I"','Setter':'sqOthExit_0 = exitOthCounter;','Bool':False,'Int':True,'Double':False, 'Header': 'sqOthExit_0'},#Other leaving volume
            # 'detMain_Annix':{'Output':'"detSQ_Annix",    &sqAnnix_0, "sqAnnix/D"','Setter':'sqAnnix_0 = annix;','Bool':False,'Int':False,'Double':True, 'Header': 'sqAnnix_0'},#Annihilation Position in volume
            # 'detMain_Anniy':{'Output':'"detSQ_Anniy",    &sqAnniy_0, "sqAnniy/D"','Setter':'sqAnniy_0 = anniy;','Bool':False,'Int':False,'Double':True, 'Header': 'sqAnniy_0'},#Annihilation Position in volume
            # 'detMain_Anniz':{'Output':'"detSQ_Anniz",    &sqAnniz_0, "sqAnniz/D"','Setter':'sqAnniz_0 = anniz;','Bool':False,'Int':False,'Double':True, 'Header': 'sqAnniz_0'},#Annihilation Position in volume
            # 'detMain_Escx':{'Output':'"detSQ_Escx",    &sqEscx_0, "sqEscx/D"','Setter':'sqEscx_0 = escx;','Bool':False,'Int':False,'Double':True, 'Header': 'sqEscx_0'},#Escape position from Volume
            # 'detMain_Escy':{'Output':'"detSQ_Escy",    &sqEscy_0, "sqEscy/D"','Setter':'sqEscy_0 = escy;','Bool':False,'Int':False,'Double':True, 'Header': 'sqEscy_0'},#Escape position from Volume
            # 'detMain_Escz':{'Output':'"detSQ_Escz",    &sqEscz_0, "sqEscz/D"','Setter':'sqEscz_0 = escz;','Bool':False,'Int':False,'Double':True, 'Header': 'sqEscz_0'}#Escape position from Volume
            }


unused = """ sqt_0 = t; 
 sqfinEn_0 = finEn;
  sqExit_0 = exited;
  sqEnAnn_0 = enAnn;"""


fileheader = """\nFile_0 = new TFile(Form(Name_0.c_str()), "RECREATE");
Tree_0 = new TTree("simData", "Simulation Data");
"""
setterHeader = """void BM_Output::setSQParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{"""
sLineEnder = "\n"
setterFooter = "}\n"
Brancher = "Tree_0->Branch("
fLineEnder = ");\n"
hheaderInt = ''
hheaderBool= ''
hheaderDouble = ''
for i in mainDict:
    if mainDict[i]['Int'] ==True:
        hheaderInt += mainDict[i]['Header']+','
    elif mainDict[i]['Bool'] ==True:
        hheaderBool += mainDict[i]['Header']+','
    elif mainDict[i]['Double'] == True:
        hheaderDouble += mainDict[i]['Header']+','
    fileheader += Brancher + mainDict[i]['Output'] + fLineEnder
    setterHeader += mainDict[i]['Setter'] +sLineEnder
    print(Brancher + mainDict[i]['Output']+fLineEnder)
    if Trig ==True:
        rt = mainDict[i]['Output'].replace('sq', 'trig')
        rt = rt.replace('SQ', 'Trig')
        fileheader += Brancher + rt + fLineEnder
        if mainDict[i]['Int'] ==True:
            hheaderInt += mainDict[i]['Header'].replace('sq','trig')+','
        elif mainDict[i]['Bool'] ==True:
            hheaderBool += mainDict[i]['Header'].replace('sq','trig')+','
        elif mainDict[i]['Double'] == True:
            hheaderDouble += mainDict[i]['Header'].replace('sq','trig')+','
    if Win == True:
        rb = mainDict[i]['Output'].replace('sq', 'wind')
        rb =rb.replace('SQ', 'Wind')
        fileheader += Brancher + rb + fLineEnder
        if mainDict[i]['Int'] ==True:
            hheaderInt += mainDict[i]['Header'].replace('sq','wind')+','
        elif mainDict[i]['Bool'] ==True:
            hheaderBool += mainDict[i]['Header'].replace('sq','wind')+','
        elif mainDict[i]['Double'] == True:
            hheaderDouble += mainDict[i]['Header'].replace('sq','wind')+','
    if Vac == True:
        vac = mainDict[i]['Output'].replace('sq', 'vac')
        vac = vac.replace('SQ', 'Vac')
        fileheader += Brancher + vac + fLineEnder
        if mainDict[i]['Int'] ==True:
            hheaderInt += mainDict[i]['Header'].replace('sq','vac')+','
        elif mainDict[i]['Bool'] ==True:
            hheaderBool += mainDict[i]['Header'].replace('sq','vac')+','
        elif mainDict[i]['Double'] == True:
            hheaderDouble += mainDict[i]['Header'].replace('sq','vac')+','
        print("I am a vacuum")
setterHeader += setterFooter
print(fileheader)



header = """// BM_Output.cc - Implementation of BM_Output class to write data to TTree for ROOT analysis

#include "BM_Output.hh"
#include "G4Threading.hh"
#include "PMT.hh"

#include <string>

#include <TFile.h>
#include <TTree.h>
#include "g4root.hh"

BM_Output* BM_Output::Instance_ = nullptr;

BM_Output::BM_Output(){}

BM_Output::~BM_Output(){}

BM_Output* BM_Output::Instance()
{
  if (!Instance_)
  {
    Instance_ = new BM_Output();
  }
  return Instance_;
}

// Writeout functions
void BM_Output::SetFilename()
{
  Name_0 =  "output/19NeSiKapton120_6Th0.root";
  Name_1 =  "output/19NeSiKapton120_6Th1.root";
  Name_2 =  "output/19NeSiKapton120_6Th2.root";
  Name_3 =  "output/19NeSiKapton120_6Th3.root";
  Name_4 =  "output/19NeSiKapton120_6Th4.root";
  Name_5 =  "output/19NeSiKapton120_6Th5.root";
  Name_6 =  "output/19NeSiKapton120_6Th6.root";
  Name_7 =  "output/19NeSiKapton120_6Th7.root";
  Name_8 =  "output/19NeSiKapton120_6Th8.root";
  Name_9 =  "output/19NeSiKapton120_6Th9.root";
  Name_10 = "output/19NeSiKapton120_6Th10.root";
  Name_11 = "output/19NeSiKapton120_6Th11.root";
  Name_12 = "output/19NeSiKapton120_6Th12.root";
  Name_13 = "output/19NeSiKapton120_6Th13.root";
  Name_14 = "output/19NeSiKapton120_6Th14.root";
  Name_15 = "output/19NeSiKapton120_6Th15.root";

}

void BM_Output::OpenFile()
{
    """

headerHeader = """// BM_Output.hh - Header file for BM_Output class to write data to TTree for ROOT analysis

#ifndef BM_OUTPUT_HH
#define BM_OUTPUT_HH
//#include "PMT.hh"
#include <string>

class TFile;
class TTree;

class BM_Output
{
public:
  BM_Output();
  virtual ~BM_Output();
  static BM_Output* Instance();

  // Writeout functions
  void SetFilename();
  void OpenFile();
  void CloseFile();
  void Fill();

  // Decay function

  // Detection functions

  void setTrigParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);//, bool bs, double bsx, double bsy, double bsz, double bst);
  void setWindParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);//, bool bs, double bsx, double bsy, double bsz, double bst);
  void setSQParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);//, bool bs, double bsx, double bsy, double bsz, double bst);
  void setTrigParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setTrigParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setWindParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setSQParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  void setVacParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter);
  
private:
  static BM_Output* Instance_;
 // PMT* pmt;

  // Writeout variables
  std::string Name_0;
  std::string Name_1;
  std::string Name_2;
  std::string Name_3;
  std::string Name_4;
  std::string Name_5;
  std::string Name_6;
  std::string Name_7;
  std::string Name_8;
  std::string Name_9;
  std::string Name_10;
  std::string Name_11;
  std::string Name_12;
  std::string Name_13;
  std::string Name_14;
  std::string Name_15;
  TFile* File_0;
  TTree* Tree_0;
  TFile* File_1;
  TTree* Tree_1;
  TFile* File_2;
  TTree* Tree_2;
  TFile* File_3;
  TTree* Tree_3;
  TFile* File_4;
  TTree* Tree_4;
  TFile* File_5;
  TTree* Tree_5;
  TFile* File_6;
  TTree* Tree_6;
  TFile* File_7;
  TTree* Tree_7;
  TFile* File_8;
  TTree* Tree_8;
  TFile* File_9;
  TTree* Tree_9;
  TFile* File_10;
  TTree* Tree_10;
  TFile* File_11;
  TTree* Tree_11;
  TFile* File_12;
  TTree* Tree_12;
  TFile* File_13;
  TTree* Tree_13;
  TFile* File_14;
  TTree* Tree_14;
  TFile* File_15;
  TTree* Tree_15;
"""
headerBool = 'bool temp,'
headerDouble = 'double temp2,'
headerInt = 'int temp3,'
filler = """}

void BM_Output::CloseFile()
{
  File_0->cd();
  Tree_0->Write();
  File_0->Close();
  File_1->cd();
  Tree_1->Write();
  File_1->Close();
  File_2->cd();
  Tree_2->Write();
  File_2->Close();
  File_3->cd();
  Tree_3->Write();
  File_3->Close();
  File_4->cd();
  Tree_4->Write();
  File_4->Close();
  File_5->cd();
  Tree_5->Write();
  File_5->Close();
  File_6->cd();
  Tree_6->Write();
  File_6->Close();
  File_7->cd();
  Tree_7->Write();
  File_7->Close();
  File_8->cd();
  Tree_8->Write();
  File_8->Close();
  File_9->cd();
  Tree_9->Write();
  File_9->Close();
  File_10->cd();
  Tree_10->Write();
  File_10->Close();
  File_11->cd();
  Tree_11->Write();
  File_11->Close();
  File_12->cd();
  Tree_12->Write();
  File_12->Close();
  File_13->cd();
  Tree_13->Write();
  File_13->Close();
  File_14->cd();
  Tree_14->Write();
  File_14->Close();
  File_15->cd();
  Tree_15->Write();
  File_15->Close();
  

}

void BM_Output::Fill()
{
  G4int threadnum = G4Threading::G4GetThreadId();
  switch(threadnum){
    case 0:
      Tree_0->Fill();
      break;
    case 1:
      Tree_1->Fill();
      break;
    case 2:
      Tree_2->Fill();
      break;
    case 3:
      Tree_3->Fill();
      break;
    case 4:
      Tree_4->Fill();
      break;
    case 5:
      Tree_5->Fill();
      break;
    case 6:
      Tree_6->Fill();
      break;
    case 7:
      Tree_7->Fill();
      break;
    case 8:
      Tree_8->Fill();
      break;
    case 9:
      Tree_9->Fill();
      break;
    case 10:
      Tree_10->Fill();
      break;
    case 11:
      Tree_11->Fill();
      break;
    case 12:
      Tree_12->Fill();
      break;
    case 13:
      Tree_13->Fill();
      break;
    case 14:
      Tree_14->Fill();
      break;
    case 15:
      Tree_15->Fill();
      break;
  }

}"""

f.write(header)
h.write(headerHeader)
for i in range(16):
    headerBool +=hheaderBool.replace('0',str(i))
    f.write(fileheader.replace('0',str(i)))
h.write(headerBool[:-1]+';\n')
f.write(filler)
for i in range(16):
    headerDouble += hheaderDouble.replace('0',str(i))
    f.write(setterHeader.replace('0',str(i)))
    if Win == True:
      rt2 = setterHeader.replace('SQ','Trig')
      rt2 = rt2.replace('sq', 'trig')
      f.write(rt2.replace('0',str(i)))
    if Trig == True:
      rb2 = setterHeader.replace('SQ','Wind')
      rb2 = rb2.replace('sq', 'wind')
      f.write(rb2.replace('0',str(i)))
    if Vac == True:
      vac2 = setterHeader.replace('SQ','Vac')
      vac2 = vac2.replace('sq', 'vac')
      f.write(vac2.replace('0',str(i)))
h.write(headerDouble[:-1]+';\n')
for i in range(16):
    headerInt += hheaderInt.replace('0',str(i))
h.write(headerInt[:-1]+';\n};\n\n#endif')
f.close()
h.close()


