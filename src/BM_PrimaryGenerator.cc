#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <complex>
#include <iostream>
#include <algorithm>
#include <vector>

#include "nat_units.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "FileReader.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleMomentum.hh"
#include "G4Event.hh"
#include "G4IonTable.hh"
#include "G4AutoLock.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "BM_PrimaryGenerator.hh"
#include "BM_EventAction.hh"

using namespace std;

BM_PrimaryGenerator::BM_PrimaryGenerator()
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(0),
      heParticleGun(0),
      fEnvelopeBox(0)
{
  G4int n_particle = 1;

  // Bi Generation
  // set up He/Ne particle generation
  
  fParticleGun = new G4ParticleGun(n_particle);
  heParticleGun = new G4ParticleGun(n_particle);
  
}

BM_PrimaryGenerator::~BM_PrimaryGenerator()
{
  delete fParticleGun;
  delete heParticleGun;
}

void BM_PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
  // this function is called at the begining of each event

  // In order to avoid dependence of PrimaryGeneratorAction on DetectorConstruction class we get Envelope volume from G4LogicalVolumeStore.

  // G4cout << "Generator 1" << G4endl;
  G4double branchEn[23] = {511, 897.77, 1442.2, 1770.228, 1063.656, 569.698, 324.25, 1438.35, 312.24, 1426.34, 893.92, 240.10, 1354.20, 881.91, 809.77, 1754.367, 1682.224, 565.8473, 1059.805, 553.8372, 481.6935, 1047.795, 975.651};
  G4double branchIn[23] = {0.076, 0.128, 0.131, 6.87, 74.5, 97.75, 7.50e-06, 0.0000144, 3.20e-05, 6.13e-05, 9.50e-05, 1.88e-04, 3.55e-04, 4.07e-04, 0.00246, 0.0034, 0.0238, 0.111, 0.44, 0.442, 1.537, 1.84, 7.08};
  G4double max_randomBi = 0.0;
  G4double num_gammas = 6;
  G4double num_electrons = 17;
  G4double num_branch = num_gammas + num_electrons;
  // max_random = accumulate(branchIn, branchIn+num_branch, max_random);
  srand(time(NULL));
  double rando = rand() % 100000000;
  double randnew = rando / 100000000;
  G4double randnew2 = 0;

  branchIn[1] = branchIn[1] + branchIn[0];
  branchIn[2] = branchIn[2] + branchIn[1];
  branchIn[3] = branchIn[3] + branchIn[2];
  branchIn[4] = branchIn[4] + branchIn[3];
  branchIn[5] = branchIn[5] + branchIn[4];
  branchIn[6] = branchIn[6] + branchIn[5];
  branchIn[7] = branchIn[7] + branchIn[6];
  branchIn[8] = branchIn[8] + branchIn[7];
  branchIn[9] = branchIn[9] + branchIn[8];
  branchIn[10] = branchIn[10] + branchIn[9];
  branchIn[11] = branchIn[11] + branchIn[10];
  branchIn[12] = branchIn[12] + branchIn[11];
  branchIn[13] = branchIn[13] + branchIn[12];
  branchIn[14] = branchIn[14] + branchIn[13];
  branchIn[15] = branchIn[15] + branchIn[14];
  branchIn[16] = branchIn[16] + branchIn[15];
  branchIn[17] = branchIn[17] + branchIn[16];
  branchIn[18] = branchIn[18] + branchIn[17];
  branchIn[19] = branchIn[19] + branchIn[18];
  branchIn[20] = branchIn[20] + branchIn[19];
  branchIn[21] = branchIn[21] + branchIn[20];
  branchIn[22] = branchIn[22] + branchIn[21];
  max_randomBi = branchIn[22];
  randnew2 = randnew * max_randomBi;
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  if (randnew2 <= branchIn[0])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[0] * keV);
  }
  else if (randnew2 <= branchIn[1])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[1] * keV);
  }
  else if (randnew2 <= branchIn[2])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[2] * keV);
  }
  else if (randnew2 <= branchIn[3])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[3] * keV);
  }
  else if (randnew2 <= branchIn[4])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[4] * keV);
  }
  else if (randnew2 <= branchIn[5])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[5] * keV);
  }
  else if (randnew2 <= branchIn[6])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[6] * keV);
  }
  else if (randnew2 <= branchIn[7])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[7] * keV);
  }
  else if (randnew2 <= branchIn[8])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[8] * keV);
  }
  else if (randnew2 <= branchIn[9])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[9] * keV);
  }
  else if (randnew2 <= branchIn[10])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[10] * keV);
  }
  else if (randnew2 <= branchIn[11])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[11] * keV);
  }
  else if (randnew2 <= branchIn[12])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[12] * keV);
  }
  else if (randnew2 <= branchIn[13])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[13] * keV);
  }
  else if (randnew2 <= branchIn[14])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[14] * keV);
  }
  else if (randnew2 <= branchIn[15])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[15] * keV);
  }
  else if (randnew2 <= branchIn[16])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[16] * keV);
  }
  else if (randnew2 <= branchIn[17])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[17] * keV);
  }
  else if (randnew2 <= branchIn[18])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[18] * keV);
  }
  else if (randnew2 <= branchIn[19])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[19] * keV);
  }
  else if (randnew2 <= branchIn[20])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[20] * keV);
  }
  else if (randnew2 <= branchIn[21])
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[21] * keV);
  }
  else
  {
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(branchEn[22] * keV);
  }

  G4double u = sqrt((1.0 / 2) * (1.0 / 2) * G4UniformRand()) * cm;
  G4double v = 2 * 3.141592653 * G4UniformRand();
  G4double x0 = 1.60274 / 2 * cm * (2 * G4UniformRand() - 1) * cos(v); // 2.68205602104/2*cm + u * cos (v);
  G4double y0 = 1.60274 / 2 * cm * (2 * G4UniformRand() - 1) * sin(v); // 2.68205602104/2*cm + u * sin (v);
  G4double randy = G4UniformRand();
  G4double zBi = -3.4873 * cm;
  if (randy > 0.5)
  {
    zBi = -3.4873 * cm; // near//-5.15112*cm-0.00064*cm;//far//-1.66*cm;//Bi//
  }
  else
  {
    zBi = -3.4873 * cm - 0.3437 * cm; // near//-5.15112*cm-0.00064*cm;//far//-1.66*cm;//Bi//
  }
  G4double z0 = -3.4873 * cm - 0.00064 * cm; // near//-5.15112*cm-0.00064*cm;//far//-1.66*cm;//Bi//
  // G4cout << "Generator 2" << G4endl;
  //  G4cout << x0 << " " << y0 << G4endl;

  G4double vx0 = (G4UniformRand() - 0.5) * 2;
  G4double vy0 = 2 * G4UniformRand() - 1;
  G4double vz0 = 2 * G4UniformRand() - 1; // G4UniformRand() for a hemisphere
  G4double t0 = 10 * G4UniformRand();
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx0, vy0, vz0));
  fParticleGun->SetParticleTime(t0);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, zBi));

  // straight pipe generator
  G4double rad_new = 0.9398; // 3.4798/2*cm beam rad;Source Radius 0.9398
  u = sqrt((rad_new / 2) * (rad_new) / 2 * G4UniformRand()) * cm;
  v = 2 * 3.141592653 * G4UniformRand();
  G4double x1 = 0 / 2 * cm + u * cos(v); //-2.68205602104
  G4double y1 = 0 / 2 * cm + u * sin(v);
  G4double z1 = 1.27 * cm + G4UniformRand() * 16.51 * cm; // 11.13765*cm-2.31115*cm+(G4UniformRand())*16*cm/2;

  G4double vx1 = 2 * G4UniformRand() - 1;
  G4double vy1 = 2 * G4UniformRand() - 1;
  G4double vz2 = -2 * G4UniformRand() + 1;
  G4double t02 = 10 * G4UniformRand();

  // T pipe generator
  G4double randall = 2 * G4UniformRand() - 1;
  G4double H1 = 6.75 * 2.54;     // make sure this agrees with Tdv_H1 in BM_Detector.cc
  G4double h2 = 6.75 * 2.54 / 2; // make sure this agrees with Tdv_h2 in BM_Detector.cc
  G4bool okay = false;
  G4double rat = (h2 + rad_new / 2) / (H1 + h2 + rad_new);
  
  // u = sqrt ((rad_new/2) * (rad_new)/2 * G4UniformRand())*cm;
  // v = 2 * 3.141592653* G4UniformRand();
  // G4double z2 = 0/2*cm + u * cos (v);//-2.68205602104
  // G4double y2 = 0/2*cm + u * sin (v);
  // G4double x2 = -1* G4UniformRand()*h2*cm;
  // G4double r2 = sqrt(y2*y2 + x2*x2);

  // Tpipe position Generator  /////////////////////////////
  //  if (randall < rat){
  //    while (okay != true){
  //      u = sqrt ((rad_new/2) * (rad_new)/2 * G4UniformRand())*cm;
  //      v = 2 * 3.141592653* G4UniformRand();
  //      G4double z2 = (1.27*4+6.75*2.54)/2*cm + u * cos (v);//-2.68205602104 3.4798/2*cm - 6.75*2.54/2*cm//(-1.27+16.51)/2*cm
  //      G4double y2 = 0/2*cm + u * sin (v);
  //      G4double x2 = -1* G4UniformRand()*h2*cm;
  //      G4double r2 = sqrt(y2*y2 + x2*x2);
  //     if (r2 > rad_new){
  //       okay = true;
  //       heParticleGun->SetParticlePosition(G4ThreeVector(x2,y2,z2));// T pipe
  //     }
  //   }
  // }
  // else {
  // // if(randall >= rat){
  //   u = sqrt ((rad_new/2) * (rad_new)/2 * G4UniformRand())*cm;
  //   v = 2 * 3.141592653* G4UniformRand();
  //   G4double x2 = 0/2*cm + u * cos (v);//-2.68205602104
  //   G4double y2 = 0/2*cm + u * sin (v);
  //   G4double z2 = 2*1.27*cm + G4UniformRand()*(6.75*2.54)*cm;
  //   heParticleGun->SetParticlePosition(G4ThreeVector(x2,y2,z2));// T pipe
  // }

  heParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx1, vy1, vz2)); // pipe
  // heParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));//pencil
  heParticleGun->SetParticleTime(t02);
  // heParticleGun->SetParticlePosition(G4ThreeVector(x1,y1,z1));//pipe
  // heParticleGun->SetParticlePosition(G4ThreeVector(0,0,0.45*cm));//-9.35*cm));//pencil
  heParticleGun->SetParticlePosition(G4ThreeVector(-x1, -y1, z0)); // z0 for source

  // He Generation (e-)

  srand(time(NULL));
  double rando2 = rand() % 100000000;
  double randnew4 = 0.;

  FileReader ffr = FileReader();
  ffr.GetAnEvent();
  
  vector<double> ncEn = FileReader().cEn;
  vector<double> ncIn = FileReader().cIn;
  randnew4 = ncIn.back() * rando2 / 100000000;
  vector<double>::iterator low;
  low = lower_bound(ncIn.begin(), ncIn.end(), randnew4);
  double n = low - ncIn.begin();
  // G4cout << ncEn.at(n) << G4endl; // access example
  
  // He/Sr is e-
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
  
  // Ne is e+
  // G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e+");
  // G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
  
  heParticleGun->SetParticleDefinition(particle);
  heParticleGun->SetParticleEnergy(ncEn.at(n) * eV);

  heParticleGun->GeneratePrimaryVertex(anEvent); // gas source
  // fParticleGun->GeneratePrimaryVertex(anEvent); //table source
}
