// BM_Output.cc - Implementation of BM_Output class to write data to TTree for ROOT analysis

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
    
File_0 = new TFile(Form(Name_0.c_str()), "RECREATE");
Tree_0 = new TTree("simData", "Simulation Data");
Tree_0->Branch("detSQ_En",   &sqEn_0, "sqEn/D");
Tree_0->Branch("detTrig_En",   &trigEn_0, "trigEn/D");
Tree_0->Branch("detWind_En",   &windEn_0, "windEn/D");
Tree_0->Branch("detVac_En",   &vacEn_0, "vacEn/D");
Tree_0->Branch("detSQ_InEn",   &sqInEn_0, "sqInEn/D");
Tree_0->Branch("detTrig_InEn",   &trigInEn_0, "trigInEn/D");
Tree_0->Branch("detWind_InEn",   &windInEn_0, "windInEn/D");
Tree_0->Branch("detVac_InEn",   &vacInEn_0, "vacInEn/D");
Tree_0->Branch("detSQ_Inx",     &sqInx_0, "sqInx/D");
Tree_0->Branch("detTrig_Inx",     &trigInx_0, "trigInx/D");
Tree_0->Branch("detWind_Inx",     &windInx_0, "windInx/D");
Tree_0->Branch("detVac_Inx",     &vacInx_0, "vacInx/D");
Tree_0->Branch("detSQ_Iny",     &sqIny_0, "sqIny/D");
Tree_0->Branch("detTrig_Iny",     &trigIny_0, "trigIny/D");
Tree_0->Branch("detWind_Iny",     &windIny_0, "windIny/D");
Tree_0->Branch("detVac_Iny",     &vacIny_0, "vacIny/D");
Tree_0->Branch("detSQ_Inz",     &sqInz_0, "sqInz/D");
Tree_0->Branch("detTrig_Inz",     &trigInz_0, "trigInz/D");
Tree_0->Branch("detWind_Inz",     &windInz_0, "windInz/D");
Tree_0->Branch("detVac_Inz",     &vacInz_0, "vacInz/D");
Tree_0->Branch("detSQ_ID",       &sqID_0, "sqID/I");
Tree_0->Branch("detTrig_ID",       &trigID_0, "trigID/I");
Tree_0->Branch("detWind_ID",       &windID_0, "windID/I");
Tree_0->Branch("detVac_ID",       &vacID_0, "vacID/I");

File_1 = new TFile(Form(Name_1.c_str()), "RECREATE");
Tree_1 = new TTree("simData", "Simulation Data");
Tree_1->Branch("detSQ_En",   &sqEn_1, "sqEn/D");
Tree_1->Branch("detTrig_En",   &trigEn_1, "trigEn/D");
Tree_1->Branch("detWind_En",   &windEn_1, "windEn/D");
Tree_1->Branch("detVac_En",   &vacEn_1, "vacEn/D");
Tree_1->Branch("detSQ_InEn",   &sqInEn_1, "sqInEn/D");
Tree_1->Branch("detTrig_InEn",   &trigInEn_1, "trigInEn/D");
Tree_1->Branch("detWind_InEn",   &windInEn_1, "windInEn/D");
Tree_1->Branch("detVac_InEn",   &vacInEn_1, "vacInEn/D");
Tree_1->Branch("detSQ_Inx",     &sqInx_1, "sqInx/D");
Tree_1->Branch("detTrig_Inx",     &trigInx_1, "trigInx/D");
Tree_1->Branch("detWind_Inx",     &windInx_1, "windInx/D");
Tree_1->Branch("detVac_Inx",     &vacInx_1, "vacInx/D");
Tree_1->Branch("detSQ_Iny",     &sqIny_1, "sqIny/D");
Tree_1->Branch("detTrig_Iny",     &trigIny_1, "trigIny/D");
Tree_1->Branch("detWind_Iny",     &windIny_1, "windIny/D");
Tree_1->Branch("detVac_Iny",     &vacIny_1, "vacIny/D");
Tree_1->Branch("detSQ_Inz",     &sqInz_1, "sqInz/D");
Tree_1->Branch("detTrig_Inz",     &trigInz_1, "trigInz/D");
Tree_1->Branch("detWind_Inz",     &windInz_1, "windInz/D");
Tree_1->Branch("detVac_Inz",     &vacInz_1, "vacInz/D");
Tree_1->Branch("detSQ_ID",       &sqID_1, "sqID/I");
Tree_1->Branch("detTrig_ID",       &trigID_1, "trigID/I");
Tree_1->Branch("detWind_ID",       &windID_1, "windID/I");
Tree_1->Branch("detVac_ID",       &vacID_1, "vacID/I");

File_2 = new TFile(Form(Name_2.c_str()), "RECREATE");
Tree_2 = new TTree("simData", "Simulation Data");
Tree_2->Branch("detSQ_En",   &sqEn_2, "sqEn/D");
Tree_2->Branch("detTrig_En",   &trigEn_2, "trigEn/D");
Tree_2->Branch("detWind_En",   &windEn_2, "windEn/D");
Tree_2->Branch("detVac_En",   &vacEn_2, "vacEn/D");
Tree_2->Branch("detSQ_InEn",   &sqInEn_2, "sqInEn/D");
Tree_2->Branch("detTrig_InEn",   &trigInEn_2, "trigInEn/D");
Tree_2->Branch("detWind_InEn",   &windInEn_2, "windInEn/D");
Tree_2->Branch("detVac_InEn",   &vacInEn_2, "vacInEn/D");
Tree_2->Branch("detSQ_Inx",     &sqInx_2, "sqInx/D");
Tree_2->Branch("detTrig_Inx",     &trigInx_2, "trigInx/D");
Tree_2->Branch("detWind_Inx",     &windInx_2, "windInx/D");
Tree_2->Branch("detVac_Inx",     &vacInx_2, "vacInx/D");
Tree_2->Branch("detSQ_Iny",     &sqIny_2, "sqIny/D");
Tree_2->Branch("detTrig_Iny",     &trigIny_2, "trigIny/D");
Tree_2->Branch("detWind_Iny",     &windIny_2, "windIny/D");
Tree_2->Branch("detVac_Iny",     &vacIny_2, "vacIny/D");
Tree_2->Branch("detSQ_Inz",     &sqInz_2, "sqInz/D");
Tree_2->Branch("detTrig_Inz",     &trigInz_2, "trigInz/D");
Tree_2->Branch("detWind_Inz",     &windInz_2, "windInz/D");
Tree_2->Branch("detVac_Inz",     &vacInz_2, "vacInz/D");
Tree_2->Branch("detSQ_ID",       &sqID_2, "sqID/I");
Tree_2->Branch("detTrig_ID",       &trigID_2, "trigID/I");
Tree_2->Branch("detWind_ID",       &windID_2, "windID/I");
Tree_2->Branch("detVac_ID",       &vacID_2, "vacID/I");

