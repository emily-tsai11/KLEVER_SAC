// PADME_PHYS.hh
// --------------------------------------------------------------
// PADME modular physics list
// Based on QGSP_BERT + OpticalPhotons
// --------------------------------------------------------------
// History:
//
// Adapted from Padme by Emily Tsai (emily.tsai11@gmail.com) 2020-7-13
// --------------------------------------------------------------

#ifndef _PADME_PHYS_H_
#define _PADME_PHYS_H_

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VModularPhysicsList.hh"
#include "CompileTimeConstraints.hh"
#include "globals.hh"

#include <CLHEP/Units/SystemOfUnits.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PADME_PHYS: public G4VModularPhysicsList
{
	public:

		PADME_PHYS(G4int ver = 1);
		virtual ~PADME_PHYS();

		// SetCuts()
		virtual void SetCuts();

	private:

		enum { ok = CompileTimeConstraints::IsA<G4VModularPhysicsList, G4VModularPhysicsList>::ok };
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
