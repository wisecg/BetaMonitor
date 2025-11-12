// BM_Detector.cc - Implementation of BM_Detector class: BetaMon detector and geometry

// Field handling
#include "G4Mag_UsualEqRhs.hh"
#include "G4EqMagElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"

// Integrators
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4PropagatorInField.hh"

// Geometry
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4PVParameterised.hh"
#include "G4TransportationManager.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"

#include "G4ThreeVector.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "BM_Detector.hh"
#include "BM_SD.hh"
#include "BM_SteppingAction.hh"
#include "MagneticField.hh"

/*
  NOTE: Nov 2025 - many block comments & other old geometries were removed to
  make the code more readable.  IF you need info on an old geometry: refer to ../test/BM_Detector.cc.

  running now:
  "SiPMBlake": SmallScin, Al, cone flange

  older geometries:
  OrigMon : top round, bottom round, square, Cu, normal flange
  SiPMWinston :  LargeScin, Cu, normal flange
  TeleOld ("telescope") : thinTele, Cu, normal flange
  TeleTAMU : TAMUdE, ?, normal flange?
  Beamline : adds the flange and decay volume geometry
  TabSource : adds table source geometry

  TODO: implement options to turn on/off certain volumes.
  Need a separate function for detector, bore, and source

  TODO: rework such that port is at 0,0,0?
*/

// MagField* BM_Detector::fMagneticField = 0;
G4FieldManager *BM_Detector::fFieldMgr = 0;
G4UniformMagField *BM_Detector::fMagneticField = 0;

BM_Detector::BM_Detector() : G4VUserDetectorConstruction(),
                             flogicDetector6(nullptr), fLogicMagnetic(nullptr), flogicDetector1(nullptr),
                             flogicDetector5(nullptr), flogicDetector3(nullptr), logicEnv(nullptr), logicWorld(nullptr) {}

BM_Detector::~BM_Detector() {}

