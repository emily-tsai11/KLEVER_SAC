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
#include "G4LorentzVector.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
	public:

		SteppingAction(EventAction*);
		~SteppingAction();

		void UserSteppingAction(const G4Step*);
		void PrintStep(const G4Step*, G4String ParticleName = "");

	private:

		EventAction* fEventAction;
		G4LorentzVector fInitialMomentum;
		G4LorentzVector fFinalMomentum;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
