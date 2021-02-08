// SteppingAction.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#ifndef SteppingAction_H
#define SteppingAction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UserSteppingAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
	public:

		SteppingAction();
		~SteppingAction();

		void SetVerboseLevel(G4int v) { fVerboseLevel = v; }
		G4int GetVerboseLevel() { return fVerboseLevel; }

		void UserSteppingAction(const G4Step*);
		void PrintStep(const G4Step*, G4String ParticleName = "");

	private:

		G4int fVerboseLevel;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