File_3 = new TFile(Form(Name_3.c_str()), "RECREATE");
Tree_3 = new TTree("simData", "Simulation Data");
Tree_3->Branch("detSQ_En",   &sqEn_3, "sqEn/D");
Tree_3->Branch("detTrig_En",   &trigEn_3, "trigEn/D");
Tree_3->Branch("detWind_En",   &windEn_3, "windEn/D");
Tree_3->Branch("detVac_En",   &vacEn_3, "vacEn/D");
Tree_3->Branch("detSQ_InEn",   &sqInEn_3, "sqInEn/D");
Tree_3->Branch("detTrig_InEn",   &trigInEn_3, "trigInEn/D");
Tree_3->Branch("detWind_InEn",   &windInEn_3, "windInEn/D");
Tree_3->Branch("detVac_InEn",   &vacInEn_3, "vacInEn/D");
Tree_3->Branch("detSQ_Inx",     &sqInx_3, "sqInx/D");
Tree_3->Branch("detTrig_Inx",     &trigInx_3, "trigInx/D");
Tree_3->Branch("detWind_Inx",     &windInx_3, "windInx/D");
Tree_3->Branch("detVac_Inx",     &vacInx_3, "vacInx/D");
Tree_3->Branch("detSQ_Iny",     &sqIny_3, "sqIny/D");
Tree_3->Branch("detTrig_Iny",     &trigIny_3, "trigIny/D");
Tree_3->Branch("detWind_Iny",     &windIny_3, "windIny/D");
Tree_3->Branch("detVac_Iny",     &vacIny_3, "vacIny/D");
Tree_3->Branch("detSQ_Inz",     &sqInz_3, "sqInz/D");
Tree_3->Branch("detTrig_Inz",     &trigInz_3, "trigInz/D");
Tree_3->Branch("detWind_Inz",     &windInz_3, "windInz/D");
Tree_3->Branch("detVac_Inz",     &vacInz_3, "vacInz/D");
Tree_3->Branch("detSQ_ID",       &sqID_3, "sqID/I");
Tree_3->Branch("detTrig_ID",       &trigID_3, "trigID/I");
Tree_3->Branch("detWind_ID",       &windID_3, "windID/I");
Tree_3->Branch("detVac_ID",       &vacID_3, "vacID/I");

File_4 = new TFile(Form(Name_4.c_str()), "RECREATE");
Tree_4 = new TTree("simData", "Simulation Data");
Tree_4->Branch("detSQ_En",   &sqEn_4, "sqEn/D");
Tree_4->Branch("detTrig_En",   &trigEn_4, "trigEn/D");
Tree_4->Branch("detWind_En",   &windEn_4, "windEn/D");
Tree_4->Branch("detVac_En",   &vacEn_4, "vacEn/D");
Tree_4->Branch("detSQ_InEn",   &sqInEn_4, "sqInEn/D");
Tree_4->Branch("detTrig_InEn",   &trigInEn_4, "trigInEn/D");
Tree_4->Branch("detWind_InEn",   &windInEn_4, "windInEn/D");
Tree_4->Branch("detVac_InEn",   &vacInEn_4, "vacInEn/D");
Tree_4->Branch("detSQ_Inx",     &sqInx_4, "sqInx/D");
Tree_4->Branch("detTrig_Inx",     &trigInx_4, "trigInx/D");
Tree_4->Branch("detWind_Inx",     &windInx_4, "windInx/D");
Tree_4->Branch("detVac_Inx",     &vacInx_4, "vacInx/D");
Tree_4->Branch("detSQ_Iny",     &sqIny_4, "sqIny/D");
Tree_4->Branch("detTrig_Iny",     &trigIny_4, "trigIny/D");
Tree_4->Branch("detWind_Iny",     &windIny_4, "windIny/D");
Tree_4->Branch("detVac_Iny",     &vacIny_4, "vacIny/D");
Tree_4->Branch("detSQ_Inz",     &sqInz_4, "sqInz/D");
Tree_4->Branch("detTrig_Inz",     &trigInz_4, "trigInz/D");
Tree_4->Branch("detWind_Inz",     &windInz_4, "windInz/D");
Tree_4->Branch("detVac_Inz",     &vacInz_4, "vacInz/D");
Tree_4->Branch("detSQ_ID",       &sqID_4, "sqID/I");
Tree_4->Branch("detTrig_ID",       &trigID_4, "trigID/I");
Tree_4->Branch("detWind_ID",       &windID_4, "windID/I");
Tree_4->Branch("detVac_ID",       &vacID_4, "vacID/I");

