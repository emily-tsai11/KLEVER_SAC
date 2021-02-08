// Analysis.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-24
// --------------------------------------------------------------

#ifndef Analysis_H
#define Analysis_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RootAnalysisManager.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class AnalysisMessenger;
class G4Event;
class G4String;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Analysis
{
	public:

		~Analysis();
		static Analysis* GetInstance();

		void SetFileName(G4String name) { fFileName = name; }
		G4String GetFileName() { return fFileName; }

		void SetTotalNEvents(G4int n) { fTotalNEvents = n; }
		G4int GetTotalNEvents() { return fTotalNEvents; }

		void OpenFile() { fManager->OpenFile(fFileName); }
		void Write() { fManager->Write(); }
		void CloseFile() { fManager->CloseFile(); }

		void FillHistograms(const G4Event* evt);

		void PrintHistograms();
		void PrintParticleList();

	protected:

		Analysis();

	private:

		static Analysis* fInstance;

		G4RootAnalysisManager* fManager;
		AnalysisMessenger* fMessenger;

		G4String fFileName;
		G4int fTotalNEvents;

		std::map<G4String, G4int> fP;
		std::map<G4String, G4int> fH;

		void CreateParticleList();
		void CreateHistograms();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
