#ifndef BM_PRIMARYGENERATOR_HH
#define BM_PRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
// #include "FileReader.hh"
#include <fstream>

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
  G4ParticleDefinition *positron;
  G4ParticleGun *fParticleGun;
  G4ParticleGun *heParticleGun;
  G4ParticleDefinition *recoilparticle;
  G4ParticleDefinition *gammaradiation;
  G4PrimaryVertex *vertexpositron;
  G4PrimaryVertex *vertexrecoil;
  G4Box *fEnvelopeBox;
  G4ThreeVector EventVertex;
  G4ThreeVector sourcePosition;
  G4double i;
  G4double theta;
};

#endif // BM_PRIMARYGENERATOR_HH