File_5 = new TFile(Form(Name_5.c_str()), "RECREATE");
Tree_5 = new TTree("simData", "Simulation Data");
Tree_5->Branch("detSQ_En",   &sqEn_5, "sqEn/D");
Tree_5->Branch("detTrig_En",   &trigEn_5, "trigEn/D");
Tree_5->Branch("detWind_En",   &windEn_5, "windEn/D");
Tree_5->Branch("detVac_En",   &vacEn_5, "vacEn/D");
Tree_5->Branch("detSQ_InEn",   &sqInEn_5, "sqInEn/D");
Tree_5->Branch("detTrig_InEn",   &trigInEn_5, "trigInEn/D");
Tree_5->Branch("detWind_InEn",   &windInEn_5, "windInEn/D");
Tree_5->Branch("detVac_InEn",   &vacInEn_5, "vacInEn/D");
Tree_5->Branch("detSQ_Inx",     &sqInx_5, "sqInx/D");
Tree_5->Branch("detTrig_Inx",     &trigInx_5, "trigInx/D");
Tree_5->Branch("detWind_Inx",     &windInx_5, "windInx/D");
Tree_5->Branch("detVac_Inx",     &vacInx_5, "vacInx/D");
Tree_5->Branch("detSQ_Iny",     &sqIny_5, "sqIny/D");
Tree_5->Branch("detTrig_Iny",     &trigIny_5, "trigIny/D");
Tree_5->Branch("detWind_Iny",     &windIny_5, "windIny/D");
Tree_5->Branch("detVac_Iny",     &vacIny_5, "vacIny/D");
Tree_5->Branch("detSQ_Inz",     &sqInz_5, "sqInz/D");
Tree_5->Branch("detTrig_Inz",     &trigInz_5, "trigInz/D");
Tree_5->Branch("detWind_Inz",     &windInz_5, "windInz/D");
Tree_5->Branch("detVac_Inz",     &vacInz_5, "vacInz/D");
Tree_5->Branch("detSQ_ID",       &sqID_5, "sqID/I");
Tree_5->Branch("detTrig_ID",       &trigID_5, "trigID/I");
Tree_5->Branch("detWind_ID",       &windID_5, "windID/I");
Tree_5->Branch("detVac_ID",       &vacID_5, "vacID/I");

File_6 = new TFile(Form(Name_6.c_str()), "RECREATE");
Tree_6 = new TTree("simData", "Simulation Data");
Tree_6->Branch("detSQ_En",   &sqEn_6, "sqEn/D");
Tree_6->Branch("detTrig_En",   &trigEn_6, "trigEn/D");
Tree_6->Branch("detWind_En",   &windEn_6, "windEn/D");
Tree_6->Branch("detVac_En",   &vacEn_6, "vacEn/D");
Tree_6->Branch("detSQ_InEn",   &sqInEn_6, "sqInEn/D");
Tree_6->Branch("detTrig_InEn",   &trigInEn_6, "trigInEn/D");
Tree_6->Branch("detWind_InEn",   &windInEn_6, "windInEn/D");
Tree_6->Branch("detVac_InEn",   &vacInEn_6, "vacInEn/D");
Tree_6->Branch("detSQ_Inx",     &sqInx_6, "sqInx/D");
Tree_6->Branch("detTrig_Inx",     &trigInx_6, "trigInx/D");
Tree_6->Branch("detWind_Inx",     &windInx_6, "windInx/D");
Tree_6->Branch("detVac_Inx",     &vacInx_6, "vacInx/D");
Tree_6->Branch("detSQ_Iny",     &sqIny_6, "sqIny/D");
Tree_6->Branch("detTrig_Iny",     &trigIny_6, "trigIny/D");
Tree_6->Branch("detWind_Iny",     &windIny_6, "windIny/D");
Tree_6->Branch("detVac_Iny",     &vacIny_6, "vacIny/D");
Tree_6->Branch("detSQ_Inz",     &sqInz_6, "sqInz/D");
Tree_6->Branch("detTrig_Inz",     &trigInz_6, "trigInz/D");
Tree_6->Branch("detWind_Inz",     &windInz_6, "windInz/D");
Tree_6->Branch("detVac_Inz",     &vacInz_6, "vacInz/D");
Tree_6->Branch("detSQ_ID",       &sqID_6, "sqID/I");
Tree_6->Branch("detTrig_ID",       &trigID_6, "trigID/I");
Tree_6->Branch("detWind_ID",       &windID_6, "windID/I");
Tree_6->Branch("detVac_ID",       &vacID_6, "vacID/I");

File_7 = new TFile(Form(Name_7.c_str()), "RECREATE");
Tree_7 = new TTree("simData", "Simulation Data");
Tree_7->Branch("detSQ_En",   &sqEn_7, "sqEn/D");
Tree_7->Branch("detTrig_En",   &trigEn_7, "trigEn/D");
Tree_7->Branch("detWind_En",   &windEn_7, "windEn/D");
Tree_7->Branch("detVac_En",   &vacEn_7, "vacEn/D");
Tree_7->Branch("detSQ_InEn",   &sqInEn_7, "sqInEn/D");
Tree_7->Branch("detTrig_InEn",   &trigInEn_7, "trigInEn/D");
Tree_7->Branch("detWind_InEn",   &windInEn_7, "windInEn/D");
Tree_7->Branch("detVac_InEn",   &vacInEn_7, "vacInEn/D");
Tree_7->Branch("detSQ_Inx",     &sqInx_7, "sqInx/D");
Tree_7->Branch("detTrig_Inx",     &trigInx_7, "trigInx/D");
Tree_7->Branch("detWind_Inx",     &windInx_7, "windInx/D");
Tree_7->Branch("detVac_Inx",     &vacInx_7, "vacInx/D");
Tree_7->Branch("detSQ_Iny",     &sqIny_7, "sqIny/D");
Tree_7->Branch("detTrig_Iny",     &trigIny_7, "trigIny/D");
Tree_7->Branch("detWind_Iny",     &windIny_7, "windIny/D");
Tree_7->Branch("detVac_Iny",     &vacIny_7, "vacIny/D");
Tree_7->Branch("detSQ_Inz",     &sqInz_7, "sqInz/D");
Tree_7->Branch("detTrig_Inz",     &trigInz_7, "trigInz/D");
Tree_7->Branch("detWind_Inz",     &windInz_7, "windInz/D");
Tree_7->Branch("detVac_Inz",     &vacInz_7, "vacInz/D");
Tree_7->Branch("detSQ_ID",       &sqID_7, "sqID/I");
Tree_7->Branch("detTrig_ID",       &trigID_7, "trigID/I");
Tree_7->Branch("detWind_ID",       &windID_7, "windID/I");
Tree_7->Branch("detVac_ID",       &vacID_7, "vacID/I");

