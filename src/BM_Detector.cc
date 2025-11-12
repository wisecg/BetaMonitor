// BM_Detector.cc - Implementation of BM_Detector class: BetaMon detector and geometry

#include "BM_Detector.hh"
#include "BM_SD.hh"
#include "BM_SteppingAction.hh"
#include "MagneticField.hh"

// Visualization-related
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

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
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"

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

// MagField* BM_Detector::fMagneticField = 0;
G4FieldManager *BM_Detector::fFieldMgr = 0;
G4UniformMagField *BM_Detector::fMagneticField = 0;

BM_Detector::BM_Detector() : G4VUserDetectorConstruction(),
                             flogicDetector6(nullptr), fLogicMagnetic(nullptr), flogicDetector1(nullptr),
                             flogicDetector5(nullptr), flogicDetector3(nullptr), logicEnv(nullptr), logicWorld(nullptr) {}

BM_Detector::~BM_Detector() {}

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"

G4VPhysicalVolume *BM_Detector::Construct()

{
    // TODO: implement options to turn on/off certain volumes.  Need a separate function for detector, bore, and source
    G4cout << "Running sim with Blake's SiPM geometry: SmallScin, Al, cone flange" << G4endl;
    
    // G4bool OrigMon = false; //top round, bottom round, square, Cu, normal flange
    // G4bool SiPMWinston = false; // LargeScin, Cu, normal flange
    // G4bool TeleOld = false;     // thinTele, Cu, normal flange
    // G4bool SiPMBlake = true;    // SmallScin, Al, cone flange
    // G4bool TeleTAMU = false; // TAMUdE, ?, normal flange?
    // G4bool Beamline = true;   // adds the flange and decay volume geometry
    // G4bool TabSource = false; // adds table source geometry

    // Get nist material manager
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

    // Vacuum parameters
    G4double atomicNumber = 1.;
    G4double massOfMole = 1.008 * g / mole;
    G4double density2 = 1.e-25 * g / cm3;
    G4double temperature = 2.73 * kelvin;
    G4double pressure = 1.3332e-8 * pascal;
    G4Material *Vacuum = new G4Material("interGalactic", atomicNumber, massOfMole, density2, kStateGas, temperature, pressure);

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    // World
    G4double world_sizeXY = 18 * cm;
    G4double world_sizeZ = 40.75 * cm;
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");
    G4Box *solidWorld =
        new G4Box("World",                                                    // its name
                  0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ); // its size
    
    logicWorld =
        new G4LogicalVolume(solidWorld, // its solid
                            world_mat,  // its material
                            "World");   // its name
    G4VPhysicalVolume *physWorld =
        new G4PVPlacement(0,               // no rotation
                          G4ThreeVector(), // at (0,0,0)
                          logicWorld,      // its logical volume
                          "World",         // its name
                          0,               // its mother volume
                          false,           // no boolean operation
                          0,               // copy number
                          checkOverlaps);  // overlaps checking
    G4UserLimits *Limits = new G4UserLimits(10 * um);
    logicWorld->SetUserLimits(Limits);

    // Decay Volume Geometry
    // G4double cyl_r1dvo = 3.81 / 2 * cm;   // Outer Radius of Decay Volume
    G4double cyl_r1dvi = 3.4798 / 2 * cm; // Inner Radius of Decay Volume
    G4double cyl_hdv = 6.75 * 2.54 * cm;  // 12.5476*cm;//length of decay volume 16.51 standard
    G4double cyl_r2dvo = 6.9088 / 2 * cm; // Flange Outer Radius

    // T Decay volume Geometry
    G4double Tdv_r1o = 3.81 / 2 * cm;       // outer radius of DV
    G4double Tdv_r1i = 3.4798 / 2 * cm;     // inner radius of DV
    G4double Tdv_H1 = 6.75 * 2.54 * cm;     // Major axis length of DV
    G4double Tdv_h2 = 6.75 * 2.54 / 2 * cm; // Minor axis length of DV

    // Vacuum
    // G4Tubs *solidEnv = new G4Tubs("Envelope", 0 * cm, cyl_r1dvi, cyl_hdv / 2, 0, 360 * deg);

    // Decay Volume
    // G4Tubs *solidShape5 = new G4Tubs("Shape5", cyl_r1dvi, cyl_r2dvo, cyl_hdv / 2., 0, 360 * deg);
    // G4Tubs *solidShape5s = new G4Tubs("Shape5s", cyl_r1dvo, cyl_r2dvo * 1.1, (cyl_hdv) / 2. - 1.27 * cm, 0, 360 * deg);
    // G4SubtractionSolid *solidDecayVol = new G4SubtractionSolid("DecayVolume", solidShape5, solidShape5s);
    // G4SubtractionSolid *solidDecayVol2 = new G4SubtractionSolid("2ndDecayVolume", solidShape5, solidShape5s);

    // Decay Volume T
    G4Tubs *solidShapeT1o = new G4Tubs("Pipe1o", 0, Tdv_r1o, Tdv_H1 / 2., 0, 360 * deg);
    G4Tubs *solidShapeT1i = new G4Tubs("Pipe1i", 0, Tdv_r1i, Tdv_H1 / 1.9, 0, 360 * deg);
    G4Tubs *solidShapeT2o = new G4Tubs("Pipe2o", 0, Tdv_r1o, Tdv_h2 / 2, 0, 360 * deg);
    G4Tubs *solidShapeT2i = new G4Tubs("Pipe2i", 0, Tdv_r1i, Tdv_h2 / 2, 0, 360 * deg);
    G4RotationMatrix *yRotT = new G4RotationMatrix;
    yRotT->rotateY(3.14159265 / 2 * rad); // Rotates 90 degrees
    G4ThreeVector TransT(Tdv_r1i / 2 - Tdv_h2 / 2, 0, 0);
    G4ThreeVector TransT2(Tdv_r1i / 2 - Tdv_h2, 0, 0);

    G4UnionSolid *Pipeo = new G4UnionSolid("OuterTPipe", solidShapeT1o, solidShapeT2o, yRotT, TransT);
    G4UnionSolid *Pipei = new G4UnionSolid("InnerTPipe", solidShapeT1i, solidShapeT2i, yRotT, TransT);
    G4SubtractionSolid *TPipe1 = new G4SubtractionSolid("TDecayVolume", Pipeo, solidShapeT1i);
    G4SubtractionSolid *TPipe = new G4SubtractionSolid("TDecayVolume", TPipe1, solidShapeT2i, yRotT, TransT2);

    // Decay Volume Flange
    G4Tubs *solidFlange = new G4Tubs("Flange", 0 * cm, cyl_r2dvo, 1.2700 / 2 * cm, 0, 360 * deg);
    G4Tubs *boreFlange = new G4Tubs("boreFlange", 0 * cm, cyl_r1dvi, 1.27 * cm, 0, 360 * deg);
    // G4Tubs *solidBlakeFlange = new G4Tubs("Flange", 0 * cm, 9.525 / 2 * cm, 1.2700 / 2 * cm, 0, 360 * deg);
    // G4Tubs *boreBlakeFlange = new G4Tubs("boreBlakeFlange", 0 * cm, cyl_r1dvi, 6.52 / 2 * mm, 0, 360 * deg);
    // G4Cons *coneBoreBlake = new G4Cons("aperature", 0 * cm, 19.8 / 2 * mm, 0, 31.8 / 2 * mm, 6.333 / 2 * mm, 0, 360 * deg);
    
    G4RotationMatrix *Rot0 = new G4RotationMatrix;
    Rot0->rotateX(0 * rad);

    // Vacuum Seal
    G4double cyl_r2c = 3.556 / 2 * cm; // copper seal radius
    G4double cyl_hc = 0.0254 * cm;     // thickness of copper
    // G4double cyl_hkap = 0.0012 * cm;   // thickness of kapton (trials,1:0.0012, 2: 0.00075, 3: 0.006, 4: 0.0127)
    G4double cyl_hAl = 0.0762 * mm;    // thickness of Aluminum only used in SIPMBlake
    G4Tubs *solidShape4 = new G4Tubs("Shape4", 0. * cm, cyl_r2c, cyl_hc / 2., 0, 360 * deg);
    // G4Tubs *solidShapeKap = new G4Tubs("Kap", 0. * cm, cyl_r2c, cyl_hkap / 2., 0, 360 * deg);
    // G4Tubs *solidShapeAl = new G4Tubs("ShapeAl", 0. * cm, 19.8 / 2 * mm, cyl_hAl / 2., 0, 360 * deg);

    G4ThreeVector zTranscone(0, 0, 6.293 / 2 * mm + cyl_hAl / 2);
    G4ThreeVector zTransbore(0, 0, -6.477 / 2 * mm);

    G4SubtractionSolid *Flangen = new G4SubtractionSolid("Flange", solidFlange, boreFlange);
    // G4SubtractionSolid *Flange1 = new G4SubtractionSolid("Flange1", solidBlakeFlange, coneBoreBlake, Rot0, zTranscone);
    // G4SubtractionSolid *Flange = new G4SubtractionSolid("Flange", Flange1, boreBlakeFlange, Rot0, zTransbore);

    logicEnv =
        new G4LogicalVolume(Pipei,        // its solid
                            Vacuum,      // its material
                            "Envelope"); // its name
    // TODO rework such that port is at 0,0,0?
    new G4PVPlacement(0,                                                          // no rotation
                      G4ThreeVector(0 * cm, 0 * cm, cyl_hdv / 2 + 2 * 1.27 * cm), // 11.13765*cm-2.31115*cm+3.9624/2*cm),         //old (-2.68205602104/2*cm, -2.68205602104/2*cm, 11.13765*cm+cyl_hdv/2)
                      logicEnv,                                                   // its logical volume
                      "Envelope",                                                 // its name
                      logicWorld,                                                 // its mother volume
                      false,                                                      // no boolean operation
                      0,                                                          // copy number
                      checkOverlaps);                                             // overlaps checking
    logicEnv->SetUserLimits(Limits);

    flogicDetector6 =
        new G4LogicalVolume(Pipei,          // its solid
                            Vacuum,         // its material
                            "Envelopedet"); // its name

    new G4PVPlacement(0,                                                          // no rotation
                      G4ThreeVector(0 * cm, 0 * cm, cyl_hdv / 2 + 2 * 1.27 * cm), // old (-2.68205602104/2*cm, -2.68205602104/2*cm, 11.13765*cm+cyl_hdv/2)
                      flogicDetector6,                                            // its logical volume
                      "Envelopedet",                                              // its name
                      logicWorld,                                                 // its mother volume
                      false,                                                      // no boolean operation
                      0,                                                          // copy number
                      checkOverlaps);                                             // overlaps checking
    flogicDetector6->SetUserLimits(Limits);

    // detector positions
    // TODO make thise solid numbers
    G4ThreeVector posb = G4ThreeVector(0 * cm, 0 * cm, (2.5 + 2.5 * 0.1524 + 0.066 + 7.05) * mm);     // DIFFERENCE: 10 mm -(1/2 detector + 2 tape + mylar+7.05 +1/2 tape)
    G4ThreeVector posb2 = G4ThreeVector(0 * cm, 0 * cm, -(3.317 * mm + (3.0 + 0.0762) / 2 * mm));     //(2.5+2.5*0.1524+0.066+7.05+1-40)*mm);
    G4ThreeVector posb3 = G4ThreeVector(0 * cm, 0 * cm, -(3.317 * mm + (3.0 + 0.0762) * 3 / 2 * mm)); //(2.5+2.5*0.1524+7.05-2-40)*mm);
    // G4ThreeVector posa = G4ThreeVector(2.68205602104/2*cm, 2.68205602104/2*cm, -1.524*cm);
    // G4ThreeVector posc = G4ThreeVector(-2.68205602104/2*cm, -2.68205602104/2*cm, 1.524*cm);
    G4ThreeVector poscu = G4ThreeVector(0 * cm, 0 * cm, 1.27 * cm + cyl_hc / 2);   // 24.05 + difference 1.4mm old Z: (4.86385-2.31115)*cm
    G4ThreeVector posAl = G4ThreeVector(0 * cm, 0 * cm, 6.477 * mm + cyl_hAl / 2); // 24.05 + difference 1.4mm old Z: (4.86385-2.31115)*cm
    G4ThreeVector posdv2 = G4ThreeVector(0 * cm, 0 * cm, 1.27 / 2 * cm);           // difference 1.4mm old Z: (4.22885-2.31115)*cm
    // Decay Volume uncomment 1 & 3
    //  G4LogicalVolume* logicDecayVolume1 =
    //    new G4LogicalVolume(solidDecayVol,         //its solid
    //                        Stainless_Steel,          //its material
    //                        "Decay_Volume");           //its name
    //  new G4PVPlacement(0,                       //no rotation
    //                    G4ThreeVector(0, 0, 0), //posdv,                    //at position
    //                    logicDecayVolume1,             //its logical volume
    //                    "Decay_Volume",                //its name
    //                    logicEnv,                //its mother volume
    //                    false,                   //no boolean operation
    //                    0,                       //copy number
    //                    checkOverlaps);          //overlaps checking
    //  logicDecayVolume1->SetUserLimits(Limits);

    // 2nd decay volume DOUBLE ENVELOPE LENGTH IF IN USE
    //  G4LogicalVolume* logicDecayVolume2 =
    //  //   new G4LogicalVolume(solidDecayVol2,         //its solid
    //  //                       Stainless_Steel,          //its material
    //  //                       "Decay_Volume");           //its name
    //  // new G4PVPlacement(0,                       //no rotation
    //  //                   G4ThreeVector(0*cm, 0*cm, cyl_hdv/2), //posdv,                    //at position
    //  //                   logicDecayVolume2,             //its logical volume
    //  //                   "Decay_Volume",                //its name
    //  //                   logicEnv,                //its mother volume
    //  //                   false,                   //no boolean operation
    //  //                   0,                       //copy number
    //  //                   checkOverlaps);          //overlaps checking
    //  // logicDecayVolume2->SetUserLimits(Limits);

    G4LogicalVolume *logicDecayVolume3 =
        new G4LogicalVolume(Flangen,           // its solid //////Flange if Blake, Flangen for generic
                            Stainless_Steel,   // its material
                            "Decay_Volume_r"); // its name
    new G4PVPlacement(0,                       // no rotation
                      posdv2,                  // posdv,                    //at position
                      logicDecayVolume3,       // its logical volume
                      "Decay_Volume_r",        // its name
                      logicWorld,              // its mother volume
                      false,                   // no boolean operation
                      0,                       // copy number
                      checkOverlaps);          // overlaps checking
    logicDecayVolume3->SetUserLimits(Limits);

    G4LogicalVolume *logicDecayVolume6 =
        new G4LogicalVolume(Flangen,                                    // its solid //////Flange if Blake, Flangen for generic
                            Stainless_Steel,                            // its material
                            "Decay_Volume_r");                          // its name
    new G4PVPlacement(0,                                                // no rotation
                      G4ThreeVector(0 * cm, 0 * cm, 3 * 1.27 / 2 * cm), // posdv,                    //at position
                      logicDecayVolume6,                                // its logical volume
                      "Decay_Volume_r",                                 // its name
                      logicWorld,                                       // its mother volume
                      false,                                            // no boolean operation
                      0,                                                // copy number
                      checkOverlaps);                                   // overlaps checking
    logicDecayVolume6->SetUserLimits(Limits);
    G4LogicalVolume *logicDecayVolume4 =
        new G4LogicalVolume(Flangen,                                // its solid
                            Stainless_Steel,                        // its material
                            "Decay_Volume_r");                      // its name
    new G4PVPlacement(0,                                            // no rotation
                      G4ThreeVector(0, 0, cyl_hdv + 1.27 * 2 * cm), // posdv,                    //at position
                      logicDecayVolume4,                            // its logical volume
                      "Decay_Volume_r",                             // its name
                      logicWorld,                                   // its mother volume
                      false,                                        // no boolean operation
                      0,                                            // copy number
                      checkOverlaps);                               // overlaps checking
    logicDecayVolume4->SetUserLimits(Limits);
    G4LogicalVolume *logicDecayVolume5 =
        new G4LogicalVolume(Flangen,                                                          // its solid
                            Stainless_Steel,                                                  // its material
                            "Decay_Volume_r");                                                // its name
    new G4PVPlacement(yRotT,                                                                  // no rotation
                      G4ThreeVector(-Tdv_h2 + 1.27 / 2 * cm, 0, cyl_hdv / 2 + 1.27 * 2 * cm), // posdv,                    //at position
                      logicDecayVolume5,                                                      // its logical volume
                      "Decay_Volume_r",                                                       // its name
                      logicWorld,                                                             // its mother volume
                      false,                                                                  // no boolean operation
                      0,                                                                      // copy number
                      false);                                                                 // overlaps checking
    logicDecayVolume5->SetUserLimits(Limits);
    flogicDetector1 =                       // G4LogicalVolume* logicDecayVolume2 =
        new G4LogicalVolume(solidShape4,    // its solid SolidShape4/Kap/Al
                            Al,             // its material Cu/Kap/ Al (new)
                            "Copper");      // its name "Copper"/"Kapton"/"Aluminum"
    new G4PVPlacement(0,                    // no rotation
                      poscu,                // at position
                      flogicDetector1,      // logicDecayVolume2,             //its logical volume
                      "Copper",             // its name
                      logicWorld,           // its mother volume
                      false,                // no boolean operation
                      0,                    // copy number
                      checkOverlaps);       // overlaps checking
    flogicDetector1->SetUserLimits(Limits); // logicDecayVolume2->SetUserLimits(Limits);
    // T Pipe
    G4LogicalVolume *logicDecayVolume1 =
        new G4LogicalVolume(TPipe,                                      // its solid
                            Stainless_Steel,                            // its material
                            "Decay_Volume");                            // its name
    new G4PVPlacement(0,                                                // no rotation
                      G4ThreeVector(0, 0, cyl_hdv / 2 + 1.27 * 2 * cm), // posdv,                    //at position
                      logicDecayVolume1,                                // its logical volume
                      "Decay_Volume",                                   // its name
                      logicWorld,                                       // its mother volume
                      false,                                            // no boolean operation
                      0,                                                // copy number
                      checkOverlaps);                                   // overlaps checking
    logicDecayVolume1->SetUserLimits(Limits);

    // G4LogicalVolume* logicDecayVolume77 =
    //   new G4LogicalVolume(Pipei,         //its solid
    //                       Stainless_Steel,          //its material
    //                       "Decay_Volume");           //its name
    // new G4PVPlacement(0,                       //no rotation
    //                   G4ThreeVector(0, -65, 0), //posdv,                    //at position
    //                   logicDecayVolume1,             //its logical volume
    //                   "Decay_Volume",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // logicDecayVolume77->SetUserLimits(Limits);
    //
    // Shape 1
    //
    // G4ThreeVector pos1 = G4ThreeVector(0, 0, ((4.5+.525)*2.54+.5+.0065)*cm);

    // Cylinder shape
    // plastic
    // G4double cyl_r1p = 0.*cm;
    // G4double cyl_r2p = 6.13156/2*cm;
    // G4double cyl_hp = 0.217932*cm;
    // G4Tubs* solidShape1 = new G4Tubs("Shape1", cyl_r1p, cyl_r2p, cyl_hp/2., 0, 360*deg);
    // //mylar
    // G4double cyl_r1m = 0.*cm;
    // G4double cyl_r2m = 6.349492/2*cm;
    // G4double cyl_hm = 0.21844*cm;
    // G4Tubs* solidShape2 = new G4Tubs("Shape2", cyl_r1m, cyl_r2m, cyl_hm/2., 0, 360*deg);
    // //tape
    // G4double cyl_r1t = 0.*cm;
    // G4double cyl_r2t = 6.35/2*cm;
    // G4double cyl_ht = 0.254*cm;
    // G4Tubs* solidShape3 = new G4Tubs("Shape3", cyl_r1t, cyl_r2t, cyl_ht/2., 0, 360*deg);
    // G4SubtractionSolid* solidShape7 = new G4SubtractionSolid("Tape-r", solidShape3, solidShape2);
    // G4SubtractionSolid* solidShape8 = new G4SubtractionSolid("Mylar-r", solidShape2, solidShape1);

    // OLD Telescope
    // telescope plastic
    // G4Tubs *telescope1 = new G4Tubs("telescope1", 0, 2.54 * cm, 2.54 / 2 * cm, 0, 360 * deg);
    // G4Box *telescope2 = new G4Box("telescope2", 2.54 * cm, 2.54 / 2 * cm, 2.54 / 2 * cm);
    // G4Tubs *thinTelescope = new G4Tubs("thinTele", 0, 2.54 * cm, 3 / 2 * mm, 0, 360 * deg);
    // telescope mylar
    // G4Tubs *telescope1my = new G4Tubs("telescope1my", 0, 2.54 * cm + 0.066 * mm, 2 * 2.54 / 2 * cm + 0.066 * mm, 0, 360 * deg);
    // G4Box *telescope2my = new G4Box("telescope2my", 2.54 * cm + 0.066 * mm, 2 * 2.54 / 2 * cm + 0.066 * mm, 2.54 / 2 * cm + 0.066 * mm);
    // G4Tubs *thinTelescopemy = new G4Tubs("thinTelemy", 0, 2.54 * cm + 0.066 * mm, 3 / 2 * mm + 0.066 * mm, 0, 360 * deg); // 3M is 0.033*2*mm glue and 0.38*mm "polymeric"
    // telescope-D tape
    // G4Tubs *telescope1t = new G4Tubs("telescope1t", 0, 2.54 * cm + 0.066 * mm + 0.2794 * mm * 0.5, 2 * 2.54 * 0.5 * cm + 0.066 * mm + 0.2794 * mm * 0.5, 0, 360 * deg);
    // G4Box *telescope2t = new G4Box("telescope2t", 2.54 * cm + 0.066 * mm + 0.2794 * mm * 0.5, 2 * 2.54 * 0.5 * cm + 0.066 * mm + 0.2794 * mm * 0.5, 2.54 * 0.5 * cm + 0.066 * mm + 0.2794 * mm * 0.5);
    // G4Tubs *thinTelescopet = new G4Tubs("thinTelet", 0, 2.54 * cm + 0.066 * mm + 1 * 0.5344 * mm, 1.5 * mm + 0.066 * mm + 0.5344 * mm * 0.5, 0, 360 * deg);

    G4RotationMatrix *xRot2 = new G4RotationMatrix; // Rotates X and Z axes only
    xRot2->rotateX(3.14159265 / 2 * rad);           // Rotates 45 degrees
    G4ThreeVector zTrans2(0, 0, 2.54 / 2 * cm);
    
    // Rotates 45 degrees
    // G4UnionSolid *telescopegeom1 = new G4UnionSolid("telescopegeom1", telescope2, telescope1, xRot2, zTrans2);
    // G4UnionSolid *telescopegeomMy1 = new G4UnionSolid("telescopegeommy1", telescope2my, telescope1my, xRot2, zTrans2);
    // G4UnionSolid *telescopegeomT1 = new G4UnionSolid("telescopegeomt1", telescope2t, telescope1t, xRot2, zTrans2);

    G4ThreeVector yTrans2(0, 2.54 * cm, 0);
    G4ThreeVector yTrans3(0, -2.54 / 2 * cm, 0);
    G4ThreeVector noTrans(0, 0, 0);
    // G4UnionSolid *telescopegeom = new G4UnionSolid("telescopegeom", telescopegeom1, telescopegeom1, Rot0, yTrans2);
    // G4SubtractionSolid *telescopegeomT = new G4SubtractionSolid("telescopegeomT", telescopegeomT1, telescopegeomMy1, Rot0, noTrans);
    // G4SubtractionSolid *telescopegeomMy = new G4SubtractionSolid("telescopegeomMy", telescopegeomMy1, telescopegeom, Rot0, yTrans3);
    // G4SubtractionSolid *thinTeleT = new G4SubtractionSolid("Tape-th", thinTelescopet, thinTelescopemy);
    // G4SubtractionSolid *thinTeleMy = new G4SubtractionSolid("Mylar-th", thinTelescopemy, thinTelescope);

    // New TAMU Telescope
    // G4Tubs *tamuTeledE = new G4Tubs("tamuTeleDE", 0 * cm, 3.175 * cm, 0.09906 / 2 * cm, 0, 360 * deg);
    // G4Tubs *tamuTeleE = new G4Tubs("tamuTeleE", 0 * cm, 3.175 * cm, 1.99898 / 2 * cm, 0, 360 * deg);
    // Winston SiPM
    // large plastic
    // G4Box* LargeScin =
    //   new G4Box("LargeScin",                      //its name
    //             20/2*mm, 20/2*mm, //old-10.123932/2*cm
    //             5/2*mm); //its size old 0.725932/2*cm
    // //large Mylar This is 3M, the glue and "polymeric" are approximmated as normal mylar
    // G4Box* LargeScinM =
    //   new G4Box("LargeScinM",                      //its name
    //             20.0/2*mm+0.066*mm, 20.0/2*mm+0.066*mm, //old-10.12444/2*cm
    //             5.0/2*mm+0.066*mm); //its size old-0.72644/2*cm
    // //large tape
    // G4Box* LargeScinT =
    //   new G4Box("LargeScinT",                      //its name
    //             20/2*mm+0.066*mm+3*0.1524*mm, 20/2*mm+0.066*mm+3*0.1524*mm, //old-10.16/2*cm20.36068 -4 was the running one for Mon1, BiOct1,SrY2
    //             5.0/2*mm+0.066*mm+3*0.1524*mm); //its size old-0.762/2*cm 0.30988*1.5=0.46482
    // G4SubtractionSolid* solidShape9 = new G4SubtractionSolid("Tape-sq", LargeScinT, LargeScinM);
    // G4SubtractionSolid* solidShape10 = new G4SubtractionSolid("Mylar-sq", LargeScinM, LargeScin);

    // Blake SiPM
    // small plastic
    G4Box *SmallScin =
        new G4Box("SmallScin", // its name
                  20.0 / 2 * mm, 20.0 / 2 * mm,
                  3.0 / 2 * mm); // its size
    // small Aluminized Mylar
    G4Box *SmallScinMA =
        new G4Box("SmallScinMA", // its name
                  20 / 2 * mm + 0.0001 * mm, 20 / 2 * mm + 0.0001 * mm,
                  3.0 / 2 * mm + 0.0001 * mm); // its size
    G4Box *SmallScinM =
        new G4Box("SmallScinM", // its name
                  20.0 / 2 * mm + 0.0761 * mm, 20.0 / 2 * mm + 0.0761 * mm,
                  3.0 / 2 * mm + 0.0761 * mm); // its size
    G4Box *SmallScinMA2 =
        new G4Box("SmallScinMAOuter",                                                                     // its name
                  20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm, 20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm, //
                  3.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm);                                             // its size

    G4Box *SmallScinMA2nd =
        new G4Box("SmallScinMA", // its name
                  20 / 2 * mm + 0.0001 * mm, 20 / 2 * mm + 0.0001 * mm,
                  3.0 / 2 * mm + 0.0001 / 2 * mm); // its size
    G4Box *SmallScinM2nd =
        new G4Box("SmallScinM", // its name
                  20.0 / 2 * mm + 0.0761 * mm, 20.0 / 2 * mm + 0.0761 * mm,
                  3.0 / 2 * mm + 0.0761 / 2 * mm); // its size
    G4Box *SmallScinMA22nd =
        new G4Box("SmallScinMAOuter", // its name
                  20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm, 20.0 / 2 * mm + 0.0762 * mm + 0.00055 * mm,
                  3.0 / 2 * mm + 0.0762 / 2 * mm); // its size

    G4SubtractionSolid *solidAlMyO = new G4SubtractionSolid("OuterAl", SmallScinMA2, SmallScinM);
    G4SubtractionSolid *solidAlMy = new G4SubtractionSolid("Mylar", SmallScinM, SmallScinMA);
    G4SubtractionSolid *solidAlMyI = new G4SubtractionSolid("InnerAl", SmallScinMA, SmallScin);

    G4SubtractionSolid *solidAlMyO2 = new G4SubtractionSolid("OuterAl2", SmallScinMA22nd, SmallScinM2nd, Rot0, G4ThreeVector(0, 0, -0.0001 / 2 * mm));
    G4SubtractionSolid *solidAlMy2 = new G4SubtractionSolid("Mylar2", SmallScinM2nd, SmallScinMA2nd, Rot0, G4ThreeVector(0, 0, -0.0761 / 2 * mm));
    G4SubtractionSolid *solidAlMyI2 = new G4SubtractionSolid("InnerAl2", SmallScinMA2nd, SmallScin, Rot0, G4ThreeVector(0, 0, -0.0762 / 2 * mm));

    G4Box *scintHolderblock =
        new G4Box("scintHolderbase", // its name
                  22.2 / 2 * mm, 22.2 / 2 * mm,
                  27.4 / 2 * mm);
    G4Box *scintHoldercutout1 =
        new G4Box("scintHoldercutout1", // its name
                  22.2 / 2 * mm - 0.85 * mm, 22.2 / 2 * mm - 0.85 * mm,
                  26.4 / 2 * mm);
    G4Box *scintHoldercutout2 =
        new G4Box("scintHoldercutout2", // its name
                  18 / 2 * mm, 18 / 2 * mm,
                  30 / 2 * mm);
    G4SubtractionSolid *scintHolder1 = new G4SubtractionSolid("scintHolder1", scintHolderblock, scintHoldercutout2);
    G4SubtractionSolid *scintHolder = new G4SubtractionSolid("scintHolder", scintHolder1, scintHoldercutout1); //,Rot0, G4ThreeVector(0,0,-1*mm));

    G4Box *scintSleevebase =
        new G4Box("scintSleevebase", // its name
                  20.5 / 2 * mm, 20.5 / 2 * mm,
                  10 / 2 * mm);
    G4Box *scintSleevecutout =
        new G4Box("scintHoldercutout2", // its name
                  16 / 2 * mm, 16 / 2 * mm,
                  30 / 2 * mm);
    G4SubtractionSolid *scintSleeve = new G4SubtractionSolid("scintSleeve", scintSleevebase, scintSleevecutout);

    G4Tubs *scintHolderBase = new G4Tubs("scintHolderbase", 20 / 2 * mm, 45.72 / 2 * mm, 1.575 / 2 * mm, 0, 360 * deg);

    G4Tubs *sipmLidBase = new G4Tubs("sipmLidBase", 10.16 / 2 * mm, 120.65 / 2 * mm, 8.89 / 2 * mm, 0, 360 * deg);
    G4Tubs *sipmLidSource = new G4Tubs("sipmLidSource", 0, 20 / 2 * mm, 6.096 / 2 * mm, 0, 360 * deg);
    G4SubtractionSolid *sipmLid = new G4SubtractionSolid("sipmLid", sipmLidBase, sipmLidSource, Rot0, G4ThreeVector(0, 0, -(2.794 + 6.096 / 2) * mm));
    // PCB
    // G4Box* PCBBoard =
    //   new G4Box("PCBBoard",                      //its name
    //             14.732/2*mm, 10/2*mm, //
    //             10/2*mm); //its size
    // G4Box* SiPM =
    //   new G4Box("SiPM",                      //its name
    //             1.35/2*mm, 3.3/2*mm, //
    //             3.3/2*mm); //its size
    // Sources
    // G4Box *BiSource = new G4Box("BiSource", 4.26974 / 2 * cm, 3.2639 / 2 * cm, 0.3437 / 2 * cm);                  

    // table Source
    G4Tubs *SourceAlo = new G4Tubs("SourceAlo", 0.9398 / 2 * cm, 1.269 / 2 * cm, 0.31749 / 2 * cm, 0, 360 * deg);
    G4Tubs *SourceAl1 = new G4Tubs("SourceAl1", 0 * cm, 1.27 / 2 * cm, 0.635 * cm, 0, 360 * deg);
    G4Tubs *SourceAl2 = new G4Tubs("SourceAl2", 0 * cm, 0.31623 / 2 * cm, 0.3175 * cm, 0, 360 * deg);
    // G4Tubs* SourceSS = new G4Tubs("SourceSS", 0*cm, 0.9398/2*cm, 0.0254*cm, 0, 360*deg);
    G4Tubs *SourceMy = new G4Tubs("SourceMy", 0 * cm, 0.9398 / 2 * cm, 0.00032 * cm, 0, 360 * deg);
    
    // Subtraction
    G4RotationMatrix *yRot = new G4RotationMatrix; // Rotates X and Z axes only
    yRot->rotateY(0 * rad);                        // Rotates 45 degrees
    G4ThreeVector zTrans(0, 0, -0.635 * cm + 0.3175 * cm / 2);
    G4SubtractionSolid *SourceAl = new G4SubtractionSolid("SourceAl", SourceAl1, SourceAl2, yRot, zTrans);

    // G4SubtractionSolid* solidShape12 = new G4SubtractionSolid("Aluminum-sq", LargeScin3M, LargeScin);

    // Logical Volumes
    // Tapes
    //   G4LogicalVolume* logicTape1 =
    //     new G4LogicalVolume(solidShape7,         //its solid
    //                         Tape,          //its material
    //                         "Tape_rt");           //its name
    //   new G4PVPlacement(0,                       //no rotation
    //                     posa,                    //at position
    //                     logicTape1,             //its logical volume
    //                     "Tape_rt",                //its name
    //                     logicWorld,                //its mother volume
    //                     false,                   //no boolean operation
    //                     0,                       //copy number
    //                     checkOverlaps);          //overlaps checking
    // logicTape1->SetUserLimits(Limits);
    //   G4LogicalVolume* logicTape2 =
    //     new G4LogicalVolume(solidShape7,         //its solid
    //                         Tape,          //its material
    //                         "Tape_rb");           //its name
    //   new G4PVPlacement(0,                       //no rotation
    //                     posc,                    //at position
    //                     logicTape2,             //its logical volume
    //                     "Tape_rb",                //its name
    //                     logicWorld,                //its mother volume
    //                     false,                   //no boolean operation
    //                     0,                       //copy number
    //                     checkOverlaps);          //overlaps checking
    //   logicTape2->SetUserLimits(Limits);

    // Mylars
    //  G4LogicalVolume* logicMylar1 =
    //    new G4LogicalVolume(solidShape8,         //its solid
    //                        Mylar,          //its material
    //                        "Mylar_rt");           //its name
    //  new G4PVPlacement(0,                       //no rotation
    //                    posa,                    //at position
    //                    logicMylar1,             //its logical volume
    //                    "Mylar_rt",                //its name
    //                    logicWorld,                //its mother volume
    //                    false,                   //no boolean operation
    //                    0,                       //copy number
    //                    checkOverlaps);          //overlaps checking
    //  logicMylar1->SetUserLimits(Limits);
    //  G4LogicalVolume* logicMylar2 =
    //    new G4LogicalVolume(solidShape8,         //its solid
    //                        Mylar,          //its material
    //                        "Mylar_rb");           //its name
    //  new G4PVPlacement(0,                       //no rotation
    //                    posc,                    //at position
    //                    logicMylar2,             //its logical volume
    //                    "Mylar_rb",                //its name
    //                    logicWorld,                //its mother volume
    //                    false,                   //no boolean operation
    //                    0,                       //copy number
    //                    checkOverlaps);          //overlaps checking
    //  logicMylar2->SetUserLimits(Limits);

    //  G4LogicalVolume* logicAlumMy =
    // new G4LogicalVolume(solidShape12,         //its solid
    // Al,          //its material
    // "Aluminum_sq");           //its name
    // new G4PVPlacement(0,                       //no rotation
    // posb,                    //at position
    // logicAlumMy,             //its logical volume
    // "Aluminum_sq",                //its name
    // logicWorld,                //its mother volume
    // false,                   //no boolean operation
    // 0,                       //copy number
    // checkOverlaps);          //overlaps checking
    // logicAlumMy->SetUserLimits(Limits);
    // Scintillators
    //  flogicDetector1 =
    //    new G4LogicalVolume(solidShape1,         //its solid
    //                        PVT,          //its material
    //                        "Detector_rt");           //its name
    //  new G4PVPlacement(0,                       //no rotation
    //                    posa,                    //at position
    //                    flogicDetector1,             //its logical volume
    //                    "Detector_rt",                //its name
    //                    logicWorld,                //its mother volume
    //                    false,                   //no boolean operation
    //                    0,                       //copy number
    //                    checkOverlaps);          //overlaps checking
    //  flogicDetector1->SetUserLimits(Limits);
    //  flogicDetector2 =
    //    new G4LogicalVolume(solidShape1,         //its solid
    //                        PVT,          //its material
    //                        "Detector_rb");           //its name
    //  new G4PVPlacement(0,                       //no rotation
    //                    posc,                    //at position
    //                    flogicDetector2,             //its logical volume
    //                    "Detector_rb",                //its name
    //                    logicWorld,                //its mother volume
    //                    false,                   //no boolean operation
    //                    0,                       //copy number
    //                    checkOverlaps);          //overlaps checking
    //  flogicDetector2->SetUserLimits(Limits);

    // SINGLE SCINT

    //     G4LogicalVolume* logicTape3 =
    //     new G4LogicalVolume(solidShape9,         //its solid
    //                         Tape,          //its material
    //                         "Tape_sq");           //its name
    //   new G4PVPlacement(0,                       //no rotation
    //                     posb,                    //at position posb
    //                     logicTape3,             //its logical volume
    //                     "Tape_sq",                //its name
    //                     logicWorld,                //its mother volume
    //                     false,                   //no boolean operation
    //                     0,                       //copy number
    //                     checkOverlaps);          //overlaps checking
    //   logicTape3->SetUserLimits(Limits);
    //  G4LogicalVolume* logicMylar3 =
    //     new G4LogicalVolume(solidShape10,         //its solid
    //                         Mylar,          //its material
    //                         "Mylar_sq");           //its name
    //   new G4PVPlacement(0,                       //no rotation
    //                     posb,                    //at position posb?
    //                     logicMylar3,             //its logical volume
    //                     "Mylar_sq",                //its name
    //                     logicWorld,                //its mother volume
    //                     false,                   //no boolean operation
    //                     0,                       //copy number
    //                     checkOverlaps);          //overlaps checking
    // logicMylar3->SetUserLimits(Limits);
    //  flogicDetector3 =
    //     new G4LogicalVolume(LargeScin,         //its solid
    //                         PVT,          //its material
    //                         "Detector_sq");           //its name
    //   new G4PVPlacement(0,                       //no rotation
    //                     posb,                    //at position posb? 10 mm shift looks close
    //                     flogicDetector3,             //its logical volume
    //                     "Detector_sq",                //its name
    //                     logicWorld,                //its mother volume
    //                     false,                   //no boolean operation
    //                     0,                       //copy number
    //                     checkOverlaps);          //overlaps checking
    //   flogicDetector3->SetUserLimits(Limits);

    // 2x 3mm scints
    //  G4LogicalVolume* logicTape3b1 =
    //    new G4LogicalVolume(solidShape9b,         //its solid
    //                        Tape,          //its material
    //                        "Tape_sq1");           //its name
    //  new G4PVPlacement(0,                       //no rotation
    //                    posb2,                    //at position posb
    //                    logicTape3b1,             //its logical volume
    //                    "Tape_sq1",                //its name
    //                    logicWorld,                //its mother volume
    //                    false,                   //no boolean operation
    //                    0,                       //copy number
    //                    checkOverlaps);          //overlaps checking
    //  logicTape3b1->SetUserLimits(Limits);

    //  ////////////////////////////Blake SiPM/////////////////
    G4LogicalVolume *logicAlMylarMy1 =
        new G4LogicalVolume(solidAlMy,    // its solid
                            Mylar,        // its material
                            "Mylar_sq1"); // its name
    new G4PVPlacement(0,                  // no rotation
                      posb2,              // at position posb?
                      logicAlMylarMy1,    // its logical volume
                      "Mylar_sq1",        // its name
                      logicWorld,         // its mother volume
                      false,              // no boolean operation
                      0,                  // copy number
                      checkOverlaps);     // overlaps checking
    logicAlMylarMy1->SetUserLimits(Limits);
    G4LogicalVolume *logicAlMylarAli1 =
        new G4LogicalVolume(solidAlMyI, // its solid
                            Al,         // its material
                            "Ali_sq1"); // its name
    new G4PVPlacement(0,                // no rotation
                      posb2,            // at position posb?
                      logicAlMylarAli1, // its logical volume
                      "Ali_sq1",        // its name
                      logicWorld,       // its mother volume
                      false,            // no boolean operation
                      0,                // copy number
                      checkOverlaps);   // overlaps checking
    logicAlMylarAli1->SetUserLimits(Limits);
    G4LogicalVolume *logicAlMylarAlo1 =
        new G4LogicalVolume(solidAlMyO, // its solid
                            Al,         // its material
                            "Alo_sq1"); // its name
    new G4PVPlacement(0,                // no rotation
                      posb2,            // at position posb?
                      logicAlMylarAlo1, // its logical volume
                      "Alo_sq1",        // its name
                      logicWorld,       // its mother volume
                      false,            // no boolean operation
                      0,                // copy number
                      checkOverlaps);   // overlaps checking
    logicAlMylarAlo1->SetUserLimits(Limits);
    flogicDetector3 =
        new G4LogicalVolume(SmallScin,       // its solid
                            PVT,             // its material
                            "Detector_sq1"); // its name
    new G4PVPlacement(0,                     // no rotation
                      posb2,                 // at position posb? 10 mm shift looks close
                      flogicDetector3,       // its logical volume
                      "Detector_sq1",        // its name
                      logicWorld,            // its mother volume
                      false,                 // no boolean operation
                      0,                     // copy number
                      checkOverlaps);        // overlaps checking
    flogicDetector3->SetUserLimits(Limits);

    G4LogicalVolume *logicAlMylarMy2 =
        new G4LogicalVolume(solidAlMy2,   // its solid
                            Mylar,        // its material
                            "Mylar_sq2"); // its name
    new G4PVPlacement(0,                  // no rotation
                      posb3,              // at position posb?
                      logicAlMylarMy2,    // its logical volume
                      "Mylar_sq2",        // its name
                      logicWorld,         // its mother volume
                      false,              // no boolean operation
                      0,                  // copy number
                      checkOverlaps);     // overlaps checking
    logicAlMylarMy2->SetUserLimits(Limits);
    G4LogicalVolume *logicAlMylarAli2 =
        new G4LogicalVolume(solidAlMyI2, // its solid
                            Al,          // its material
                            "Ali_sq2");  // its name
    new G4PVPlacement(0,                 // no rotation
                      posb3,             // at position posb?
                      logicAlMylarAli2,  // its logical volume
                      "Ali_sq2",         // its name
                      logicWorld,        // its mother volume
                      false,             // no boolean operation
                      0,                 // copy number
                      checkOverlaps);    // overlaps checking
    logicAlMylarAli2->SetUserLimits(Limits);
    G4LogicalVolume *logicAlMylarAlo2 =
        new G4LogicalVolume(solidAlMyO2, // its solid
                            Al,          // its material
                            "Alo_sq2");  // its name
    new G4PVPlacement(0,                 // no rotation
                      posb3,             // at position posb?
                      logicAlMylarAlo2,  // its logical volume
                      "Alo_sq2",         // its name
                      logicWorld,        // its mother volume
                      false,             // no boolean operation
                      0,                 // copy number
                      checkOverlaps);    // overlaps checking
    logicAlMylarAlo2->SetUserLimits(Limits);
    flogicDetector5 =
        new G4LogicalVolume(SmallScin,       // its solid
                            PVT,             // its material
                            "Detector_sq2"); // its name
    new G4PVPlacement(0,                     // no rotation
                      posb3,                 // at position posb? 10 mm shift looks close
                      flogicDetector5,       // its logical volume
                      "Detector_sq2",        // its name
                      logicWorld,            // its mother volume
                      false,                 // no boolean operation
                      0,                     // copy number
                      checkOverlaps);        // overlaps checking
    flogicDetector5->SetUserLimits(Limits);

    G4LogicalVolume *logicSiPMLid =
        new G4LogicalVolume(sipmLid,                                  // its solid
                            Cu,                                       // its material
                            "sipm_Lid");                              // its name
    new G4PVPlacement(0,                                              // no rotation
                      G4ThreeVector(0, 0, -(32.079 + 8.89 / 2) * mm), //-(0.5*25.4)*mm),                    //at position posb?
                      logicSiPMLid,                                   // its logical volume
                      "sipm_Lid",                                     // its name
                      logicWorld,                                     // its mother volume
                      false,                                          // no boolean operation
                      0,                                              // copy number
                      checkOverlaps);                                 // overlaps checking
    logicSiPMLid->SetUserLimits(Limits);

    G4LogicalVolume *logicScintHolder =
        new G4LogicalVolume(scintHolder,                             // its solid
                            mat_PCB,                                 // its material
                            "scint_holder");                         // its name
    new G4PVPlacement(0,                                             // no rotation
                      G4ThreeVector(0, 0, -(2.317 + 27.4 / 2) * mm), //-(0.5*25.4)*mm),                    //at position posb?
                      logicScintHolder,                              // its logical volume
                      "scint_holder",                                // its name
                      logicWorld,                                    // its mother volume
                      false,                                         // no boolean operation
                      0,                                             // copy number
                      checkOverlaps);                                // overlaps checking
    logicScintHolder->SetUserLimits(Limits);

    G4LogicalVolume *logicScintHolderBase =
        new G4LogicalVolume(scintHolderBase,                           // its solid
                            mat_PCB,                                   // its material
                            "scint_holderBase");                       // its name
    new G4PVPlacement(0,                                               // no rotation
                      G4ThreeVector(0, 0, -(29.717 + 1.575 / 2) * mm), //-(0.5*25.4)*mm),                    //at position posb?
                      logicScintHolderBase,                            // its logical volume
                      "scint_holderBase",                              // its name
                      logicWorld,                                      // its mother volume
                      false,                                           // no boolean operation
                      0,                                               // copy number
                      checkOverlaps);                                  // overlaps checking
    logicScintHolderBase->SetUserLimits(Limits);

    G4LogicalVolume *logicScintSleeve =
        new G4LogicalVolume(scintSleeve,                           // its solid
                            Al,                                    // its material
                            "scint_Sleeve");                       // its name
    new G4PVPlacement(0,                                           // no rotation
                      G4ThreeVector(0, 0, -(9.467 + 10 / 2) * mm), //-(0.5*25.4)*mm),                    //at position posb?
                      logicScintSleeve,                            // its logical volume
                      "scint_Sleeve",                              // its name
                      logicWorld,                                  // its mother volume
                      false,                                       // no boolean operation
                      0,                                           // copy number
                      checkOverlaps);                              // overlaps checking
    logicScintSleeve->SetUserLimits(Limits);

    G4double possource = -3.4873 * cm; // near//-5.15112*cm;//far//-1.10236*cm;//near//-1.66*cm;//Bi//

    //  ////////////////////////////OLD TELESCOPE/////////////////
    // flogicDetector3 =
    //   new G4LogicalVolume(telescopegeom,         //its solid
    //                       PVT,          //its material
    //                       "Detector_tele1");           //its name
    // new G4PVPlacement(xRot2,                       //no rotation
    //                   G4ThreeVector(0, -2.54/2*cm, -1.7145*cm-2.54/2*cm+0.066/2*mm+2*0.2794/2*mm-3*mm),                    //at position
    //                   flogicDetector3,             //its logical volume
    //                   "Detector_tele1",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // flogicDetector3->SetUserLimits(Limits);
    // G4LogicalVolume* logicteleMy =
    //   new G4LogicalVolume(telescopegeomMy,         //its solid
    //                       Mylar,          //its material
    //                       "TelescopeMy");           //its name
    // new G4PVPlacement(xRot2,                       //no rotation
    //                   G4ThreeVector(0, -2.54/2*cm, -1.7145*cm-2.54*cm-3*mm),                    //at position
    //                   logicteleMy,             //its logical volume
    //                   "TelescopeMy",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // logicteleMy->SetUserLimits(Limits);
    // G4LogicalVolume* logicteleT =
    //   new G4LogicalVolume(telescopegeomT,         //its solid
    //                       Tape,          //its material
    //                       "TelescopeT");           //its name
    // new G4PVPlacement(xRot2,                       //no rotation
    //                   G4ThreeVector(0, -2.54/2*cm, -1.7145*cm-2.54*cm-3*mm),                    //at position
    //                   logicteleT,             //its logical volume
    //                   "TelescopeT",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // logicteleT->SetUserLimits(Limits);

    // ////////////////////////////////////////////////////
    G4RotationMatrix *Rot4 = new G4RotationMatrix; // Rotates X and Z axes only
    Rot4->rotateY(0.104616657633 * rad);
    Rot4->rotateZ(3.14159265 * rad);
    // flogicDetector5 =
    //   new G4LogicalVolume(thinTelescope,         //its solid
    //                       PVT,          //its material
    //                       "Detector_tele2");           //its name
    // new G4PVPlacement(Rot4,                       //no rotation
    //                   G4ThreeVector(0, 0, -1.7145*cm+0.585*2.54*cm+3/2*mm-5*mm),                    //at position
    //                   flogicDetector5,             //its logical volume
    //                   "Detector_tele2",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // flogicDetector5->SetUserLimits(Limits);
    // G4LogicalVolume* logictelethMy =
    //   new G4LogicalVolume(thinTeleMy,         //its solid
    //                       Mylar,          //its material
    //                       "thTelescopeMy");           //its name
    // new G4PVPlacement(Rot4,                       //no rotation
    //                   G4ThreeVector(0, 0, -1.7145*cm+0.585*2.54*cm+3/2*mm-5*mm),                    //at position
    //                   logictelethMy,             //its logical volume
    //                   "thTelescopeMy",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // logictelethMy->SetUserLimits(Limits);
    // G4LogicalVolume* logictelethT =
    //   new G4LogicalVolume(thinTeleT,         //its solid
    //                       Tape,          //its material
    //                       "thTelescopeT");           //its name
    // new G4PVPlacement(Rot4,                       //no rotation
    //                   G4ThreeVector(0, 0, -1.7145*cm+0.585*2.54*cm+3/2*mm-5*mm),                    //at position
    //                   logictelethT,             //its logical volume
    //                   "thTelescopeT",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // logictelethT->SetUserLimits(Limits);

    /////////////////////////////////////////////////////////
    // G4LogicalVolume* logicBiSource =
    //   new G4LogicalVolume(BiSource,         //its solid
    //                       Al,          //its material
    //                       "BiSource");           //its name
    // new G4PVPlacement(0,                       //no rotation
    //                   G4ThreeVector(0, 0, possource-0.3437/2*cm),                    //at position
    //                   logicBiSource,             //its logical volume
    //                   "BiSource",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // logicBiSource->SetUserLimits(Limits);
    G4RotationMatrix *RotSource = new G4RotationMatrix; // Rotates X and Z axes only
    RotSource->rotateX(2 * 3.14159265 * rad);
    G4LogicalVolume *logicSourceMy =
        new G4LogicalVolume(SourceMy,                 // its solid
                            Mylar,                    // its material
                            "SourceMy");              // its name
    new G4PVPlacement(RotSource,                      // no rotation
                      G4ThreeVector(0, 0, possource), // at position
                      logicSourceMy,                  // its logical volume
                      "SourceMy",                     // its name
                      logicWorld,                     // its mother volume
                      false,                          // no boolean operation
                      0,                              // copy number
                      checkOverlaps);                 // overlaps checking
    logicSourceMy->SetUserLimits(Limits);

    G4LogicalVolume *logicSourceAl =
        new G4LogicalVolume(SourceAl,                            // its solid
                            Al,                                  // its material
                            "SourceAl");                         // its name
    new G4PVPlacement(RotSource,                                 // no rotation
                      G4ThreeVector(0, 0, possource - 0.8 * cm), // at position 0.635
                      logicSourceAl,                             // its logical volume
                      "SourceAl",                                // its name
                      logicWorld,                                // its mother volume
                      false,                                     // no boolean operation
                      0,                                         // copy number
                      checkOverlaps);                            // overlaps checking
    logicSourceAl->SetUserLimits(Limits);
    G4LogicalVolume *logicSourceAlo =
        new G4LogicalVolume(SourceAlo,                         // its solid
                            Al,                                // its material
                            "SourceAlo");                      // its name
    new G4PVPlacement(RotSource,                               // no rotation
                      G4ThreeVector(0, 0, possource - 0 * cm), // at position
                      logicSourceAlo,                          // its logical volume
                      "SourceAlo",                             // its name
                      logicWorld,                              // its mother volume
                      false,                                   // no boolean operation
                      0,                                       // copy number
                      checkOverlaps);                          // overlaps checking
    logicSourceAlo->SetUserLimits(Limits);

    // G4LogicalVolume* logicSourceSS =
    //   new G4LogicalVolume(SourceSS,         //its solid
    //                       Stainless_Steel,          //its material
    //                       "SourceSS");           //its name
    // new G4PVPlacement(0,                       //no rotation
    //                   G4ThreeVector(0, 0, possource-0.01064*cm),                    //at position
    //                   logicSourceSS,             //its logical volume
    //                   "SourceSS",                //its name
    //                   logicWorld,                //its mother volume
    //                   false,                   //no boolean operation
    //                   0,                       //copy number
    //                   checkOverlaps);          //overlaps checking
    // logicSourceSS->SetUserLimits(Limits);

    // //always return the physical World
    //
    return physWorld;
}
void BM_Detector::ConstructSDandField()
{
    SDMan = G4SDManager::GetSDMpointer();

    G4VSensitiveDetector *ScinTrig = new BM_SD("Det_trig", "Det_trig_HC"); // trigger
    G4VSensitiveDetector *Scinsq = new BM_SD("Det_sq", "Det_sq_HC");
    G4VSensitiveDetector *SDWindow = new BM_SD("Det_window", "Det_wind_HC"); // window foil
    G4VSensitiveDetector *SDVac = new BM_SD("Det_vac", "Det_vac_HC");
    // G4VSensitiveDetector* Scinr2 = new BM_SD("Det_rb",  "Det_wind_HC");

    // Add the silicon detectors to the Sens.Det.Management
    SDMan->AddNewDetector(Scinsq);
    SDMan->AddNewDetector(SDVac);
    SDMan->AddNewDetector(SDWindow);
    SDMan->AddNewDetector(ScinTrig);

    //   Turn on the sensitive detectors
    flogicDetector1->SetSensitiveDetector(SDWindow);
    flogicDetector6->SetSensitiveDetector(SDVac);
    flogicDetector3->SetSensitiveDetector(Scinsq);
    flogicDetector5->SetSensitiveDetector(ScinTrig);

    // Magnetic field ----------------------------------------------------------
    G4double amplitude = 0. * gauss;
    G4double theta = 60. * degree;
    G4MagneticField *MagneticField = new G4UniformMagField(G4ThreeVector(amplitude * sin(theta * 3.141592653 / 180), 0., amplitude * cos(theta * 3.141592653 / 180)));
    G4FieldManager *globalFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    globalFieldMgr->SetDetectorField(MagneticField);
    globalFieldMgr->CreateChordFinder(MagneticField);
    flogicDetector6->SetFieldManager(globalFieldMgr, false);

    // y = 1.56951645526 cm

    // Set Shape2 as scoring volume
    fScoringVolume = logicWorld;
}