G4VPhysicalVolume *BM_Detector::Construct()
{
  G4cout << "Running sim with Blake's SiPM geometry: SmallScin, Al, cone flange" << G4endl;

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // NIST material manager
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *Fe = nist->FindOrBuildMaterial("G4_Fe");
  G4Material *PVT = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  // G4Material *Tape = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
  G4Material *Mylar = nist->FindOrBuildMaterial("G4_MYLAR");
  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
  G4Material *Cr = nist->FindOrBuildMaterial("G4_Cr");
  G4Material *Ni = nist->FindOrBuildMaterial("G4_Ni");
  G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");
  G4Material *Mn = nist->FindOrBuildMaterial("G4_Mn");
  // G4Material *Si = nist->FindOrBuildMaterial("G4_Si");
  // G4Material *Kap = nist->FindOrBuildMaterial("G4_KAPTON");
  G4double density = 8030. * mg / cm3;
  G4Material *Stainless_Steel = new G4Material("Stainless_Steel", density, 4);

  // auto pcb = new G4Element("Vetronite", 2. * g / cm3, 2); // Fiber glass
  auto el_PCB_Si = new G4Element("Silicon", "Si", 14., 28.0855 * g / mole);
  auto el_PCB_O = new G4Element("Oxygen", "O", 8., 15.9994 * g / mole);
  auto mat_PCB = new G4Material("Vetronite", 2. * g / cm3, 2); // Fiber glass
  mat_PCB->AddElement(el_PCB_Si, 1);
  mat_PCB->AddElement(el_PCB_O, 2);
  Stainless_Steel->AddMaterial(Fe, 72 * perCent);
  Stainless_Steel->AddMaterial(Cr, 18 * perCent);
  Stainless_Steel->AddMaterial(Ni, 8 * perCent);
  Stainless_Steel->AddMaterial(Mn, 2 * perCent);

  // Vacuum
  G4double atomicNumber = 1.;
  G4double massOfMole = 1.008 * g / mole;
  G4double density2 = 1.e-25 * g / cm3;
  G4double temperature = 2.73 * kelvin;
  G4double pressure = 1.3332e-8 * pascal;
  G4Material *Vacuum = new G4Material("interGalactic", atomicNumber, massOfMole, density2, kStateGas, temperature, pressure);

  // World
  G4double world_sizeXY = 18 * cm;
  G4double world_sizeZ = 40.75 * cm;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box *solidWorld = new G4Box("World", 0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);
  logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");

  // set maximum step - SetUserLimits(Limits) should be called for every G4LogicalVolume we create
  G4UserLimits *Limits = new G4UserLimits(10 * um); 
  logicWorld->SetUserLimits(Limits);

  // for reference - give the parameter definitions for G4PVPlacement once
  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // origin (default at (0,0,0))
                        logicWorld,      // its logical volume
                        "World",         // its name
                        0,               // its mother volume
                        false,           // no boolean operation
                        0,               // copy number
                        checkOverlaps);  // overlaps checking

  // Decay Volume Geometry
  // G4double cyl_r1dvo = 3.81 / 2 * cm;   // Outer Radius of Decay Volume
  G4double cyl_r1dvi = 3.4798 / 2 * cm; // Inner Radius of Decay Volume
  G4double cyl_hdv = 6.75 * 2.54 * cm;  // 12.5476*cm;//length of decay volume 16.51 standard
  G4double cyl_r2dvo = 6.9088 / 2 * cm; // Flange Outer Radius

  // T Decay Volume Geometry
  G4double Tdv_r1o = 3.81 / 2 * cm;       // outer radius of DV
  G4double Tdv_r1i = 3.4798 / 2 * cm;     // inner radius of DV
  G4double Tdv_H1 = 6.75 * 2.54 * cm;     // Major axis length of DV
  G4double Tdv_h2 = 6.75 * 2.54 / 2 * cm; // Minor axis length of DV

  // Decay Volume T
  G4Tubs *solidShapeT1o = new G4Tubs("Pipe1o", 0, Tdv_r1o, Tdv_H1 / 2., 0, 360 * deg);
  G4Tubs *solidShapeT1i = new G4Tubs("Pipe1i", 0, Tdv_r1i, Tdv_H1 / 1.9, 0, 360 * deg);
  G4Tubs *solidShapeT2o = new G4Tubs("Pipe2o", 0, Tdv_r1o, Tdv_h2 / 2, 0, 360 * deg);
  G4Tubs *solidShapeT2i = new G4Tubs("Pipe2i", 0, Tdv_r1i, Tdv_h2 / 2, 0, 360 * deg);
  
  // Decay Volume Flange
  G4Tubs *solidFlange = new G4Tubs("Flange", 0 * cm, cyl_r2dvo, 1.2700 / 2 * cm, 0, 360 * deg);
  G4Tubs *boreFlange = new G4Tubs("boreFlange", 0 * cm, cyl_r1dvi, 1.27 * cm, 0, 360 * deg);

  // Vacuum Seal
  G4double cyl_r2c = 3.556 / 2 * cm; // copper seal radius
  G4double cyl_hc = 0.0254 * cm;     // thickness of copper
  // G4double cyl_hkap = 0.0012 * cm;   // thickness of kapton (trials,1:0.0012, 2: 0.00075, 3: 0.006, 4: 0.0127)

  // 
  G4ThreeVector TransT(Tdv_r1i / 2 - Tdv_h2 / 2, 0, 0);
  G4RotationMatrix *yRotT = new G4RotationMatrix;
  yRotT->rotateY(3.14159265 / 2 * rad); // Rotates 90 degrees
  G4UnionSolid *Pipei = new G4UnionSolid("InnerTPipe", solidShapeT1i, solidShapeT2i, yRotT, TransT);
  logicEnv = new G4LogicalVolume(Pipei, Vacuum, "Envelope"); // (its solid, its material, its name)
  new G4PVPlacement(0, G4ThreeVector(0 * cm, 0 * cm, cyl_hdv / 2 + 2 * 1.27 * cm),
                    logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);
  logicEnv->SetUserLimits(Limits);

  flogicDetector6 = new G4LogicalVolume(Pipei, Vacuum, "Envelopedet");
  new G4PVPlacement(0, G4ThreeVector(0 * cm, 0 * cm, cyl_hdv / 2 + 2 * 1.27 * cm),
                    flogicDetector6, "Envelopedet", logicWorld, false, 0, checkOverlaps);
  flogicDetector6->SetUserLimits(Limits);

  G4SubtractionSolid *Flangen = new G4SubtractionSolid("Flange", solidFlange, boreFlange);
  G4LogicalVolume *logicDecayVolume3 = new G4LogicalVolume(Flangen, Stainless_Steel, "Decay_Volume_r");
  G4ThreeVector posdv2 = G4ThreeVector(0 * cm, 0 * cm, 1.27 / 2 * cm);
  new G4PVPlacement(0, posdv2, logicDecayVolume3, "Decay_Volume_r", logicWorld,
                    false, 0, checkOverlaps);
  logicDecayVolume3->SetUserLimits(Limits);

  G4LogicalVolume *logicDecayVolume6 = new G4LogicalVolume(Flangen, Stainless_Steel, "Decay_Volume_r");
  new G4PVPlacement(0, G4ThreeVector(0 * cm, 0 * cm, 3 * 1.27 / 2 * cm),
                    logicDecayVolume6, "Decay_Volume_r", logicWorld, false, 0, checkOverlaps);
  logicDecayVolume6->SetUserLimits(Limits);

  G4LogicalVolume *logicDecayVolume4 = new G4LogicalVolume(Flangen, Stainless_Steel, "Decay_Volume_r");
  new G4PVPlacement(0, G4ThreeVector(0, 0, cyl_hdv + 1.27 * 2 * cm), logicDecayVolume4, "Decay_Volume_r",
                    logicWorld, false, 0, checkOverlaps);
  logicDecayVolume4->SetUserLimits(Limits);

  G4LogicalVolume *logicDecayVolume5 = new G4LogicalVolume(Flangen, Stainless_Steel, "Decay_Volume_r");
  new G4PVPlacement(yRotT, G4ThreeVector(-Tdv_h2 + 1.27 / 2 * cm, 0, cyl_hdv / 2 + 1.27 * 2 * cm),
                    logicDecayVolume5, "Decay_Volume_r", logicWorld, false, 0, false);
  logicDecayVolume5->SetUserLimits(Limits);

  G4Tubs *solidShape4 = new G4Tubs("Shape4", 0. * cm, cyl_r2c, cyl_hc / 2., 0, 360 * deg);
  flogicDetector1 = new G4LogicalVolume(solidShape4, Al, "Copper");
  G4ThreeVector poscu = G4ThreeVector(0 * cm, 0 * cm, 1.27 * cm + cyl_hc / 2);
  new G4PVPlacement(0, poscu, flogicDetector1, "Copper", logicWorld, false, 0, checkOverlaps);
  flogicDetector1->SetUserLimits(Limits);

  // T Pipe
  G4ThreeVector TransT2(Tdv_r1i / 2 - Tdv_h2, 0, 0);
  G4UnionSolid *Pipeo = new G4UnionSolid("OuterTPipe", solidShapeT1o, solidShapeT2o, yRotT, TransT);
  G4SubtractionSolid *TPipe1 = new G4SubtractionSolid("TDecayVolume", Pipeo, solidShapeT1i);
  G4SubtractionSolid *TPipe = new G4SubtractionSolid("TDecayVolume", TPipe1, solidShapeT2i, yRotT, TransT2);

  G4LogicalVolume *logicDecayVolume1 = new G4LogicalVolume(TPipe, Stainless_Steel, "Decay_Volume");
  new G4PVPlacement(0, G4ThreeVector(0, 0, cyl_hdv / 2 + 1.27 * 2 * cm),
                    logicDecayVolume1, "Decay_Volume", logicWorld, false, 0, checkOverlaps);
  logicDecayVolume1->SetUserLimits(Limits);

  // Blake's SiPM (2024)
  G4Box *SmallScin = new G4Box("SmallScin", 20.0 / 2 * mm, 20.0 / 2 * mm, 3.0 / 2 * mm);

  G4Box *SmallScinMA = new G4Box("SmallScinMA", // aluminized mylar
                                 20 / 2 * mm + 0.0001 * mm, 20 / 2 * mm + 0.0001 * mm,
                                 3.0 / 2 * mm + 0.0001 * mm);

  G4Box *SmallScinM = new G4Box("SmallScinM",
                                20.0 / 2 * mm + 0.0761 * mm, 20.0 / 2 * mm + 0.0761 * mm,
                                3.0 / 2 * mm + 0.0761 * mm);

  G4Box *SmallScinMA2 = new G4Box("SmallScinMAOuter",
                                  20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm,
                                  20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm,
                                  3.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm);

  G4Box *SmallScinMA2nd = new G4Box("SmallScinMA",
                                    20 / 2 * mm + 0.0001 * mm, 20 / 2 * mm + 0.0001 * mm,
                                    3.0 / 2 * mm + 0.0001 / 2 * mm); // its size

  G4Box *SmallScinM2nd = new G4Box("SmallScinM",
                                   20.0 / 2 * mm + 0.0761 * mm,
                                   20.0 / 2 * mm + 0.0761 * mm,
                                   3.0 / 2 * mm + 0.0761 / 2 * mm);

  G4Box *SmallScinMA22nd = new G4Box("SmallScinMAOuter",
                                     20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm,
                                     20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm,
                                     3.0 / 2 * mm + 0.0762 / 2 * mm);

  G4SubtractionSolid *solidAlMyO = new G4SubtractionSolid("OuterAl", SmallScinMA2, SmallScinM);

  G4SubtractionSolid *solidAlMy = new G4SubtractionSolid("Mylar", SmallScinM, SmallScinMA);

  G4SubtractionSolid *solidAlMyI = new G4SubtractionSolid("InnerAl", SmallScinMA, SmallScin);

  G4RotationMatrix *Rot0 = new G4RotationMatrix;
  Rot0->rotateX(0 * rad);

  G4SubtractionSolid *solidAlMyO2 = new G4SubtractionSolid("OuterAl2",
                                                           SmallScinMA22nd, SmallScinM2nd, Rot0,
                                                           G4ThreeVector(0, 0, -0.0001 / 2 * mm));

  G4SubtractionSolid *solidAlMy2 = new G4SubtractionSolid("Mylar2",
                                                          SmallScinM2nd, SmallScinMA2nd, Rot0,
                                                          G4ThreeVector(0, 0, -0.0761 / 2 * mm));

  G4SubtractionSolid *solidAlMyI2 = new G4SubtractionSolid("InnerAl2",
                                                           SmallScinMA2nd, SmallScin, Rot0,
                                                           G4ThreeVector(0, 0, -0.0762 / 2 * mm));

  G4Box *scintHolderblock = new G4Box("scintHolderbase",
                                      22.2 / 2 * mm, 22.2 / 2 * mm,
                                      27.4 / 2 * mm);

  G4Box *scintHoldercutout1 = new G4Box("scintHoldercutout1",
                                        22.2 / 2 * mm - 0.85 * mm,
                                        22.2 / 2 * mm - 0.85 * mm,
                                        26.4 / 2 * mm);
  G4Box *scintHoldercutout2 = new G4Box("scintHoldercutout2",
                                        18 / 2 * mm, 18 / 2 * mm,
                                        30 / 2 * mm);

  G4SubtractionSolid *scintHolder1 = new G4SubtractionSolid("scintHolder1", scintHolderblock, scintHoldercutout2);

  G4SubtractionSolid *scintHolder = new G4SubtractionSolid("scintHolder", scintHolder1, scintHoldercutout1);

  G4Box *scintSleevebase = new G4Box("scintSleevebase",  20.5 / 2 * mm,  20.5 / 2 * mm, 10 / 2 * mm);
  
  G4Box *scintSleevecutout = new G4Box("scintHoldercutout2",  16 / 2 * mm, 16 / 2 * mm, 30 / 2 * mm);
  
  G4SubtractionSolid *scintSleeve = new G4SubtractionSolid("scintSleeve", scintSleevebase, scintSleevecutout);

  G4Tubs *scintHolderBase = new G4Tubs("scintHolderbase", 20 / 2 * mm, 45.72 / 2 * mm, 1.575 / 2 * mm, 0, 360 * deg);

  G4Tubs *sipmLidBase = new G4Tubs("sipmLidBase", 10.16 / 2 * mm, 120.65 / 2 * mm, 8.89 / 2 * mm,
                                   0, 360 * deg);

  G4Tubs *sipmLidSource = new G4Tubs("sipmLidSource", 0, 20 / 2 * mm, 6.096 / 2 * mm, 0, 360 * deg);

  G4SubtractionSolid *sipmLid = new G4SubtractionSolid("sipmLid", sipmLidBase, sipmLidSource, Rot0,
                                                       G4ThreeVector(0, 0, -(2.794 + 6.096 / 2) * mm));

  // table Source
  G4Tubs *SourceAlo = new G4Tubs("SourceAlo", 0.9398 / 2 * cm, 1.269 / 2 * cm, 0.31749 / 2 * cm, 0, 360 * deg);
  G4Tubs *SourceAl1 = new G4Tubs("SourceAl1", 0 * cm, 1.27 / 2 * cm, 0.635 * cm, 0, 360 * deg);
  G4Tubs *SourceAl2 = new G4Tubs("SourceAl2", 0 * cm, 0.31623 / 2 * cm, 0.3175 * cm, 0, 360 * deg);
  G4Tubs *SourceMy = new G4Tubs("SourceMy", 0 * cm, 0.9398 / 2 * cm, 0.00032 * cm, 0, 360 * deg);

  G4RotationMatrix *yRot = new G4RotationMatrix; // Rotates X and Z axes only
  yRot->rotateY(0 * rad);                        // Rotates 45 degrees
  G4ThreeVector zTrans(0, 0, -0.635 * cm + 0.3175 * cm / 2);
  G4SubtractionSolid *SourceAl = new G4SubtractionSolid("SourceAl", SourceAl1, SourceAl2, yRot, zTrans);

  // Blake SiPM design (2024)
  G4ThreeVector posb2 = G4ThreeVector(0 * cm, 0 * cm, -(3.317 * mm + (3.0 + 0.0762) / 2 * mm));

  G4LogicalVolume *logicAlMylarMy1 = new G4LogicalVolume(solidAlMy, Mylar, "Mylar_sq1");
  new G4PVPlacement(0, posb2, logicAlMylarMy1, "Mylar_sq1", logicWorld, false, 0, checkOverlaps);
  logicAlMylarMy1->SetUserLimits(Limits);

  G4LogicalVolume *logicAlMylarAli1 = new G4LogicalVolume(solidAlMyI, Al, "Ali_sq1");
  new G4PVPlacement(0, posb2, logicAlMylarAli1, "Ali_sq1", logicWorld, false, 0, checkOverlaps);
  logicAlMylarAli1->SetUserLimits(Limits);

  G4LogicalVolume *logicAlMylarAlo1 = new G4LogicalVolume(solidAlMyO, Al, "Alo_sq1");
  new G4PVPlacement(0, posb2, logicAlMylarAlo1, "Alo_sq1", logicWorld, false, 0, checkOverlaps);
  logicAlMylarAlo1->SetUserLimits(Limits);

  flogicDetector3 = new G4LogicalVolume(SmallScin, PVT, "Detector_sq1");
  new G4PVPlacement(0, posb2, flogicDetector3, "Detector_sq1", logicWorld, false, 0, checkOverlaps);
  flogicDetector3->SetUserLimits(Limits);

  G4ThreeVector posb3 = G4ThreeVector(0 * cm, 0 * cm, -(3.317 * mm + (3.0 + 0.0762) * 3 / 2 * mm));

  G4LogicalVolume *logicAlMylarMy2 = new G4LogicalVolume(solidAlMy2, Mylar, "Mylar_sq2");
  new G4PVPlacement(0, posb3, logicAlMylarMy2, "Mylar_sq2", logicWorld, false, 0, checkOverlaps);
  logicAlMylarMy2->SetUserLimits(Limits);

  G4LogicalVolume *logicAlMylarAli2 = new G4LogicalVolume(solidAlMyI2, Al, "Ali_sq2");
  new G4PVPlacement(0, posb3, logicAlMylarAli2, "Ali_sq2", logicWorld, false, 0, checkOverlaps);
  logicAlMylarAli2->SetUserLimits(Limits);

  G4LogicalVolume *logicAlMylarAlo2 = new G4LogicalVolume(solidAlMyO2, Al, "Alo_sq2");
  new G4PVPlacement(0, posb3, logicAlMylarAlo2, "Alo_sq2", logicWorld, false, 0, checkOverlaps);
  logicAlMylarAlo2->SetUserLimits(Limits);

  flogicDetector5 = new G4LogicalVolume(SmallScin, PVT, "Detector_sq2");
  new G4PVPlacement(0, posb3, flogicDetector5, "Detector_sq2", logicWorld, false, 0, checkOverlaps);
  flogicDetector5->SetUserLimits(Limits);

  G4LogicalVolume *logicSiPMLid = new G4LogicalVolume(sipmLid, Cu, "sipm_Lid");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -(32.079 + 8.89 / 2) * mm), logicSiPMLid, "sipm_Lid",
                    logicWorld, false, 0, checkOverlaps);
  logicSiPMLid->SetUserLimits(Limits);

  G4LogicalVolume *logicScintHolder = new G4LogicalVolume(scintHolder, mat_PCB, "scint_holder");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -(2.317 + 27.4 / 2) * mm), logicScintHolder, "scint_holder",
                    logicWorld, false, 0, checkOverlaps);
  logicScintHolder->SetUserLimits(Limits);

  G4LogicalVolume *logicScintHolderBase = new G4LogicalVolume(scintHolderBase, mat_PCB, "scint_holderBase");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -(29.717 + 1.575 / 2) * mm), logicScintHolderBase, "scint_holderBase",
                    logicWorld, false, 0, checkOverlaps);
  logicScintHolderBase->SetUserLimits(Limits);

  G4LogicalVolume *logicScintSleeve = new G4LogicalVolume(scintSleeve, Al, "scint_Sleeve");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -(9.467 + 10 / 2) * mm), logicScintSleeve, "scint_Sleeve",
                    logicWorld, false, 0, checkOverlaps);
  logicScintSleeve->SetUserLimits(Limits);

  G4double possource = -3.4873 * cm;
  G4RotationMatrix *RotSource = new G4RotationMatrix;
  RotSource->rotateX(2 * 3.14159265 * rad);

  G4LogicalVolume *logicSourceMy = new G4LogicalVolume(SourceMy, Mylar, "SourceMy");
  new G4PVPlacement(RotSource, G4ThreeVector(0, 0, possource), logicSourceMy, "SourceMy",
                    logicWorld, false, 0, checkOverlaps);
  logicSourceMy->SetUserLimits(Limits);

  G4LogicalVolume *logicSourceAl = new G4LogicalVolume(SourceAl, Al, "SourceAl");
  new G4PVPlacement(RotSource, G4ThreeVector(0, 0, possource - 0.8 * cm), logicSourceAl, "SourceAl",
                    logicWorld, false, 0, checkOverlaps);
  logicSourceAl->SetUserLimits(Limits);

  G4LogicalVolume *logicSourceAlo = new G4LogicalVolume(SourceAlo, Al, "SourceAlo");
  new G4PVPlacement(RotSource, G4ThreeVector(0, 0, possource - 0 * cm), logicSourceAlo, "SourceAlo",
                    logicWorld, false, 0, checkOverlaps);
  logicSourceAlo->SetUserLimits(Limits);

  // always return the physical World
  return physWorld;
}

