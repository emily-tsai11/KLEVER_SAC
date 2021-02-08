// RunAction.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#ifndef RunAction_H
#define RunAction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UserRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class G4Timer;
class G4VAnalysisManager;
class HistManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
	public:

		RunAction();
		~RunAction();

		void BeginOfRunAction(const G4Run* aRun);
		void EndOfRunAction(const G4Run* aRun);

		void SetNumEvents(G4int n) { fNumEvents = n; }
		G4int GetNumEvents() { return fNumEvents; }

	private:

		G4int fNumEvents;
		G4Timer* fTimer;
		G4VAnalysisManager* fAnalysisManager;
		HistManager* fHistManager;

		void CreateHistograms();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