File_8 = new TFile(Form(Name_8.c_str()), "RECREATE");
Tree_8 = new TTree("simData", "Simulation Data");
Tree_8->Branch("detSQ_En",   &sqEn_8, "sqEn/D");
Tree_8->Branch("detTrig_En",   &trigEn_8, "trigEn/D");
Tree_8->Branch("detWind_En",   &windEn_8, "windEn/D");
Tree_8->Branch("detVac_En",   &vacEn_8, "vacEn/D");
Tree_8->Branch("detSQ_InEn",   &sqInEn_8, "sqInEn/D");
Tree_8->Branch("detTrig_InEn",   &trigInEn_8, "trigInEn/D");
Tree_8->Branch("detWind_InEn",   &windInEn_8, "windInEn/D");
Tree_8->Branch("detVac_InEn",   &vacInEn_8, "vacInEn/D");
Tree_8->Branch("detSQ_Inx",     &sqInx_8, "sqInx/D");
Tree_8->Branch("detTrig_Inx",     &trigInx_8, "trigInx/D");
Tree_8->Branch("detWind_Inx",     &windInx_8, "windInx/D");
Tree_8->Branch("detVac_Inx",     &vacInx_8, "vacInx/D");
Tree_8->Branch("detSQ_Iny",     &sqIny_8, "sqIny/D");
Tree_8->Branch("detTrig_Iny",     &trigIny_8, "trigIny/D");
Tree_8->Branch("detWind_Iny",     &windIny_8, "windIny/D");
Tree_8->Branch("detVac_Iny",     &vacIny_8, "vacIny/D");
Tree_8->Branch("detSQ_Inz",     &sqInz_8, "sqInz/D");
Tree_8->Branch("detTrig_Inz",     &trigInz_8, "trigInz/D");
Tree_8->Branch("detWind_Inz",     &windInz_8, "windInz/D");
Tree_8->Branch("detVac_Inz",     &vacInz_8, "vacInz/D");
Tree_8->Branch("detSQ_ID",       &sqID_8, "sqID/I");
Tree_8->Branch("detTrig_ID",       &trigID_8, "trigID/I");
Tree_8->Branch("detWind_ID",       &windID_8, "windID/I");
Tree_8->Branch("detVac_ID",       &vacID_8, "vacID/I");

File_9 = new TFile(Form(Name_9.c_str()), "RECREATE");
Tree_9 = new TTree("simData", "Simulation Data");
Tree_9->Branch("detSQ_En",   &sqEn_9, "sqEn/D");
Tree_9->Branch("detTrig_En",   &trigEn_9, "trigEn/D");
Tree_9->Branch("detWind_En",   &windEn_9, "windEn/D");
Tree_9->Branch("detVac_En",   &vacEn_9, "vacEn/D");
Tree_9->Branch("detSQ_InEn",   &sqInEn_9, "sqInEn/D");
Tree_9->Branch("detTrig_InEn",   &trigInEn_9, "trigInEn/D");
Tree_9->Branch("detWind_InEn",   &windInEn_9, "windInEn/D");
Tree_9->Branch("detVac_InEn",   &vacInEn_9, "vacInEn/D");
Tree_9->Branch("detSQ_Inx",     &sqInx_9, "sqInx/D");
Tree_9->Branch("detTrig_Inx",     &trigInx_9, "trigInx/D");
Tree_9->Branch("detWind_Inx",     &windInx_9, "windInx/D");
Tree_9->Branch("detVac_Inx",     &vacInx_9, "vacInx/D");
Tree_9->Branch("detSQ_Iny",     &sqIny_9, "sqIny/D");
Tree_9->Branch("detTrig_Iny",     &trigIny_9, "trigIny/D");
Tree_9->Branch("detWind_Iny",     &windIny_9, "windIny/D");
Tree_9->Branch("detVac_Iny",     &vacIny_9, "vacIny/D");
Tree_9->Branch("detSQ_Inz",     &sqInz_9, "sqInz/D");
Tree_9->Branch("detTrig_Inz",     &trigInz_9, "trigInz/D");
Tree_9->Branch("detWind_Inz",     &windInz_9, "windInz/D");
Tree_9->Branch("detVac_Inz",     &vacInz_9, "vacInz/D");
Tree_9->Branch("detSQ_ID",       &sqID_9, "sqID/I");
Tree_9->Branch("detTrig_ID",       &trigID_9, "trigID/I");
Tree_9->Branch("detWind_ID",       &windID_9, "windID/I");
Tree_9->Branch("detVac_ID",       &vacID_9, "vacID/I");

File_10 = new TFile(Form(Name_10.c_str()), "RECREATE");
Tree_10 = new TTree("simData", "Simulation Data");
Tree_10->Branch("detSQ_En",   &sqEn_10, "sqEn/D");
Tree_10->Branch("detTrig_En",   &trigEn_10, "trigEn/D");
Tree_10->Branch("detWind_En",   &windEn_10, "windEn/D");
Tree_10->Branch("detVac_En",   &vacEn_10, "vacEn/D");
Tree_10->Branch("detSQ_InEn",   &sqInEn_10, "sqInEn/D");
Tree_10->Branch("detTrig_InEn",   &trigInEn_10, "trigInEn/D");
Tree_10->Branch("detWind_InEn",   &windInEn_10, "windInEn/D");
Tree_10->Branch("detVac_InEn",   &vacInEn_10, "vacInEn/D");
Tree_10->Branch("detSQ_Inx",     &sqInx_10, "sqInx/D");
Tree_10->Branch("detTrig_Inx",     &trigInx_10, "trigInx/D");
Tree_10->Branch("detWind_Inx",     &windInx_10, "windInx/D");
Tree_10->Branch("detVac_Inx",     &vacInx_10, "vacInx/D");
Tree_10->Branch("detSQ_Iny",     &sqIny_10, "sqIny/D");
Tree_10->Branch("detTrig_Iny",     &trigIny_10, "trigIny/D");
Tree_10->Branch("detWind_Iny",     &windIny_10, "windIny/D");
Tree_10->Branch("detVac_Iny",     &vacIny_10, "vacIny/D");
Tree_10->Branch("detSQ_Inz",     &sqInz_10, "sqInz/D");
Tree_10->Branch("detTrig_Inz",     &trigInz_10, "trigInz/D");
Tree_10->Branch("detWind_Inz",     &windInz_10, "windInz/D");
Tree_10->Branch("detVac_Inz",     &vacInz_10, "vacInz/D");
Tree_10->Branch("detSQ_ID",       &sqID_10, "sqID/I");
Tree_10->Branch("detTrig_ID",       &trigID_10, "trigID/I");
Tree_10->Branch("detWind_ID",       &windID_10, "windID/I");
Tree_10->Branch("detVac_ID",       &vacID_10, "vacID/I");

