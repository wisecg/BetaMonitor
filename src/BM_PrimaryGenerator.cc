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
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"

#include "BM_PrimaryGenerator.hh"
#include "BM_EventAction.hh"
#include "BM_Output.hh"


using namespace std;

BM_PrimaryGenerator::BM_PrimaryGenerator() : G4VUserPrimaryGeneratorAction(), fParticleSource(nullptr)
{
  fParticleSource = new G4GeneralParticleSource();
}

BM_PrimaryGenerator::~BM_PrimaryGenerator()
{
  delete fParticleSource;
}

void BM_PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
  fParticleSource->GeneratePrimaryVertex(anEvent);
}

// BM_PrimaryGenerator::BM_PrimaryGenerator() public G4VUserPrimaryGeneratorAction
// {
//   public:
//     void GeneratePrimaries(G4Event* event) { fParticleGun.GeneratePrimaryVertex(event); } 
//   private:
//     G4GeneralParticleSource fParticleGun;
// };

// BM_PrimaryGenerator::BM_PrimaryGenerator() : G4VUserPrimaryGeneratorAction(),
//                                              fParticleGun(0)
// {
//   G4int n_particle = 1;

//   // He/Ne (monoenergetic) particle generation - also used for Sr90 betas
//   // fParticleGun = new G4ParticleGun(n_particle);
//   fParticleGun = new G4GeneralParticleSource(n_particle);
//   messenger = new BM_PrimaryGeneratorMessenger(this);
// }

// BM_PrimaryGenerator::~BM_PrimaryGenerator()
// {
//   delete fParticleGun;
//   delete messenger;
// }

// void BM_PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
// {
//   /*
//   For each primary, we need to call SetParticleTime, SetParticlePosition, 
//   SetParticleMomentumDirection, and GeneratePrimaryVertex.
//   */
//   fParticleGun->SetParticleTime(10 * G4UniformRand());

//   bool cal_source = (sourceType == "cal");
//   bool gas_source = (sourceType == "gas"); 

//   // === set particle type, energy, and intensity === 

//   G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
//   G4String particleName;

//   G4ParticleDefinition *particle = particleTable->FindParticle(particleType);
//   fParticleGun->SetParticleDefinition(particle);

//   // sample from the CDF of the beta decay spectrum (input file)
//   double r2 = cIn.back() * G4UniformRand(); // cIn.back() is the final value of the cdf (can be norm'd to 1, but not required)
  
//   // return the first position not less than the specified value (r2)
//   vector<double>::iterator low = lower_bound(cIn.begin(), cIn.end(), r2); 
  
//   // difference of two iterator positions is an integer.  cIn.begin() is == 0.
//   double n = low - cIn.begin();
  
//   fParticleGun->SetParticleEnergy(cEn.at(n) * eV);

//   // declaring variables used in position generation
//   G4double x, y, z, vx, vy, vz, r_rand, theta;

//   // === "T pipe" position generator ===
//   if (gas_source) 
//   {  
//     // T pipe dimensions, variables names from BM_Detector.cc
//     G4double cyl_hdv = 6.75 * 2.54 * cm;     // height of decay volume 16.51 standard
//     G4double Tdv_r1i = 3.4798 / 2 * cm; //t-pipe decay vol inner radius
//     G4double Tdv_h1 = 6.75 * 2.54 * cm; // t-pipe major axis length
//     G4double Tdv_h2 = 6.75 * 2.54 / 2 * cm; // t-pipe minor axis length
//     G4double flange_width = 1.27 * cm; // Flange width
    
//     // rectangular bounding area for positions generation
//     // Coordinate system origin is center of T pipe, will shift later
//     G4double xmin = -Tdv_h2 - flange_width; // should maybe be Tdv_r1i - Tdv_h2
//     G4double xmax = Tdv_r1i;
//     G4double ymin = -Tdv_r1i; 
//     G4double ymax = Tdv_r1i;
//     G4double zmin = -Tdv_h1 / 2 - flange_width;
//     G4double zmax = Tdv_h1 / 2 + flange_width;

//     bool in_tpipe = false;
//     G4double x_t, y_t, z_t;
//     while (!in_tpipe)
//     {
//       x_t = xmin + (xmax - xmin) * G4UniformRand();
//       y_t = ymin + (ymax - ymin) * G4UniformRand();
//       z_t = zmin + (zmax - zmin) * G4UniformRand();
      
