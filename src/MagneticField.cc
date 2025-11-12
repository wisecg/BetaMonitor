/// \file MagneticField.cc
/// \brief Implementation of the MagneticField class

#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

MagField::MagField() : G4MagneticField(), fMessenger(nullptr), fBz(0.0 * gauss)
{
  DefineCommands();
}

MagField::~MagField()
{
  delete fMessenger;
}

void MagField::GetFieldValue(const G4double[4], double *bField) const
{
  bField[0] = 0.;
  bField[1] = fBz * sin(60 * degree);
  bField[2] = fBz * cos(60 * degree);
}

void MagField::DefineCommands()
{
  // Define /main/field command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/main/field/",
                                      "Field control");

  // fieldValue command
  auto &valueCmd = fMessenger->DeclareMethodWithUnit("value", "gauss",
                                                     &MagField::SetField,
                                                     "Set field strength.");
  valueCmd.SetParameterName("field", true);
  valueCmd.SetDefaultValue("0.");
}
