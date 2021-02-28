// Analysis.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-24
// --------------------------------------------------------------

#ifndef Analysis_H
#define Analysis_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;
class TH1D;
class TH2D;
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

		void OpenFile();
		void Write();
		void CloseFile();

		void FillHistograms(const G4Event* evt);

		void PrintHistograms();
		void PrintParticleList();

	protected:

		Analysis();

	private:

		static Analysis* fInstance;

		G4String fFileName;
		G4int fTotalNEvents;

		TFile* fOut;
		AnalysisMessenger* fMessenger;

		G4double fSACRows;
		G4double fSACCols;
		G4double fSACLayers;

		std::map<G4String, G4int> fP;
		std::map<G4String, TH1D*> fH1D;
		std::map<G4String, TH2D*> fH2D;

		void CreateParticleList();
		void CreateLogBins(int nBins, double min, double max, double* edges);
		void CreateHistograms();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
