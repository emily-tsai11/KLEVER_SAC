// CustomMuonDecay.hh
// ------------- G4MuonDecayChannelWithSpin info --------------
// GEANT 4 class header file
//
//		17 August 2004 P. Gumplinger and T. MacPhail
//		samples Michel spectrum including 1st order
//		radiative corrections
//			Reference: Florian Scheck "Muon Physics", in Physics Reports
//			(Review Section of Physics Letters) 44, No. 4 (1978)
//			187-248. North-Holland Publishing Company, Amsterdam
//			at page 210 cc.
//
//			W.E. Fisher and F. Scheck, Nucl. Phys. B83 (1974) 25.
// ------------------------------------------------------------
// History:
//
// Created by Karim Massri (karim.massri@cern.ch) 2017-10-09
//		based on G4MuonDecayChannelWithSpin class (see info above)
// Adapted from KLMC by Emily Tsai (emily.tsai11@gmail.com) 2020-7-10
// --------------------------------------------------------------

#ifndef CustomMuonDecay_H
#define CustomMuonDecay_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"
#include "G4MuonDecayChannel.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// this class describes muon decay kinemtics.
// this version assumes V-A coupling with 1st order radiative correctons,
//		the standard model Michel parameter values, but
//		gives incorrect energy spectrum for neutrinos.
class CustomMuonDecay : public G4MuonDecayChannel
{
	public:

		// constructors
		CustomMuonDecay(const G4String& theParentName, G4double theBR);
		// destructor
		virtual ~CustomMuonDecay();

	protected:

		// copy constructor and assignment operator
		CustomMuonDecay(const CustomMuonDecay&);
		CustomMuonDecay & operator=(const CustomMuonDecay&);

	private:

		CustomMuonDecay();

	public:

		virtual G4DecayProducts* DecayIt(G4double);

		void SetPolarization(G4ThreeVector);
		const G4ThreeVector& GetPolarization() const;

	private:

		G4ThreeVector parent_polarization;

		// radiative correction factors
		G4double F_c(G4double x, G4double x0);
		G4double F_theta(G4double x, G4double x0);
		G4double R_c(G4double x);

		G4double EMMU;
		G4double EMASS;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void CustomMuonDecay::SetPolarization(G4ThreeVector polar)
{
	parent_polarization = polar;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline const G4ThreeVector& CustomMuonDecay::GetPolarization() const
{
	return parent_polarization;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double CustomMuonDecay::F_c(G4double x, G4double x0)
{
	G4double omega = std::log(EMMU / EMASS);
	G4double f_c;

	f_c = (5.0 + 17.0 * x - 34.0 * x * x) * (omega + std::log(x)) - 22.0 * x + 34.0 * x * x;
	f_c = (1.0 - x) / (3.0 * x * x) * f_c;
	f_c = (6.0 - 4.0 * x) * R_c(x) + (6.0 - 6.0 * x) * std::log(x) + f_c;
	f_c = (CLHEP::fine_structure_const / CLHEP::twopi) * (x * x - x0 * x0) * f_c;

	return f_c;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double CustomMuonDecay::F_theta(G4double x, G4double x0)
{
	G4double omega = std::log(EMMU / EMASS);
	G4double f_theta;

	f_theta = (1.0 + x + 34 * x * x) * (omega + std::log(x)) + 3.0 - 7.0 * x - 32.0 * x * x;
	f_theta = f_theta + ((4.0 * (1.0 - x) * (1.0 - x)) / x) * std::log(1.0 - x);
	f_theta = (1.0 - x) / (3.0 * x * x) * f_theta;
	f_theta = (2.0 - 4.0 * x) * R_c(x) + (2.0 - 6.0 * x) * std::log(x) - f_theta;
	f_theta = (CLHEP::fine_structure_const / CLHEP::twopi) * (x * x - x0 * x0) * f_theta;

	return f_theta;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