File_11 = new TFile(Form(Name_11.c_str()), "RECREATE");
Tree_11 = new TTree("simData", "Simulation Data");
Tree_11->Branch("detSQ_En",   &sqEn_11, "sqEn/D");
Tree_11->Branch("detTrig_En",   &trigEn_11, "trigEn/D");
Tree_11->Branch("detWind_En",   &windEn_11, "windEn/D");
Tree_11->Branch("detVac_En",   &vacEn_11, "vacEn/D");
Tree_11->Branch("detSQ_InEn",   &sqInEn_11, "sqInEn/D");
Tree_11->Branch("detTrig_InEn",   &trigInEn_11, "trigInEn/D");
Tree_11->Branch("detWind_InEn",   &windInEn_11, "windInEn/D");
Tree_11->Branch("detVac_InEn",   &vacInEn_11, "vacInEn/D");
Tree_11->Branch("detSQ_Inx",     &sqInx_11, "sqInx/D");
Tree_11->Branch("detTrig_Inx",     &trigInx_11, "trigInx/D");
Tree_11->Branch("detWind_Inx",     &windInx_11, "windInx/D");
Tree_11->Branch("detVac_Inx",     &vacInx_11, "vacInx/D");
Tree_11->Branch("detSQ_Iny",     &sqIny_11, "sqIny/D");
Tree_11->Branch("detTrig_Iny",     &trigIny_11, "trigIny/D");
Tree_11->Branch("detWind_Iny",     &windIny_11, "windIny/D");
Tree_11->Branch("detVac_Iny",     &vacIny_11, "vacIny/D");
Tree_11->Branch("detSQ_Inz",     &sqInz_11, "sqInz/D");
Tree_11->Branch("detTrig_Inz",     &trigInz_11, "trigInz/D");
Tree_11->Branch("detWind_Inz",     &windInz_11, "windInz/D");
Tree_11->Branch("detVac_Inz",     &vacInz_11, "vacInz/D");
Tree_11->Branch("detSQ_ID",       &sqID_11, "sqID/I");
Tree_11->Branch("detTrig_ID",       &trigID_11, "trigID/I");
Tree_11->Branch("detWind_ID",       &windID_11, "windID/I");
Tree_11->Branch("detVac_ID",       &vacID_11, "vacID/I");

File_12 = new TFile(Form(Name_12.c_str()), "RECREATE");
Tree_12 = new TTree("simData", "Simulation Data");
Tree_12->Branch("detSQ_En",   &sqEn_12, "sqEn/D");
Tree_12->Branch("detTrig_En",   &trigEn_12, "trigEn/D");
Tree_12->Branch("detWind_En",   &windEn_12, "windEn/D");
Tree_12->Branch("detVac_En",   &vacEn_12, "vacEn/D");
Tree_12->Branch("detSQ_InEn",   &sqInEn_12, "sqInEn/D");
Tree_12->Branch("detTrig_InEn",   &trigInEn_12, "trigInEn/D");
Tree_12->Branch("detWind_InEn",   &windInEn_12, "windInEn/D");
Tree_12->Branch("detVac_InEn",   &vacInEn_12, "vacInEn/D");
Tree_12->Branch("detSQ_Inx",     &sqInx_12, "sqInx/D");
Tree_12->Branch("detTrig_Inx",     &trigInx_12, "trigInx/D");
Tree_12->Branch("detWind_Inx",     &windInx_12, "windInx/D");
Tree_12->Branch("detVac_Inx",     &vacInx_12, "vacInx/D");
Tree_12->Branch("detSQ_Iny",     &sqIny_12, "sqIny/D");
Tree_12->Branch("detTrig_Iny",     &trigIny_12, "trigIny/D");
Tree_12->Branch("detWind_Iny",     &windIny_12, "windIny/D");
Tree_12->Branch("detVac_Iny",     &vacIny_12, "vacIny/D");
Tree_12->Branch("detSQ_Inz",     &sqInz_12, "sqInz/D");
Tree_12->Branch("detTrig_Inz",     &trigInz_12, "trigInz/D");
Tree_12->Branch("detWind_Inz",     &windInz_12, "windInz/D");
Tree_12->Branch("detVac_Inz",     &vacInz_12, "vacInz/D");
Tree_12->Branch("detSQ_ID",       &sqID_12, "sqID/I");
Tree_12->Branch("detTrig_ID",       &trigID_12, "trigID/I");
Tree_12->Branch("detWind_ID",       &windID_12, "windID/I");
Tree_12->Branch("detVac_ID",       &vacID_12, "vacID/I");

File_13 = new TFile(Form(Name_13.c_str()), "RECREATE");
Tree_13 = new TTree("simData", "Simulation Data");
Tree_13->Branch("detSQ_En",   &sqEn_13, "sqEn/D");
Tree_13->Branch("detTrig_En",   &trigEn_13, "trigEn/D");
Tree_13->Branch("detWind_En",   &windEn_13, "windEn/D");
Tree_13->Branch("detVac_En",   &vacEn_13, "vacEn/D");
Tree_13->Branch("detSQ_InEn",   &sqInEn_13, "sqInEn/D");
Tree_13->Branch("detTrig_InEn",   &trigInEn_13, "trigInEn/D");
Tree_13->Branch("detWind_InEn",   &windInEn_13, "windInEn/D");
Tree_13->Branch("detVac_InEn",   &vacInEn_13, "vacInEn/D");
Tree_13->Branch("detSQ_Inx",     &sqInx_13, "sqInx/D");
Tree_13->Branch("detTrig_Inx",     &trigInx_13, "trigInx/D");
Tree_13->Branch("detWind_Inx",     &windInx_13, "windInx/D");
Tree_13->Branch("detVac_Inx",     &vacInx_13, "vacInx/D");
Tree_13->Branch("detSQ_Iny",     &sqIny_13, "sqIny/D");
Tree_13->Branch("detTrig_Iny",     &trigIny_13, "trigIny/D");
Tree_13->Branch("detWind_Iny",     &windIny_13, "windIny/D");
Tree_13->Branch("detVac_Iny",     &vacIny_13, "vacIny/D");
Tree_13->Branch("detSQ_Inz",     &sqInz_13, "sqInz/D");
Tree_13->Branch("detTrig_Inz",     &trigInz_13, "trigInz/D");
Tree_13->Branch("detWind_Inz",     &windInz_13, "windInz/D");
Tree_13->Branch("detVac_Inz",     &vacInz_13, "vacInz/D");
Tree_13->Branch("detSQ_ID",       &sqID_13, "sqID/I");
Tree_13->Branch("detTrig_ID",       &trigID_13, "trigID/I");
Tree_13->Branch("detWind_ID",       &windID_13, "windID/I");
Tree_13->Branch("detVac_ID",       &vacID_13, "vacID/I");

