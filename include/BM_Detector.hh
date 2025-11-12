// BM_Detector.hh - header for BM_Detector class: BetaMon detector and geometry

#ifndef BM_DETECTOR_H
#define BM_DETECTOR_H

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include "BM_TrackInfo.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4UniformMagField.hh"
#include "globals.hh"
#include "G4SDManager.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class MagField;

/// Detector construction class to define materials and geometry.

class BM_Detector : public G4VUserDetectorConstruction
{
public:
  BM_Detector();
  virtual ~BM_Detector();

  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

  G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }
  G4SDManager *SDMan;
  
  G4LogicalVolume *flogicDetector6;
  G4LogicalVolume *fLogicMagnetic;
  G4LogicalVolume *flogicDetector1;
  G4LogicalVolume *flogicDetector5;
  G4LogicalVolume *flogicDetector3;
  G4LogicalVolume *logicEnv;
  G4LogicalVolume *logicWorld;

  static G4UniformMagField *fMagneticField;
  static G4FieldManager *fFieldMgr;

protected:
  G4LogicalVolume *fScoringVolume;
};

#endif
