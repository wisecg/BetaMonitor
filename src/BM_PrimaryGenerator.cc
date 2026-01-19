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
  // std::ifstream inputFile ("../dat/90SrDecay_cdf.txt");
  // std::ifstream inputFile("../dat/6HeDecay_cdf.txt");
  std::ifstream inputFile("../dat/19NeDecay_cdf.txt");
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

  bool cal_source = false;
  bool gas_source = true; 

  // === set particle type, energy, and intensity === 

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  // He/Sr is e-
  // G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
  
  // Ne is e+
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e+");
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


  // declaring variables used in position generation
  G4double x, y, z, vx, vy, vz, r_rand, theta;

  // === "T pipe" position generator ===
  if (gas_source) 
  {  
    // T pipe dimensions, variables names from BM_Detector.cc
    G4double cyl_hdv = 6.75 * 2.54 * cm;     // height of decay volume 16.51 standard
    G4double Tdv_r1i = 3.4798 / 2 * cm; //t-pipe decay vol inner radius
    G4double Tdv_h1 = 6.75 * 2.54 * cm; // t-pipe major axis length
    G4double Tdv_h2 = 6.75 * 2.54 / 2 * cm; // t-pipe minor axis length
    
    // rectangular bounding area for positions generation
    // Coordinate system origin is center of T pipe, will shift later
    G4double xmin = -Tdv_h2; // should maybe be Tdv_r1i - Tdv_h2
    G4double xmax = Tdv_r1i;
    G4double ymin = -Tdv_r1i; 
    G4double ymax = Tdv_r1i;
    G4double zmin = -Tdv_h1 / 2;
    G4double zmax = Tdv_h1 / 2;

    bool in_tpipe = false;
    G4double x_t, y_t, z_t;
    while (!in_tpipe)
    {
      x_t = xmin + (xmax - xmin) * G4UniformRand();
      y_t = ymin + (ymax - ymin) * G4UniformRand();
      z_t = zmin + (zmax - zmin) * G4UniformRand();
      
      bool in_main = (
        (x_t * x_t + y_t * y_t) <= (Tdv_r1i * Tdv_r1i) && 
        abs(z_t) <= (Tdv_h1 / 2)
      ); // in main cylinder

      bool in_branch = (
        (y_t * y_t + z_t * z_t) <= (Tdv_r1i * Tdv_r1i) && 
        (x_t >= -Tdv_h2) && (x_t <= 0)
      ); // in branch cylinder
      in_tpipe = in_main || in_branch;
    }

    // shift coordinates to be relative to world origin
    x = x_t;
    y = y_t;
    z = z_t + (cyl_hdv / 2 + 2 * 1.27 * cm); // shift z to be inside T pipe in world coords
    vx = 2 * G4UniformRand() - 1;
    vy = 2 * G4UniformRand() - 1;
    vz = 2 * G4UniformRand() - 1;
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx, vy, vz));
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z)); 
  }

  // === calibration source position generator ===
  else if (cal_source)
  {
    // G4cout << "Cal source\n";

    // calibration source generator - Source Radius 0.9398 / 2
    G4double r_source = 0.9398 / 2; // radius of the source 3.4798/2*cm beam rad; 
    G4double mylar_thickness = 0.00032; // thickness of source backing material in cm
    
    r_rand = r_source * sqrt(G4UniformRand()) * cm; // r_rand to sample uniformly in circle
    theta = 2 * 3.141592653 * G4UniformRand(); 
    x = r_rand * cos(theta); 
    y = r_rand * sin(theta);
    z = -3.4873 * cm - mylar_thickness / 2; // placing particles behind the mylar window

    // G4double z1 = 1.27 * cm + G4UniformRand() * 16.51 * cm; // 11.13765*cm-2.31115*cm+(G4UniformRand())*16*cm/2;
    vx = 2 * G4UniformRand() - 1;
    vy = 2 * G4UniformRand() - 1;
    vz = 2 * G4UniformRand() - 1; //only generate particles in hemisphere toward detector

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx, vy, vz));
    fParticleGun->SetParticlePosition(G4ThreeVector(-x, -y, z));
  }
  else 
  {
    G4cout << "No source set!\n";
    exit(0);
  }

  fParticleGun->GeneratePrimaryVertex(anEvent); 
}