File_14 = new TFile(Form(Name_14.c_str()), "RECREATE");
Tree_14 = new TTree("simData", "Simulation Data");
Tree_14->Branch("detSQ_En",   &sqEn_14, "sqEn/D");
Tree_14->Branch("detTrig_En",   &trigEn_14, "trigEn/D");
Tree_14->Branch("detWind_En",   &windEn_14, "windEn/D");
Tree_14->Branch("detVac_En",   &vacEn_14, "vacEn/D");
Tree_14->Branch("detSQ_InEn",   &sqInEn_14, "sqInEn/D");
Tree_14->Branch("detTrig_InEn",   &trigInEn_14, "trigInEn/D");
Tree_14->Branch("detWind_InEn",   &windInEn_14, "windInEn/D");
Tree_14->Branch("detVac_InEn",   &vacInEn_14, "vacInEn/D");
Tree_14->Branch("detSQ_Inx",     &sqInx_14, "sqInx/D");
Tree_14->Branch("detTrig_Inx",     &trigInx_14, "trigInx/D");
Tree_14->Branch("detWind_Inx",     &windInx_14, "windInx/D");
Tree_14->Branch("detVac_Inx",     &vacInx_14, "vacInx/D");
Tree_14->Branch("detSQ_Iny",     &sqIny_14, "sqIny/D");
Tree_14->Branch("detTrig_Iny",     &trigIny_14, "trigIny/D");
Tree_14->Branch("detWind_Iny",     &windIny_14, "windIny/D");
Tree_14->Branch("detVac_Iny",     &vacIny_14, "vacIny/D");
Tree_14->Branch("detSQ_Inz",     &sqInz_14, "sqInz/D");
Tree_14->Branch("detTrig_Inz",     &trigInz_14, "trigInz/D");
Tree_14->Branch("detWind_Inz",     &windInz_14, "windInz/D");
Tree_14->Branch("detVac_Inz",     &vacInz_14, "vacInz/D");
Tree_14->Branch("detSQ_ID",       &sqID_14, "sqID/I");
Tree_14->Branch("detTrig_ID",       &trigID_14, "trigID/I");
Tree_14->Branch("detWind_ID",       &windID_14, "windID/I");
Tree_14->Branch("detVac_ID",       &vacID_14, "vacID/I");

