/// \file MagneticField.hh
/// \brief Definition of the MagneticField class

#ifndef MagneticField_H
#define MagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

class G4GenericMessenger;

/// Magnetic field

class MagField : public G4MagneticField
{
public:
  MagField();
  virtual ~MagField();

  virtual void GetFieldValue(const G4double point[4], double *bField) const;

  void SetField(G4double val) { fBz = val; }
  G4double GetField() const { return fBz; }

private:
  void DefineCommands();

  G4GenericMessenger *fMessenger;
  G4double fBz;
};

//-----------------------------------------------------------------------------

#endif
