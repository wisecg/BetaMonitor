// BM_Detector.hh - header for BM_Detector class: BetaMon detector and geometry
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

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
  G4LogicalVolume *flogicDetector1;
  G4LogicalVolume *flogicDetector5;
  G4LogicalVolume *flogicDetector6;
  G4LogicalVolume *flogicDetector3;
  G4LogicalVolume *logicWorld;
  G4LogicalVolume *logicSiPM1;
  G4LogicalVolume *logicSiPM2;
  G4LogicalVolume *logicSiPM3;
  G4LogicalVolume *logicSiPM4;
  G4LogicalVolume *logicPCB1;
  G4LogicalVolume *logicPCB2;
  G4LogicalVolume *logicPCB3;
  G4LogicalVolume *logicPCB4;
  G4LogicalVolume *fLogicMagnetic;
  G4LogicalVolume *logicEnv;

  static G4UniformMagField *fMagneticField;
  static G4FieldManager *fFieldMgr;

protected:
  G4LogicalVolume *fScoringVolume;
};

#endif