//       bool in_main = (
//         (x_t * x_t + y_t * y_t) <= (Tdv_r1i * Tdv_r1i) && 
//         abs(z_t) <= (Tdv_h1 / 2)
//       ); // in main cylinder

//       bool in_branch = (
//         (y_t * y_t + z_t * z_t) <= (Tdv_r1i * Tdv_r1i) && 
//         (x_t >= -Tdv_h2) && (x_t <= 0)
//       ); // in branch cylinder
//       in_tpipe = in_main || in_branch;
//     }

//     // shift coordinates to be relative to world origin
//     x = x_t;
//     y = y_t;
//     z = z_t + (cyl_hdv / 2 + 2 * 1.27 * cm); // shift z to be inside T pipe in world coords
//     vx = 2 * G4UniformRand() - 1;
//     vy = 2 * G4UniformRand() - 1;
//     vz = 2 * G4UniformRand() - 1;
//     fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx, vy, vz));
//     fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z)); 
//   }

//   // === calibration source position generator ===
//   else if (cal_source)
//   {
//     // G4cout << "Cal source\n";

//     // calibration source generator - Source Radius 0.9398 / 2
//     G4double r_source = 0.9398 / 2; // radius of the source 3.4798/2*cm beam rad; 
//     G4double mylar_thickness = 0.00032; // thickness of source backing material in cm
//     G4double possource = -3.4873 * cm + 7.6477 * mm;
//     r_rand = r_source * sqrt(G4UniformRand()) * cm; // r_rand to sample uniformly in circle
//     theta = 2 * 3.141592653 * G4UniformRand(); 
//     x = r_rand * cos(theta); 
//     y = r_rand * sin(theta);
//     z = possource - mylar_thickness / 2; // placing particles behind the mylar window

//     // G4double z1 = 1.27 * cm + G4UniformRand() * 16.51 * cm; // 11.13765*cm-2.31115*cm+(G4UniformRand())*16*cm/2;
//     vx = 2 * G4UniformRand() - 1;
//     vy = 2 * G4UniformRand() - 1;
//     vz = 2 * G4UniformRand() - 1; //only generate particles in hemisphere toward detector

//     fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx, vy, vz));
//     fParticleGun->SetParticlePosition(G4ThreeVector(-x, -y, z));
//   }
//   else 
//   {
//     G4cout << "No source set!\n";
//     exit(0);
//   }

//   // Fill the primary input tree with primary info
//   // Get BM_Output instance
//   BM_Output* output = BM_Output::Instance();
//   // Use the last set values for x, y, z, vx, vy, vz, and energy
//   int primary_pid = particle->GetPDGEncoding();
//   int primary_eventid = anEvent->GetEventID();
//   output->setPrimaryInputParams(
//     primary_pid,
//     primary_eventid,
//     x, y, z,
//     vx, vy, vz,
//     fParticleGun->GetParticleEnergy()
//   );
//   output->FillPrimaryInput();

//   fParticleGun->GeneratePrimaryVertex(anEvent); 
// }

// void BM_PrimaryGenerator::LoadSpectrum() {
//   cEn.clear();
//   cIn.clear();
//   if (inputFileName.empty()) {
//     std::cerr << "ERROR: inputFileName is empty!" << std::endl;
//     exit(1);
//   }
//   std::ifstream inputFile(inputFileName);
//   if (!inputFile.is_open()) {
//     std::cerr << "ERROR: Could not open input file: " << inputFileName << std::endl;
//     exit(1);
//   }
//   double a, b;
//   while (inputFile >> a >> b) {
//     cEn.push_back(a);
//     cIn.push_back(b);
//   }
//   inputFile.close();
// }

// BM_PrimaryGeneratorMessenger::BM_PrimaryGeneratorMessenger(BM_PrimaryGenerator* gen)
// : generator(gen) {
//   inputFileCmd = new G4UIcmdWithAString("/bm/inputFile", this);

//   sourceTypeCmd = new G4UIcmdWithAString("/bm/sourceType", this);
//   particleTypeCmd = new G4UIcmdWithAString("/bm/particleType", this);
// }

// BM_PrimaryGeneratorMessenger::~BM_PrimaryGeneratorMessenger() {
//   delete inputFileCmd;
//   delete sourceTypeCmd;
//   delete particleTypeCmd;
// }

// void BM_PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
//   if(cmd == inputFileCmd) {
//     generator->inputFileName = value;
//     generator->LoadSpectrum();
//   }
//   if(cmd == sourceTypeCmd) generator->sourceType = value;
//   if(cmd == particleTypeCmd) generator->particleType = value;
// }

