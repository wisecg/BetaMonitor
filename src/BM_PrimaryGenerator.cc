#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>

#include "nat_units.hh"
#include "globals.hh"
#include "Randomize.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleMomentum.hh"
// #include "G4Event.hh"
// #include "G4IonTable.hh"
// #include "G4AutoLock.hh"
// #include "G4LogicalVolumeStore.hh"
// #include "G4LogicalVolume.hh"
// #include "G4Box.hh"
// #include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "BM_PrimaryGenerator.hh"
#include "BM_EventAction.hh"

using namespace std;

BM_PrimaryGenerator::BM_PrimaryGenerator() : G4VUserPrimaryGeneratorAction(),
                                             fParticleGun(0)
{
  G4int n_particle = 1;

  // He/Ne (monoenergetic) particle generation - also used for Sr90 betas
  fParticleGun = new G4ParticleGun(n_particle);

  // FIXME: pass the name of the input file (energy & intensity columns)

  // input the CDF of a beta decay (energy & intensity columns)
  // the intensity must be monotonically increasing, but not necessarily normalized to [0,1]

  // std::ifstream inputFile ("6HeDecay5.txt");
  // std::ifstream inputFile ("19NeDecay.txt");
  // std::ifstream inputFile ("Mono1MeV.txt");
  // std::ifstream inputFile ("133BaGammas.txt");
  // std::ifstream inputFile ("137CsGammas.txt");
  // std::ifstream inputFile ("60CoGammas.txt");
  // std::ifstream inputFile("90Sr90YDecaycdf.txt");
  std::ifstream inputFile ("../dat/90SrDecay_cdf.txt");
  // std::ifstream inputFile ("90YDecaypdf.txt");
  // std::ifstream inputFile("He6Betas-test.txt");

  if (!inputFile.is_open()) {
    std::cerr << "ERROR: Could not open input file" << std::endl;
    exit(1);
  }

  std::filesystem::path currentPath = std::filesystem::current_path();
  std::cout << "Current working directory: " << currentPath << std::endl;

  double a, b;
  while (inputFile >> a >> b) {
    cEn.push_back(a);
    cIn.push_back(b);
  } 
  inputFile.close();

}

BM_PrimaryGenerator::~BM_PrimaryGenerator()
{
  delete fParticleGun;
}

