// Analysis.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-24
// --------------------------------------------------------------

#include "Analysis.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <string>

#include "G4Event.hh"
#include "G4PrimaryParticle.hh"

#include "AnalysisMessenger.hh"
#include "SACGeometry.hh"
#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis* Analysis::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis* Analysis::GetInstance()
{
	if(fInstance == 0) fInstance = new Analysis();
	return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis::Analysis()
{
	fMessenger = new AnalysisMessenger();
	fManager = new G4RootAnalysisManager();

	// Defaults
	fTotalNEvents = -1;
	fFileName = "SACRun";

	CreateParticleList();
	CreateHistograms();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis::~Analysis()
{
	delete fMessenger;
	delete fManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::CreateParticleList()
{
	fP.insert({"all", 0});
	fP.insert({"e+", 1});
	fP.insert({"e-", 2});
	fP.insert({"gamma", 3});
	fP.insert({"mu+", 4});
	fP.insert({"mu-", 5});
	fP.insert({"neutron", 6});
	fP.insert({"opticalphoton", 7});
	fP.insert({"other", 8});
	fP.insert({"pi+", 9});
	fP.insert({"pi-", 10});
	fP.insert({"pi0", 11});
	fP.insert({"proton", 12});
	fP.insert({"untracked", 13});
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::CreateHistograms()
{
	G4int fIndex1 = 0;
	G4int fIndex2 = 0;
	G4int fIndex3 = 0;
	G4int fN1DBinsX = 200;

	SACGeometry* Geo = SACGeometry::GetInstance();
	G4double fSACRows = (G4double) Geo->GetSACNRows();
	G4double fSACCols = (G4double) Geo->GetSACNCols();
	G4double fSACLayers = (G4double) Geo->GetSACNLayers();

	std::map<G4String, G4int>::iterator iter;
	G4String key;

	// Energy deposition per event / incident energy
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		key = "h1EDep_PerEvent_" + iter->first;
		fH.insert({key, fIndex1});
		fIndex1++;

		fManager->CreateH1(key,
			"% of " + iter->first + " energy deposition per event",
			fN1DBinsX + 2, 0.0 - 1.0 / fN1DBinsX, 1.0 + 1.0 / fN1DBinsX);
	}

	// Energy deposition in SAC layers / incident energy
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		key = "h1EDep_PerLayer_" + iter->first;
		fH.insert({key, fIndex1});
		fIndex1++;

		fManager->CreateH1(key,
			"% of " + iter->first + " energy deposition per SAC layer",
			(G4int) fSACLayers, 0.0, fSACLayers);
	}

	// 2D energy deposition in SAC layer (z = <#>) / incident energy
	for(G4int i = 0; i < fSACLayers; i++)
	{
		for(iter = fP.begin(); iter != fP.end(); iter++)
		{
			key = "h2EDepZ" + std::to_string(i) + "_PerLayer_" + iter->first;
			fH.insert({key, fIndex2});
			fIndex2++;

			fManager->CreateH2(key,
				"% of " + iter->first + " energy deposition in SAC layer z = " + std::to_string(i),
				(G4int) fSACRows, 0.0, fSACRows,
				(G4int) fSACCols, 0.0, fSACCols);
		}
	}

	// 3D energy deposition in SAC / incident energy
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		key = "h3EDep_SAC_" + iter->first;
		fH.insert({key, fIndex3});
		fIndex3++;

		fManager->CreateH3(key,
			"% of " + iter->first + " energy deposition in SAC",
			(G4int) fSACRows, 0.0, fSACRows,
			(G4int) fSACCols, 0.0, fSACCols,
			(G4int) fSACLayers, 0.0, fSACLayers);
	}

	// Multiplicity per event / incident energy [1/MeV]
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		key = "h1Mult_PerEvent_" + iter->first;
		fH.insert({key, fIndex1});
		fIndex1++;

		fManager->CreateH1(key,
			"number of " + iter->first + " per event / incident energy",
			fN1DBinsX + 20, 0.0, 110.0);
	}

	// Low range of multiplicity per event / incident energy [1/MeV]
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		key = "h1MultLow_PerEvent_" + iter->first;
		fH.insert({key, fIndex1});
		fIndex1++;

		fManager->CreateH1(key,
			"number of " + iter->first + " per event / incident energy (low range)",
			fN1DBinsX + 20, 0.0, 11.0);
	}

	// Track length per hit [cm]
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		key = "h1TrLen_PerHit_" + iter->first;
		fH.insert({key, fIndex1});
		fIndex1++;

		fManager->CreateH1(key,
			iter->first + " track length per hit",
			fN1DBinsX + 20, 0.0 * cm, 220.0 * cm);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::FillHistograms(const G4Event* evt)
{
	if(fTotalNEvents == -1)
		G4cout << "[Analysis::FillHistograms] ERROR: TOTAL NUMBER OF EVENTS IS " << fTotalNEvents << "!!!" << G4endl;

	// Get incident particle name & energy
	G4PrimaryParticle* primary = evt->GetPrimaryVertex()->GetPrimary();
	G4String primaryName = primary->GetParticleDefinition()->GetParticleName();
	G4double primaryInitE = primary->GetKineticEnergy();

	// Get hits collection
	SACHitsCollection* fSACCollection = (SACHitsCollection*) evt->GetHCofThisEvent()->GetHC(0);
	G4int nHits = fSACCollection->entries();

	// Hit attributes
	G4int cellID;
	G4int trackID;
	G4String particleName;
	G4String volumeName;
	G4String sdName;
	G4String creatorProcessName;
	G4double energyDeposition;
	// G4double time;
	G4double trackLength;
	// G4ThreeVector position;
	// G4ThreeVector localPosition;

	// Helpers to fill histograms, will need to scale # entries w/# groups
	std::map<G4int, G4bool> trackedHits;
	G4double fEDepPerEvent[] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4double fMultPerEvent[] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4int pIndex, x, y, z;
	G4double eNorm;

	// Filling histograms
	for(G4int i = 0; i < nHits; i++)
	{
		// Get current hit and attributes
		SACHit* currentHit = (*fSACCollection)[i];
		cellID = currentHit->GetCellID();
		trackID = currentHit->GetTrackID();
		particleName = currentHit->GetParticleName();
		volumeName = currentHit->GetVolumeName();
		sdName = currentHit->GetSDName();
		// DON'T USE FOR NOW, CAUSING SEGFAULTS -- TRY AGAIN AFTER OPTICAL BOUNDARIES DEFINED
		// creatorProcessName = currentHit->GetCreatorProcessName();
		energyDeposition = currentHit->GetEnergyDeposition();
		// time = currentHit->GetTime();
		trackLength = currentHit->GetTrackLength();
		// position = currentHit->GetPosition();
		// localPosition = currentHit->GetLocalPosition();

		try { pIndex = fP.at(particleName); }
		catch(std::out_of_range)
		{
			// G4cout << "[Analysis::FillHistograms] Particle not yet added to list --- " << particleName << G4endl;
			pIndex = fP.at("other");
			particleName = "other";
		}

		x = cellID % 10;		// x = which row (0-9)
		y = cellID / 10 % 10;	// y = which column (0-9)
		z = cellID / 100;		// z = which layer (0 = front, 1, 2, 3 = back)

		eNorm = energyDeposition / primaryInitE / fTotalNEvents;

		// Increment energy deposition in event
		fEDepPerEvent[fP.at("all")] += energyDeposition;
		fEDepPerEvent[pIndex] += energyDeposition;

		// Fill "once per hit" histograms
		// Energy deposition in SAC layers / incident energy
		fManager->FillH1(
			fH.at("h1EDep_PerLayer_all"),
			z, eNorm);
		fManager->FillH1(
			fH.at("h1EDep_PerLayer_" + particleName),
			z, eNorm);

		// 2D energy deposition in SAC layer (z = <#>) / incident energy
		fManager->FillH2(
			fH.at("h2EDepZ" + std::to_string(z) + "_PerLayer_all"),
			x, y, eNorm);
		fManager->FillH2(
			fH.at("h2EDepZ" + std::to_string(z) + "_PerLayer_" + particleName),
			x, y, eNorm);

		// 3D energy deposition in SAC / incident energy
		fManager->FillH3(
			fH.at("h3EDep_SAC_all"),
			x, y, z, eNorm);
		fManager->FillH3(
			fH.at("h3EDep_SAC_" + particleName),
			x, y, z, eNorm);

		// Track length per hit
		fManager->FillH1(
			fH.at("h1TrLen_PerHit_all"),
			trackLength, 1.0);
		fManager->FillH1(
			fH.at("h1TrLen_PerHit_" + particleName),
			trackLength, 1.0);

		// Increment multiplicity in the event
		if(trackedHits[trackID] == false)
		{
			trackedHits[trackID] = true;
			fMultPerEvent[fP.at("all")]++;
			fMultPerEvent[pIndex]++;
		}
	}
	// Take out optical photon energy, since they don't obey energy conservation
	// https://geant4-forum.web.cern.ch/t/energy-deposition-is-higher-than-the-primary-source-energy/2064/3
	fEDepPerEvent[fP.at("all")] -= fEDepPerEvent[fP.at("opticalphoton")];
	// Get true untracked energy deposition
	fEDepPerEvent[fP.at("untracked")] = primaryInitE - fEDepPerEvent[fP.at("all")];

	// Fill "once per event" histograms
	std::map<G4String, G4int>::iterator iter;
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		// Energy deposition per event / incident energy
		fManager->FillH1(
			fH.at("h1EDep_PerEvent_" + iter->first),
			fEDepPerEvent[iter->second] / primaryInitE, 1.0);

		// Multiplicity per event / incident energy
		fManager->FillH1(
			fH.at("h1Mult_PerEvent_" + iter->first),
			fMultPerEvent[iter->second] / primaryInitE, 1.0);

		// Low range of multiplicity per event / incident energy
		fManager->FillH1(
			fH.at("h1MultLow_PerEvent_" + iter->first),
			fMultPerEvent[iter->second] / primaryInitE, 1.0);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::PrintParticleList()
{
	std::map<G4String, G4int>::iterator iter;
	for(iter = fP.begin(); iter != fP.end(); iter++)
		G4cout << iter->first << " --- " << iter->second << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::PrintHistograms()
{
	std::map<G4String, G4int>::iterator iter;
	for(iter = fH.begin(); iter != fH.end(); iter++)
		G4cout << iter->first << " --- " << iter->second << G4endl;
}
