// HistManager.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-1-24
// --------------------------------------------------------------

#ifndef HistManager_H
#define HistManager_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "g4analysis.hh"

#include <map>
#include <utility>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistManager
{
	public:

		~HistManager();
		static HistManager* GetInstance();

		void PrintHistogramNames();
		void PrintHistogramBounds();

		void SetFileName(G4String name) { fFileName = name; }
		G4String GetFileName() { return fFileName; }

		void SetNBins(G4int nBins) { fNBins = nBins; }
		G4int GetNBins() { return fNBins; }

		std::map<G4String, G4int> GetHistogramNames() { return fHistogramNames; }
		std::map<G4String, std::pair<G4double, G4double>> GetHistogramBounds() { return fHistogramBounds; }

	protected:

		HistManager();

	private:

		static HistManager* fInstance;
		// HistManagerMessenger* fMessenger;

		G4String fFileName;
		G4int fNBins;
		std::map<G4String, G4int> fHistogramNames;
		std::map<G4String, std::pair<G4double, G4double>> fHistogramBounds;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