void BM_Detector::ConstructSDandField()
{
  /*
  NOTE: although this function is not explicitly called in BetaMon, it is still used.
  ConstructSDandField() is invoked in G4RunManager::InitializeGeometry() alongside Construct().
  If run with multithreading mode, it is invoked for each thread additionally from G4WorkerRunManager::InitializeGeometry().
  https://geant4-forum.web.cern.ch/t/constructsdandfield-in-multi-threaded-mode/2986
  */
  SDMan = G4SDManager::GetSDMpointer();

  G4VSensitiveDetector *ScinTrig = new BM_SD("Det_trig", "Det_trig_HC"); // trigger
  G4VSensitiveDetector *Scinsq = new BM_SD("Det_sq", "Det_sq_HC");
  G4VSensitiveDetector *SDWindow = new BM_SD("Det_window", "Det_wind_HC"); // window foil
  G4VSensitiveDetector *SDVac = new BM_SD("Det_vac", "Det_vac_HC");

  // Add the silicon detectors to the Sens.Det.Management
  SDMan->AddNewDetector(Scinsq);
  SDMan->AddNewDetector(SDVac);
  SDMan->AddNewDetector(SDWindow);
  SDMan->AddNewDetector(ScinTrig);

  // Turn on the sensitive detectors
  flogicDetector1->SetSensitiveDetector(SDWindow);
  flogicDetector6->SetSensitiveDetector(SDVac);
  flogicDetector3->SetSensitiveDetector(Scinsq);
  flogicDetector5->SetSensitiveDetector(ScinTrig);

  // Magnetic field
  G4double amplitude = 0. * gauss;
  G4double theta = 60. * degree;
  G4MagneticField *MagneticField = new G4UniformMagField(G4ThreeVector(amplitude * sin(theta * 3.141592653 / 180), 
                                                          0., amplitude * cos(theta * 3.141592653 / 180)));
  G4FieldManager *globalFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  globalFieldMgr->SetDetectorField(MagneticField);
  globalFieldMgr->CreateChordFinder(MagneticField);
  flogicDetector6->SetFieldManager(globalFieldMgr, false);

  fScoringVolume = logicWorld;
}