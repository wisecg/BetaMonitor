// nat_units.hh
// Getting tired of other systems without natural units
// Creator: Benjamin Schroeder
// 2017

#ifndef NAT_UNITS_HH
#define NAT_UNITS_HH
// #define MEHLMAN_UNITS // Use the outdated/imprecise units that Mehlman used

namespace NAT
{
   // Mathematical constants
#ifdef MEHLMAN_UNITS
   static constexpr double pi = 3.1415926535; // Mehlman's
   static constexpr double e = 2.71828183;    // Mehlman's
#else
   static constexpr double pi = 3.141592653589793238;
   static constexpr double e = 2.718281828459045235;
#endif // MEHLMAN_UNITS

   // Unit system
   static constexpr double c = 1.;
   static constexpr double hbar = 1.;
   static constexpr double MeV = 1.;

   // Derived masses, energies
   static constexpr double GeV = 1.e+3 * MeV;
   static constexpr double keV = 1.e-3 * MeV;
   static constexpr double eV = 1.e-6 * MeV;
   static constexpr double kg = eV / c / c / (1.782661907e-36);
   static constexpr double gram = kg / 1000;
   static constexpr double ton = 1000 * kg;
   static constexpr double joule = eV / 1.6021766208e-19;

   // Derived lengths, times
   static constexpr double second = hbar / MeV / (6.582119514e-22);
   static constexpr double minute = 60. * second;
   static constexpr double hour = 60. * minute;
   static constexpr double day = 24. * hour;
   static constexpr double year = 365. * day;
   static constexpr double meter = c * second / 299792458.;
   static constexpr double km = 1.e+3 * meter;
   static constexpr double cm = 1.e-2 * meter;
   static constexpr double mm = 1.e-3 * meter;
   static constexpr double um = 1.e-6 * meter;
   static constexpr double nm = 1.e-9 * meter;
   static constexpr double angstrom = 1.e-10 * meter;
   static constexpr double pm = 1.e-12 * meter;
   static constexpr double fm = 1.e-15 * meter;

   // Derived areas
   static constexpr double cm2 = cm * cm;
   static constexpr double barn = 1.e-24 * cm2;

   // Physical constants and particular properties
#ifdef MEHLMAN_UNITS
   static constexpr double alpha_em = 7.297352537e-3;          // Mehlman's
   static constexpr double GF = 1.11637e-5 / GeV / GeV;        // Fermi constant // Mehlman's
   static constexpr double mass_e = 0.510998910 * MeV / c / c; // Mehlman's
   static constexpr double mass_p = 938.272013 * MeV / c / c;  // Mehlman's
   static constexpr double AMU = 931.494028 * MeV / c / c;     // Mehlman's
#else
   static constexpr double alpha_em = 7.2973525664e-3;
   static constexpr double GF = 1.1163787e-5 / GeV / GeV; // Fermi constant
   static constexpr double mass_e = 0.5109989461 * MeV / c / c;
   static constexpr double mass_p = 938.2720813 * MeV / c / c;
   static constexpr double AMU = 931.4940954 * MeV / c / c;
#endif // MEHLMAN_UNITS

   static constexpr double k_boltzmann = 8.6173303e-5 * eV; // / Kelvin

}

#endif // NAT_UNITS_HH
