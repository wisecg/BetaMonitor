#ifndef BM_PRIMARYGENERATOR_HH
#define BM_PRIMARYGENERATOR_HH
#include <fstream>
#include <vector>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleGun;
class G4Event;
class G4ParticleDefinition;
class G4SingleParticleSource;
class G4PrimaryVertex;
class G4Box;

class BM_PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  BM_PrimaryGenerator();
  ~BM_PrimaryGenerator();

public:
  static BM_PrimaryGenerator *Instance();
  void GeneratePrimaries(G4Event *anEvent);

private:
  static BM_PrimaryGenerator *instance_;

public:
  G4ParticleGun *fParticleGun;
  std::vector<double> cEn;
  std::vector<double> cIn;
};

#endif 