void BM_PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
  /*
  For each primary, we need to call SetParticleTime, SetParticlePosition, 
  SetParticleMomentumDirection, and GeneratePrimaryVertex.
  */
  fParticleGun->SetParticleTime(10 * G4UniformRand());

  bool cal_source = true;
  bool gas_source = false;

  // === set particle type, energy, and intensity === 

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  // He/Sr is e-
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
  
  // Ne is e+
  // G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e+");
  // G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
  
  fParticleGun->SetParticleDefinition(particle);

  // sample from the CDF of the beta decay spectrum (input file)
  srand(time(NULL));
  double r1 = (double)rand() / RAND_MAX; // [0, 1]
  double r2 = cIn.back() * r1;  // cIn.back() is the final value of the cdf (can be norm'd to 1, but not required)
  
  // return the first position not less than the specified value (r2)
  vector<double>::iterator low = lower_bound(cIn.begin(), cIn.end(), r2); 
  
  // difference of two iterator positions is an integer.  cIn.begin() is == 0.
  double n = low - cIn.begin();
  
  fParticleGun->SetParticleEnergy(cEn.at(n) * eV);


  // === "T pipe" position generator ===
  if (gas_source) 
  {  
    G4double z0 = -3.4873 * cm - 0.00064 * cm; // near//-5.15112*cm-0.00064*cm;//far//-1.66*cm;//Bi//

    // G4double u0 = sqrt((1.0 / 2) * (1.0 / 2) * G4UniformRand()) * cm;
    G4double v0 = 2 * 3.141592653 * G4UniformRand();
    // G4double x0 = 1.60274 / 2 * cm * (2 * G4UniformRand() - 1) * cos(v0); // 2.68205602104/2*cm + u * cos (v);
    // G4double y0 = 1.60274 / 2 * cm * (2 * G4UniformRand() - 1) * sin(v0); // 2.68205602104/2*cm + u * sin (v);

    // calibration source generator - Source Radius 0.9398
    G4double rad_new = 0.9398; // 3.4798/2*cm beam rad; 
    G4double u0 = sqrt((rad_new / 2) * (rad_new) / 2 * G4UniformRand()) * cm;
    G4double v = 2 * 3.141592653 * G4UniformRand();
    G4double x1 = 0 / 2 * cm + u0 * cos(v); //-2.68205602104
    G4double y1 = 0 / 2 * cm + u0 * sin(v);

    // G4double z1 = 1.27 * cm + G4UniformRand() * 16.51 * cm; // 11.13765*cm-2.31115*cm+(G4UniformRand())*16*cm/2;
    G4double vx1 = 2 * G4UniformRand() - 1;
    G4double vy1 = 2 * G4UniformRand() - 1;
    G4double vz2 = -2 * G4UniformRand() + 1;
   
    G4double randall = 2 * G4UniformRand() - 1;
    G4double H1 = 6.75 * 2.54;     // make sure this agrees with Tdv_H1 in BM_Detector.cc
    G4double h2 = 6.75 * 2.54 / 2; // make sure this agrees with Tdv_h2 in BM_Detector.cc
    
    G4double rat = (h2 + rad_new / 2) / (H1 + h2 + rad_new);

    u0 = sqrt ((rad_new/2) * (rad_new)/2 * G4UniformRand())*cm;
    v0 = 2 * 3.141592653* G4UniformRand();
    G4double z2 = 0/2*cm + u0 * cos (v0);//-2.68205602104
    G4double y2 = 0/2*cm + u0 * sin (v0);
    G4double x2 = -1* G4UniformRand()*h2*cm;
    G4double r2 = sqrt(y2*y2 + x2*x2);
    
    G4bool okay = false;
    if (randall < rat){
      while (okay != true){
        G4double u = sqrt ((rad_new/2) * (rad_new)/2 * G4UniformRand())*cm;
        G4double v = 2 * 3.141592653 * G4UniformRand();
        G4double z2 = (1.27*4+6.75*2.54)/2*cm + u * cos (v);//-2.68205602104 3.4798/2*cm - 6.75*2.54/2*cm//(-1.27+16.51)/2*cm
        G4double y2 = 0/2*cm + u * sin (v);
        G4double x2 = -1* G4UniformRand()*h2*cm;
        G4double r2 = sqrt(y2*y2 + x2*x2);
        if (r2 > rad_new){
          okay = true;
          fParticleGun->SetParticlePosition(G4ThreeVector(x2,y2,z2));// T pipe
        }
      }
    }
    else {
      G4double u0 = sqrt ((rad_new/2) * (rad_new)/2 * G4UniformRand())*cm;
      G4double v0 = 2 * 3.141592653* G4UniformRand();
      G4double x2 = 0/2*cm + u0 * cos (v0);//-2.68205602104
      G4double y2 = 0/2*cm + u0 * sin (v0);
      G4double z2 = 2*1.27*cm + G4UniformRand()*(6.75*2.54)*cm;
      fParticleGun->SetParticlePosition(G4ThreeVector(x2,y2,z2));// T pipe
    }    
  }

  // === calibration source position generator ===
  else if (cal_source)
  {
    G4cout << "Cal source\n";

    // calibration source generator - Source Radius 0.9398
    G4double rad_new = 0.9398; // 3.4798/2*cm beam rad; 
    G4double u0 = sqrt((rad_new / 2) * (rad_new) / 2 * G4UniformRand()) * cm;
    G4double v = 2 * 3.141592653 * G4UniformRand();
    G4double x1 = 0 / 2 * cm + u0 * cos(v); //-2.68205602104
    G4double y1 = 0 / 2 * cm + u0 * sin(v);
    G4double z0 = -3.4873 * cm + G4UniformRand() * 0.31749 / 2 * cm;


    // G4double z1 = 1.27 * cm + G4UniformRand() * 16.51 * cm; // 11.13765*cm-2.31115*cm+(G4UniformRand())*16*cm/2;
    G4double vx1 = 2 * G4UniformRand() - 1;
    G4double vy1 = 2 * G4UniformRand() - 1;
    G4double vz2 = -2 * G4UniformRand() + 1;

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx1, vy1, vz2));
    fParticleGun->SetParticlePosition(G4ThreeVector(-x1, -y1, z0));
  }
  else 
  {
    G4cout << "No source set!\n";
    exit(0);
  }

  fParticleGun->GeneratePrimaryVertex(anEvent); 
}

