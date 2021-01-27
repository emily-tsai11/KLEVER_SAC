// HistManager.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-24
// --------------------------------------------------------------

#ifndef HistManager_H
#define HistManager_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "g4analysis.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef struct
{
	G4int index;
	G4String description;
	G4int nBinsX;
	G4double lowX;
	G4double highX;
} f1DHistInfo;

typedef struct
{
	G4int index;
	G4String description;
	G4int nBinsX;
	G4double lowX;
	G4double highX;
	G4int nBinsY;
	G4double lowY;
	G4double highY;
} f2DHistInfo;

typedef struct
{
	G4int index;
	G4String description;
	G4int nBinsX;
	G4double lowX;
	G4double highX;
	G4int nBinsY;
	G4double lowY;
	G4double highY;
	G4int nBinsZ;
	G4double lowZ;
	G4double highZ;
} f3DHistInfo;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistManagerMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistManager
{
	public:

		~HistManager();
		static HistManager* GetInstance();

		void PrintHistNames();
		void Print1DHistInfo();
		void Print2DHistInfo();
		void Print3DHistInfo();

		void SetFileName(G4String name) { fFileName = name; }
		G4String GetFileName() { return fFileName; }

		std::map<G4String, G4int> GetParticleNames() { return fParticleNames; }
		std::map<G4String, f1DHistInfo> Get1DHistInfo() { return f1DH; }
		std::map<G4String, f2DHistInfo> Get2DHistInfo() { return f2DH; }
		std::map<G4String, f3DHistInfo> Get3DHistInfo() { return f3DH; }

	protected:

		HistManager();

	private:

		static HistManager* fInstance;

		G4String fFileName;
		std::map<G4String, G4int> fParticleNames;
		std::map<G4String, f1DHistInfo> f1DH;
		std::map<G4String, f2DHistInfo> f2DH;
		std::map<G4String, f3DHistInfo> f3DH;

		friend class HistManagerMessenger;
		HistManagerMessenger* fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
