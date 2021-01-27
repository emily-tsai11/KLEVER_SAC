// HistManager.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-24
// --------------------------------------------------------------

#include "HistManager.hh"

#include "G4SystemOfUnits.hh"
#include <string>

#include "HistManagerMessenger.hh"
#include "SACGeometry.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager* HistManager::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager* HistManager::GetInstance()
{
	if(fInstance == 0) fInstance = new HistManager();
	return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager::HistManager()
{
	fMessenger = new HistManagerMessenger();

	// root save file name
	fFileName = "SACRun";

	// histogram parameters
	G4int fIndex1 = 0;
	G4int fIndex2 = 0;
	G4int fIndex3 = 0;
	G4int fN1DBinsX = 100;
	std::map<G4String, G4int>::iterator iter;

	SACGeometry* Geo = SACGeometry::GetInstance();
	G4double fSACRows = (G4double) Geo->GetSACNRows();
	G4double fSACCols = (G4double) Geo->GetSACNCols();
	G4double fSACLayers = (G4double) Geo->GetSACNLayers();

	// map particle names
	fParticleNames.insert({"all", 0});
	fParticleNames.insert({"e+", 1});
	fParticleNames.insert({"e-", 2});
	fParticleNames.insert({"gamma", 3});
	fParticleNames.insert({"mu+", 4});
	fParticleNames.insert({"mu-", 5});
	fParticleNames.insert({"neutron", 6});
	fParticleNames.insert({"opticalphoton", 7});
	fParticleNames.insert({"other", 8});
	fParticleNames.insert({"pi+", 9});
	fParticleNames.insert({"pi-", 10});
	fParticleNames.insert({"pi0", 11});
	fParticleNames.insert({"proton", 12});
	fParticleNames.insert({"untracked", 13});

	// map histogram names -- KEEP IN ALPHABETICAL ORDER (that's how maps sort)
	// percent of energy deposition per event
	for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
	{
		f1DH.insert({
			"h1EDepNorm_PerEvent_" + iter->first,
			f1DHistInfo{fIndex1,
				"% of " + iter->first + " energy deposition per event",
				fN1DBinsX, 0.0, 1.0}
		});
		fIndex1++;
	}

	// energy deposition per event
	for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
	{
		f1DH.insert({
			"h1EDep_PerEvent_" + iter->first,
			f1DHistInfo{fIndex1,
				iter->first + " energy deposition per event",
				fN1DBinsX, 0.0 * MeV, 1.0 * GeV}
		});
		fIndex1++;
	}

	// low range of multiplicity per event
	for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
	{
		f1DH.insert({
			"h1MultLow_PerEvent_" + iter->first,
			f1DHistInfo{fIndex1,
				"number of " + iter->first + " per event (low range)",
				fN1DBinsX, 0.0, 10000.0}
			});
			fIndex1++;
		}

	// multiplicity per event
	for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
	{
		f1DH.insert({
			"h1Mult_PerEvent_" + iter->first,
			f1DHistInfo{fIndex1,
				"number of " + iter->first + " per event",
				fN1DBinsX, 0.0, 100000.0}
		});
		fIndex1++;
	}

	// multiplicity per SAC layer
	for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
	{
		f1DH.insert({
			"h1Mult_PerLayer_" + iter->first,
			f1DHistInfo{fIndex1,
				"number of " + iter->first + " per SAC layer",
				(G4int) fSACLayers, 0.0, fSACLayers}
		});
		fIndex1++;
	}

	// multiplicity in 2D SAC layer z = <#>
	for(G4int i = 0; i < fSACLayers; i++)
	{
		for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
		{
			f2DH.insert({
				"h2MultZ" + std::to_string(i) + "_PerLayer_" + iter->first,
				f2DHistInfo{fIndex2,
					"number of " + iter->first + " in SAC layer z = " + std::to_string(i),
					(G4int) fSACRows, 0.0, fSACRows,
					(G4int) fSACCols, 0.0, fSACCols}
			});
			fIndex2++;
		}
	}

	// multiplicity in 3D SAC
	for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
	{
		f3DH.insert({
			"h3Mult_SAC_" + iter->first,
			f3DHistInfo{fIndex3,
				"number of " + iter->first + " in SAC",
				(G4int) fSACRows, 0.0, fSACRows,
				(G4int) fSACCols, 0.0, fSACCols,
				(G4int) fSACLayers, 0.0, fSACLayers}
		});
		fIndex3++;
	}

	// track length per hit
	for(iter = fParticleNames.begin(); iter != fParticleNames.end(); iter++)
	{
		f1DH.insert({
			"h1TrLen_PerHit_" + iter->first,
			f1DHistInfo{fIndex1,
				iter->first + " track length per hit",
				fN1DBinsX, 0.0 * cm, 202.5 * cm}
		});
		fIndex1++;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager::~HistManager()
{
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistManager::PrintHistNames()
{
	G4cout << "---------- 1D histograms ----------" << G4endl;
	std::map<G4String, f1DHistInfo>::iterator iter1;
	for(iter1 = f1DH.begin(); iter1 != f1DH.end(); iter1++) G4cout << iter1->first << G4endl;

	G4cout << "---------- 2D histograms ----------" << G4endl;
	std::map<G4String, f2DHistInfo>::iterator iter2;
	for(iter2 = f2DH.begin(); iter2 != f2DH.end(); iter2++) G4cout << iter2->first << G4endl;

	G4cout << "---------- 3D histograms ----------" << G4endl;
	std::map<G4String, f3DHistInfo>::iterator iter3;
	for(iter3 = f3DH.begin(); iter3 != f3DH.end(); iter3++) G4cout << iter3->first << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistManager::Print1DHistInfo()
{
	G4cout << "---------- List of 1D histograms ----------" << G4endl;
	std::map<G4String, f1DHistInfo>::iterator iter1;
	for(iter1 = f1DH.begin(); iter1 != f1DH.end(); iter1++)
	{
		G4cout << "Histogram name: " << iter1->first << G4endl;
		G4cout << "Histogram index: " << iter1->second.index << G4endl;
		G4cout << "Histogram description: " << iter1->second.description << G4endl;
		G4cout << "Number of X bins: " << iter1->second.nBinsX << G4endl;
		G4cout << "X bounds: (" << iter1->second.lowX << ", " << iter1->second.highX << ")" << G4endl;
		G4cout << "-------------------------------------------" << G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistManager::Print2DHistInfo()
{
	G4cout << "---------- List of 2D histograms ----------" << G4endl;
	std::map<G4String, f2DHistInfo>::iterator iter2;
	for(iter2 = f2DH.begin(); iter2 != f2DH.end(); iter2++)
	{
		G4cout << "Histogram name: " << iter2->first << G4endl;
		G4cout << "Histogram index: " << iter2->second.index << G4endl;
		G4cout << "Histogram description: " << iter2->second.description << G4endl;
		G4cout << "Number of X bins: " << iter2->second.nBinsX << G4endl;
		G4cout << "X bounds: (" << iter2->second.lowX << ", " << iter2->second.highX << ")" << G4endl;
		G4cout << "Number of Y bins: " << iter2->second.nBinsY << G4endl;
		G4cout << "Y bounds: (" << iter2->second.lowY << ", " << iter2->second.highY << ")" << G4endl;
		G4cout << "-------------------------------------------" << G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistManager::Print3DHistInfo()
{
	G4cout << "---------- List of 3D histograms ----------" << G4endl;
	std::map<G4String, f3DHistInfo>::iterator iter3;
	for(iter3 = f3DH.begin(); iter3 != f3DH.end(); iter3++)
	{
		G4cout << "Histogram name: " << iter3->first << G4endl;
		G4cout << "Histogram index: " << iter3->second.index << G4endl;
		G4cout << "Histogram description: " << iter3->second.description << G4endl;
		G4cout << "Number of X bins: " << iter3->second.nBinsX << G4endl;
		G4cout << "X bounds: (" << iter3->second.lowX << ", " << iter3->second.highX << ")" << G4endl;
		G4cout << "Number of Y bins: " << iter3->second.nBinsY << G4endl;
		G4cout << "Y bounds: (" << iter3->second.lowY << ", " << iter3->second.highY << ")" << G4endl;
		G4cout << "Number of Z bins: " << iter3->second.nBinsZ << G4endl;
		G4cout << "Z bounds: (" << iter3->second.lowZ << ", " << iter3->second.highZ << ")" << G4endl;
		G4cout << "-------------------------------------------" << G4endl;
	}
}
