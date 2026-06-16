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



