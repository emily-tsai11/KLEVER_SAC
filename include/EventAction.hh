// EventAction.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef EventAction_H
#define EventAction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UserEventAction.hh"
#include "TRandom3.h"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Event;
class G4Timer;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
	public:

		EventAction(int fSeedNum);
		~EventAction();

		void BeginOfEventAction(const G4Event*);
		void EndOfEventAction(const G4Event*);
		void FillRandomEnginesStates();

		G4long GetEventID() { return fEventID; }

	private:

		G4Timer* fTimer;
		G4long fEventID;
		long fCurrentEventCount;
		TRandom3* fRandomDecayState;
		long fRanecuState[2];
		G4int fCommandLineSeed;
		// G4bool fRandomEngineStateFileExist;
		// TFile *fRandomEngineStateFile;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
