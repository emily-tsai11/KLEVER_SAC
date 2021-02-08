// PADMEPhysics.hh
// --------------------------------------------------------------
// PADME modular physics list
// Based on QGSP_BERT + OpticalPhotons
// --------------------------------------------------------------
// History:
//
// Adapted from PADME by Emily Tsai (emily.tsai11@gmail.com) 2020-7-13
// --------------------------------------------------------------

#ifndef PADMEPhysics_H
#define PADMEPhysics_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VModularPhysicsList.hh"
#include "CompileTimeConstraints.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PADMEPhysics : public G4VModularPhysicsList
{
	public:

		PADMEPhysics(G4int ver = 0);
		virtual ~PADMEPhysics();
		virtual void SetCuts();

	private:

		enum { ok = CompileTimeConstraints::IsA<G4VModularPhysicsList, G4VModularPhysicsList>::ok };
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