File_15 = new TFile(Form(Name_15.c_str()), "RECREATE");
Tree_15 = new TTree("simData", "Simulation Data");
Tree_15->Branch("detSQ_En",   &sqEn_15, "sqEn/D");
Tree_15->Branch("detTrig_En",   &trigEn_15, "trigEn/D");
Tree_15->Branch("detWind_En",   &windEn_15, "windEn/D");
Tree_15->Branch("detVac_En",   &vacEn_15, "vacEn/D");
Tree_15->Branch("detSQ_InEn",   &sqInEn_15, "sqInEn/D");
Tree_15->Branch("detTrig_InEn",   &trigInEn_15, "trigInEn/D");
Tree_15->Branch("detWind_InEn",   &windInEn_15, "windInEn/D");
Tree_15->Branch("detVac_InEn",   &vacInEn_15, "vacInEn/D");
Tree_15->Branch("detSQ_Inx",     &sqInx_15, "sqInx/D");
Tree_15->Branch("detTrig_Inx",     &trigInx_15, "trigInx/D");
Tree_15->Branch("detWind_Inx",     &windInx_15, "windInx/D");
Tree_15->Branch("detVac_Inx",     &vacInx_15, "vacInx/D");
Tree_15->Branch("detSQ_Iny",     &sqIny_15, "sqIny/D");
Tree_15->Branch("detTrig_Iny",     &trigIny_15, "trigIny/D");
Tree_15->Branch("detWind_Iny",     &windIny_15, "windIny/D");
Tree_15->Branch("detVac_Iny",     &vacIny_15, "vacIny/D");
Tree_15->Branch("detSQ_Inz",     &sqInz_15, "sqInz/D");
Tree_15->Branch("detTrig_Inz",     &trigInz_15, "trigInz/D");
Tree_15->Branch("detWind_Inz",     &windInz_15, "windInz/D");
Tree_15->Branch("detVac_Inz",     &vacInz_15, "vacInz/D");
Tree_15->Branch("detSQ_ID",       &sqID_15, "sqID/I");
Tree_15->Branch("detTrig_ID",       &trigID_15, "trigID/I");
Tree_15->Branch("detWind_ID",       &windID_15, "windID/I");
Tree_15->Branch("detVac_ID",       &vacID_15, "vacID/I");
}

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

}void BM_Output::setSQParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_0 = en;
sqInEn_0 = inen;
sqInx_0 = inx;
sqIny_0 = iny;
sqInz_0 = inz;
sqID_0 = iD;
}
void BM_Output::setTrigParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_0 = en;
trigInEn_0 = inen;
trigInx_0 = inx;
trigIny_0 = iny;
trigInz_0 = inz;
trigID_0 = iD;
}
void BM_Output::setWindParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_0 = en;
windInEn_0 = inen;
windInx_0 = inx;
windIny_0 = iny;
windInz_0 = inz;
windID_0 = iD;
}
void BM_Output::setVacParams0(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_0 = en;
vacInEn_0 = inen;
vacInx_0 = inx;
vacIny_0 = iny;
vacInz_0 = inz;
vacID_0 = iD;
}
void BM_Output::setSQParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_1 = en;
sqInEn_1 = inen;
sqInx_1 = inx;
sqIny_1 = iny;
sqInz_1 = inz;
sqID_1 = iD;
}
void BM_Output::setTrigParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_1 = en;
trigInEn_1 = inen;
trigInx_1 = inx;
trigIny_1 = iny;
trigInz_1 = inz;
trigID_1 = iD;
}
void BM_Output::setWindParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_1 = en;
windInEn_1 = inen;
windInx_1 = inx;
windIny_1 = iny;
windInz_1 = inz;
windID_1 = iD;
}
void BM_Output::setVacParams1(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_1 = en;
vacInEn_1 = inen;
vacInx_1 = inx;
vacIny_1 = iny;
vacInz_1 = inz;
vacID_1 = iD;
}
void BM_Output::setSQParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_2 = en;
sqInEn_2 = inen;
sqInx_2 = inx;
sqIny_2 = iny;
sqInz_2 = inz;
sqID_2 = iD;
}
void BM_Output::setTrigParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_2 = en;
trigInEn_2 = inen;
trigInx_2 = inx;
trigIny_2 = iny;
trigInz_2 = inz;
trigID_2 = iD;
}
void BM_Output::setWindParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_2 = en;
windInEn_2 = inen;
windInx_2 = inx;
windIny_2 = iny;
windInz_2 = inz;
windID_2 = iD;
}
void BM_Output::setVacParams2(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_2 = en;
vacInEn_2 = inen;
vacInx_2 = inx;
vacIny_2 = iny;
vacInz_2 = inz;
vacID_2 = iD;
}
void BM_Output::setSQParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_3 = en;
sqInEn_3 = inen;
sqInx_3 = inx;
sqIny_3 = iny;
sqInz_3 = inz;
sqID_3 = iD;
}
void BM_Output::setTrigParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_3 = en;
trigInEn_3 = inen;
trigInx_3 = inx;
trigIny_3 = iny;
trigInz_3 = inz;
trigID_3 = iD;
}
void BM_Output::setWindParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_3 = en;
windInEn_3 = inen;
windInx_3 = inx;
windIny_3 = iny;
windInz_3 = inz;
windID_3 = iD;
}
void BM_Output::setVacParams3(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_3 = en;
vacInEn_3 = inen;
vacInx_3 = inx;
vacIny_3 = iny;
vacInz_3 = inz;
vacID_3 = iD;
}
void BM_Output::setSQParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_4 = en;
sqInEn_4 = inen;
sqInx_4 = inx;
sqIny_4 = iny;
sqInz_4 = inz;
sqID_4 = iD;
}
void BM_Output::setTrigParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_4 = en;
trigInEn_4 = inen;
trigInx_4 = inx;
trigIny_4 = iny;
trigInz_4 = inz;
trigID_4 = iD;
}
void BM_Output::setWindParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_4 = en;
windInEn_4 = inen;
windInx_4 = inx;
windIny_4 = iny;
windInz_4 = inz;
windID_4 = iD;
}
void BM_Output::setVacParams4(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_4 = en;
vacInEn_4 = inen;
vacInx_4 = inx;
vacIny_4 = iny;
vacInz_4 = inz;
vacID_4 = iD;
}
void BM_Output::setSQParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_5 = en;
sqInEn_5 = inen;
sqInx_5 = inx;
sqIny_5 = iny;
sqInz_5 = inz;
sqID_5 = iD;
}
void BM_Output::setTrigParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_5 = en;
trigInEn_5 = inen;
trigInx_5 = inx;
trigIny_5 = iny;
trigInz_5 = inz;
trigID_5 = iD;
}
void BM_Output::setWindParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_5 = en;
windInEn_5 = inen;
windInx_5 = inx;
windIny_5 = iny;
windInz_5 = inz;
windID_5 = iD;
}
void BM_Output::setVacParams5(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_5 = en;
vacInEn_5 = inen;
vacInx_5 = inx;
vacIny_5 = iny;
vacInz_5 = inz;
vacID_5 = iD;
}
void BM_Output::setSQParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_6 = en;
sqInEn_6 = inen;
sqInx_6 = inx;
sqIny_6 = iny;
sqInz_6 = inz;
sqID_6 = iD;
}
void BM_Output::setTrigParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_6 = en;
trigInEn_6 = inen;
trigInx_6 = inx;
trigIny_6 = iny;
trigInz_6 = inz;
trigID_6 = iD;
}
void BM_Output::setWindParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_6 = en;
windInEn_6 = inen;
windInx_6 = inx;
windIny_6 = iny;
windInz_6 = inz;
windID_6 = iD;
}
void BM_Output::setVacParams6(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_6 = en;
vacInEn_6 = inen;
vacInx_6 = inx;
vacIny_6 = iny;
vacInz_6 = inz;
vacID_6 = iD;
}
void BM_Output::setSQParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_7 = en;
sqInEn_7 = inen;
sqInx_7 = inx;
sqIny_7 = iny;
sqInz_7 = inz;
sqID_7 = iD;
}
void BM_Output::setTrigParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_7 = en;
trigInEn_7 = inen;
trigInx_7 = inx;
trigIny_7 = iny;
trigInz_7 = inz;
trigID_7 = iD;
}
void BM_Output::setWindParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_7 = en;
windInEn_7 = inen;
windInx_7 = inx;
windIny_7 = iny;
windInz_7 = inz;
windID_7 = iD;
}
void BM_Output::setVacParams7(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_7 = en;
vacInEn_7 = inen;
vacInx_7 = inx;
vacIny_7 = iny;
vacInz_7 = inz;
vacID_7 = iD;
}
void BM_Output::setSQParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_8 = en;
sqInEn_8 = inen;
sqInx_8 = inx;
sqIny_8 = iny;
sqInz_8 = inz;
sqID_8 = iD;
}
void BM_Output::setTrigParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_8 = en;
trigInEn_8 = inen;
trigInx_8 = inx;
trigIny_8 = iny;
trigInz_8 = inz;
trigID_8 = iD;
}
void BM_Output::setWindParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_8 = en;
windInEn_8 = inen;
windInx_8 = inx;
windIny_8 = iny;
windInz_8 = inz;
windID_8 = iD;
}
void BM_Output::setVacParams8(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_8 = en;
vacInEn_8 = inen;
vacInx_8 = inx;
vacIny_8 = iny;
vacInz_8 = inz;
vacID_8 = iD;
}
void BM_Output::setSQParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_9 = en;
sqInEn_9 = inen;
sqInx_9 = inx;
sqIny_9 = iny;
sqInz_9 = inz;
sqID_9 = iD;
}
void BM_Output::setTrigParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_9 = en;
trigInEn_9 = inen;
trigInx_9 = inx;
trigIny_9 = iny;
trigInz_9 = inz;
trigID_9 = iD;
}
void BM_Output::setWindParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_9 = en;
windInEn_9 = inen;
windInx_9 = inx;
windIny_9 = iny;
windInz_9 = inz;
windID_9 = iD;
}
void BM_Output::setVacParams9(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_9 = en;
vacInEn_9 = inen;
vacInx_9 = inx;
vacIny_9 = iny;
vacInz_9 = inz;
vacID_9 = iD;
}
void BM_Output::setSQParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_10 = en;
sqInEn_10 = inen;
sqInx_10 = inx;
sqIny_10 = iny;
sqInz_10 = inz;
sqID_10 = iD;
}
void BM_Output::setTrigParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_10 = en;
trigInEn_10 = inen;
trigInx_10 = inx;
trigIny_10 = iny;
trigInz_10 = inz;
trigID_10 = iD;
}
void BM_Output::setWindParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_10 = en;
windInEn_10 = inen;
windInx_10 = inx;
windIny_10 = iny;
windInz_10 = inz;
windID_10 = iD;
}
void BM_Output::setVacParams10(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_10 = en;
vacInEn_10 = inen;
vacInx_10 = inx;
vacIny_10 = iny;
vacInz_10 = inz;
vacID_10 = iD;
}
void BM_Output::setSQParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_11 = en;
sqInEn_11 = inen;
sqInx_11 = inx;
sqIny_11 = iny;
sqInz_11 = inz;
sqID_11 = iD;
}
void BM_Output::setTrigParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_11 = en;
trigInEn_11 = inen;
trigInx_11 = inx;
trigIny_11 = iny;
trigInz_11 = inz;
trigID_11 = iD;
}
void BM_Output::setWindParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_11 = en;
windInEn_11 = inen;
windInx_11 = inx;
windIny_11 = iny;
windInz_11 = inz;
windID_11 = iD;
}
void BM_Output::setVacParams11(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_11 = en;
vacInEn_11 = inen;
vacInx_11 = inx;
vacIny_11 = iny;
vacInz_11 = inz;
vacID_11 = iD;
}
void BM_Output::setSQParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_12 = en;
sqInEn_12 = inen;
sqInx_12 = inx;
sqIny_12 = iny;
sqInz_12 = inz;
sqID_12 = iD;
}
void BM_Output::setTrigParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_12 = en;
trigInEn_12 = inen;
trigInx_12 = inx;
trigIny_12 = iny;
trigInz_12 = inz;
trigID_12 = iD;
}
void BM_Output::setWindParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_12 = en;
windInEn_12 = inen;
windInx_12 = inx;
windIny_12 = iny;
windInz_12 = inz;
windID_12 = iD;
}
void BM_Output::setVacParams12(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_12 = en;
vacInEn_12 = inen;
vacInx_12 = inx;
vacIny_12 = iny;
vacInz_12 = inz;
vacID_12 = iD;
}
void BM_Output::setSQParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_13 = en;
sqInEn_13 = inen;
sqInx_13 = inx;
sqIny_13 = iny;
sqInz_13 = inz;
sqID_13 = iD;
}
void BM_Output::setTrigParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_13 = en;
trigInEn_13 = inen;
trigInx_13 = inx;
trigIny_13 = iny;
trigInz_13 = inz;
trigID_13 = iD;
}
void BM_Output::setWindParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_13 = en;
windInEn_13 = inen;
windInx_13 = inx;
windIny_13 = iny;
windInz_13 = inz;
windID_13 = iD;
}
void BM_Output::setVacParams13(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_13 = en;
vacInEn_13 = inen;
vacInx_13 = inx;
vacIny_13 = iny;
vacInz_13 = inz;
vacID_13 = iD;
}
void BM_Output::setSQParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_14 = en;
sqInEn_14 = inen;
sqInx_14 = inx;
sqIny_14 = iny;
sqInz_14 = inz;
sqID_14 = iD;
}
void BM_Output::setTrigParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_14 = en;
trigInEn_14 = inen;
trigInx_14 = inx;
trigIny_14 = iny;
trigInz_14 = inz;
trigID_14 = iD;
}
void BM_Output::setWindParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_14 = en;
windInEn_14 = inen;
windInx_14 = inx;
windIny_14 = iny;
windInz_14 = inz;
windID_14 = iD;
}
void BM_Output::setVacParams14(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_14 = en;
vacInEn_14 = inen;
vacInx_14 = inx;
vacIny_14 = iny;
vacInz_14 = inz;
vacID_14 = iD;
}
void BM_Output::setSQParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{sqEn_15 = en;
sqInEn_15 = inen;
sqInx_15 = inx;
sqIny_15 = iny;
sqInz_15 = inz;
sqID_15 = iD;
}
void BM_Output::setTrigParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{trigEn_15 = en;
trigInEn_15 = inen;
trigInx_15 = inx;
trigIny_15 = iny;
trigInz_15 = inz;
trigID_15 = iD;
}
void BM_Output::setWindParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{windEn_15 = en;
windInEn_15 = inen;
windInx_15 = inx;
windIny_15 = iny;
windInz_15 = inz;
windID_15 = iD;
}
void BM_Output::setVacParams15(bool hit, int iD, int IDNew, double en, double enPhot, double enPos, double enElec, double enOther, double enAnn, int exitAnnCounter, double x, double y, double z, double t, double inen, double inx, double iny, double inz, double annix, double anniy, double anniz, double escx, double escy, double escz, double finEn, bool exited, int exitPhotCounter, int exitPosCounter, int exitElecCounter, int exitOthCounter)//, //double theta,
{vacEn_15 = en;
vacInEn_15 = inen;
vacInx_15 = inx;
vacIny_15 = iny;
vacInz_15 = inz;
vacID_15 = iD;
}
