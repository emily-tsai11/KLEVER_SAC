// RunAction.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#ifndef RunAction_H
#define RunAction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define N_PARTICLES 11

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VAnalysisManager.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;
class RunActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
	public:

		RunAction();
		~RunAction();

		void BeginOfRunAction(const G4Run* aRun);
		void EndOfRunAction(const G4Run* aRun);

	private:

		G4double PerHitEDepBound[N_PARTICLES];
		G4double PerHitTrLenBound[N_PARTICLES];
		G4int PerEventMultMinBound[N_PARTICLES];
		G4int PerEventMultMaxBound[N_PARTICLES];
		G4double PerEventEDepBound[N_PARTICLES];
		G4double PerEventInitEBound[N_PARTICLES];

		G4Timer* fTimer;
		G4VAnalysisManager* fAnalysisManager;
		RunActionMessenger* fMessenger;

		void ReadHistogramBounds();
		void CreateHistograms();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
