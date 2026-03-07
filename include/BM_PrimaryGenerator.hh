#ifndef BM_PRIMARYGENERATOR_HH
#define BM_PRIMARYGENERATOR_HH
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class BM_PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
  public:
    BM_PrimaryGenerator();
    virtual ~BM_PrimaryGenerator();
    virtual void GeneratePrimaries(G4Event*);
  private:
    G4GeneralParticleSource* fParticleSource;
};

#endif
// #include <fstream>
// #include <vector>
// #include "globals.hh"
// #include "G4VUserPrimaryGeneratorAction.hh"
// #include "G4ParticleGun.hh"
// #include "G4GeneralParticleSource.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4UImessenger.hh"
// #include "G4UIcmdWithAString.hh"
// #include "G4UIcmdWithABool.hh"

// class G4ParticleGun;
// class G4Event;
// class G4ParticleDefinition;
// class G4SingleParticleSource;
// class G4PrimaryVertex;
// class G4Box;

// class BM_PrimaryGenerator : public G4VUserPrimaryGeneratorAction
// {
// public:
//   BM_PrimaryGenerator();
//   ~BM_PrimaryGenerator();

// public:
//   static BM_PrimaryGenerator *Instance();
//   void GeneratePrimaries(G4Event *anEvent);
//   void LoadSpectrum();

// private:
//   static BM_PrimaryGenerator *instance_;

// public:
//   G4GeneralParticleSource *fParticleGun;
//   std::vector<double> cEn;
//   std::vector<double> cIn;
//   G4String inputFileName;
//   G4String sourceType; // "gas" or "cal"
//   G4String particleType; // "e-" or "e+"
//   // Messenger pointer
//   class BM_PrimaryGeneratorMessenger* messenger;
// };

// class BM_PrimaryGeneratorMessenger : public G4UImessenger {
//   BM_PrimaryGenerator* generator;
//   G4UIcmdWithAString* inputFileCmd;
//   G4UIcmdWithAString* sourceTypeCmd;
//   G4UIcmdWithAString* particleTypeCmd;
// public:
//   BM_PrimaryGeneratorMessenger(BM_PrimaryGenerator* gen);
//   ~BM_PrimaryGeneratorMessenger();
//   void SetNewValue(G4UIcommand* cmd, G4String value) override;
// };

// #endif
