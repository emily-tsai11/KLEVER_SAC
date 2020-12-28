// RunAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "RunAction.hh"

#include "G4RunManager.hh"

#include "G4Run.hh"
#include "G4Timer.hh"
#include "G4UnitsTable.hh"

#include <string>

#include "PrimaryGeneratorAction.hh"
#include "RunActionMessenger.hh"
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
	fTimer = new G4Timer();
	fMessenger = new RunActionMessenger();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
	delete fTimer;
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
	G4cout << "RunAction::BeginOfRunAction(): Run " << aRun->GetRunID() << " begins!" << G4endl;
	fTimer->Start();

	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// get analysis manager and open output file
	fAnalysisManager = G4AnalysisManager::Instance();
	fAnalysisManager->SetVerboseLevel(1);
	fAnalysisManager->OpenFile(fMessenger->GetFileName());

	// get histogram bounds
	const G4int nParticles = 11;

	G4double PerHitEDepBound[nParticles];
	{
		PerHitEDepBound[0] = fMessenger->GetGammaPerHitEDepBound();
		PerHitEDepBound[1] = fMessenger->GetPositronPerHitEDepBound();
		PerHitEDepBound[2] = fMessenger->GetElectronPerHitEDepBound();
		PerHitEDepBound[3] = fMessenger->GetProtonPerHitEDepBound();
		PerHitEDepBound[4] = fMessenger->GetNeutronPerHitEDepBound();
		PerHitEDepBound[5] = fMessenger->GetPionPlusPerHitEDepBound();
		PerHitEDepBound[6] = fMessenger->GetPionMinusPerHitEDepBound();
		PerHitEDepBound[7] = fMessenger->GetPionZeroPerHitEDepBound();
		PerHitEDepBound[8] = fMessenger->GetMuonPlusPerHitEDepBound();
		PerHitEDepBound[9] = fMessenger->GetMuonMinusPerHitEDepBound();
		PerHitEDepBound[10] = fMessenger->GetOptPhotPerHitEDepBound();
	}

	G4double PerHitTrLenBound[nParticles];
	{
		PerHitTrLenBound[0] = fMessenger->GetGammaPerHitTrLenBound();
		PerHitTrLenBound[1] = fMessenger->GetPositronPerHitTrLenBound();
		PerHitTrLenBound[2] = fMessenger->GetElectronPerHitTrLenBound();
		PerHitTrLenBound[3] = fMessenger->GetProtonPerHitTrLenBound();
		PerHitTrLenBound[4] = fMessenger->GetNeutronPerHitTrLenBound();
		PerHitTrLenBound[5] = fMessenger->GetPionPlusPerHitTrLenBound();
		PerHitTrLenBound[6] = fMessenger->GetPionMinusPerHitTrLenBound();
		PerHitTrLenBound[7] = fMessenger->GetPionZeroPerHitTrLenBound();
		PerHitTrLenBound[8] = fMessenger->GetMuonPlusPerHitTrLenBound();
		PerHitTrLenBound[9] = fMessenger->GetMuonMinusPerHitTrLenBound();
		PerHitTrLenBound[10] = fMessenger->GetOptPhotPerHitTrLenBound();
	}

	G4int PerEventMultMinBound[nParticles];
	G4int PerEventMultMaxBound[nParticles];
	{
		PerEventMultMinBound[0] = fMessenger->GetGammaPerEventMultMinBound();
		PerEventMultMaxBound[0] = fMessenger->GetGammaPerEventMultMaxBound();
		PerEventMultMinBound[1] = fMessenger->GetPositronPerEventMultMinBound();
		PerEventMultMaxBound[1] = fMessenger->GetPositronPerEventMultMaxBound();
		PerEventMultMinBound[2] = fMessenger->GetElectronPerEventMultMinBound();
		PerEventMultMaxBound[2] = fMessenger->GetElectronPerEventMultMaxBound();
		PerEventMultMinBound[3] = fMessenger->GetProtonPerEventMultMinBound();
		PerEventMultMaxBound[3] = fMessenger->GetProtonPerEventMultMaxBound();
		PerEventMultMinBound[4] = fMessenger->GetNeutronPerEventMultMinBound();
		PerEventMultMaxBound[4] = fMessenger->GetNeutronPerEventMultMaxBound();
		PerEventMultMinBound[5] = fMessenger->GetPionPlusPerEventMultMinBound();
		PerEventMultMaxBound[5] = fMessenger->GetPionPlusPerEventMultMaxBound();
		PerEventMultMinBound[6] = fMessenger->GetPionMinusPerEventMultMinBound();
		PerEventMultMaxBound[6] = fMessenger->GetPionMinusPerEventMultMaxBound();
		PerEventMultMinBound[7] = fMessenger->GetPionZeroPerEventMultMinBound();
		PerEventMultMaxBound[7] = fMessenger->GetPionZeroPerEventMultMaxBound();
		PerEventMultMinBound[8] = fMessenger->GetMuonPlusPerEventMultMinBound();
		PerEventMultMaxBound[8] = fMessenger->GetMuonPlusPerEventMultMaxBound();
		PerEventMultMinBound[9] = fMessenger->GetMuonMinusPerEventMultMinBound();
		PerEventMultMaxBound[9] = fMessenger->GetMuonMinusPerEventMultMaxBound();
		PerEventMultMinBound[10] = fMessenger->GetOptPhotPerEventMultMinBound();
		PerEventMultMaxBound[10] = fMessenger->GetOptPhotPerEventMultMaxBound();
	}

	G4double PerEventEDepBound[nParticles];
	{
		PerEventEDepBound[0] = fMessenger->GetGammaPerEventEDepBound();
		PerEventEDepBound[1] = fMessenger->GetPositronPerEventEDepBound();
		PerEventEDepBound[2] = fMessenger->GetElectronPerEventEDepBound();
		PerEventEDepBound[3] = fMessenger->GetProtonPerEventEDepBound();
		PerEventEDepBound[4] = fMessenger->GetNeutronPerEventEDepBound();
		PerEventEDepBound[5] = fMessenger->GetPionPlusPerEventEDepBound();
		PerEventEDepBound[6] = fMessenger->GetPionMinusPerEventEDepBound();
		PerEventEDepBound[7] = fMessenger->GetPionZeroPerEventEDepBound();
		PerEventEDepBound[8] = fMessenger->GetMuonPlusPerEventEDepBound();
		PerEventEDepBound[9] = fMessenger->GetMuonMinusPerEventEDepBound();
		PerEventEDepBound[10] = fMessenger->GetOptPhotPerEventEDepBound();
	}

	G4double PerEventInitEBound[nParticles];
	{
		PerEventInitEBound[0] = fMessenger->GetGammaPerEventInitEBound();
		PerEventInitEBound[1] = fMessenger->GetPositronPerEventInitEBound();
		PerEventInitEBound[2] = fMessenger->GetElectronPerEventInitEBound();
		PerEventInitEBound[3] = fMessenger->GetProtonPerEventInitEBound();
		PerEventInitEBound[4] = fMessenger->GetNeutronPerEventInitEBound();
		PerEventInitEBound[5] = fMessenger->GetPionPlusPerEventInitEBound();
		PerEventInitEBound[6] = fMessenger->GetPionMinusPerEventInitEBound();
		PerEventInitEBound[7] = fMessenger->GetPionZeroPerEventInitEBound();
		PerEventInitEBound[8] = fMessenger->GetMuonPlusPerEventInitEBound();
		PerEventInitEBound[9] = fMessenger->GetMuonMinusPerEventInitEBound();
		PerEventInitEBound[10] = fMessenger->GetOptPhotPerEventInitEBound();
	}

	// list particle names
	G4String ParticleNames[] = {"Gamma", "Positron", "Electron", "Proton", "Neutron",
		"PionPlus", "PionMinus", "PionZero", "MuonPlus", "MuonMinus", "OptPhot"};

	// -------------------- CREATE 1D HISTOGRAMS --------------------

	// energy deposition per hit, unweighted -- 0
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerHit_EDep",
			ParticleNames[i] + " energy deposition per hit",
			500, 0.0 * CLHEP::MeV, PerHitEDepBound[i] * CLHEP::MeV);
	// energy deposition per hit, weighted by energy deposition -- 1
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerHit_wEDep",
			ParticleNames[i] + " weighted energy deposition per hit",
			500, 0.0 * CLHEP::MeV, PerHitEDepBound[i] * CLHEP::MeV);
	// track length per hit -- 2
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerHit_TrLen",
			ParticleNames[i] + " track length per hit",
			500, 0.0 * CLHEP::cm, PerHitTrLenBound[i] * CLHEP::cm);
	// multiplicity per event -- 3
	for(G4int i = 0; i < nParticles - 1; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerEvent_Mult",
			ParticleNames[i] + " multiplicity per event / incident energy",
			500, 0, 5.0);
	fAnalysisManager->CreateH1("hOptPhot_PerEvent_Mult",
		"OptPhot multiplicity per event / incident energy",
		500, 0, 200.0);
	// multiplicity per event for threshold plots -- 4
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerEvent_lowMult",
			ParticleNames[i] + " multiplicity per event / incident energy",
			1500, PerEventMultMinBound[i], PerEventMultMaxBound[i]);
	// energy deposition per event -- 5
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerEvent_EDep",
			ParticleNames[i] + " energy deposition per event / incident energy",
			500, 0.0 * CLHEP::MeV, PerEventEDepBound[i] * CLHEP::MeV);
	// initial (PreStepPoint) energy, unweighted -- 6
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerEvent_InitE",
			ParticleNames[i] + " initial energy",
			500, 0.0 * CLHEP::MeV, PerEventInitEBound[i] * CLHEP::MeV);
	// initial (PreStepPoint) energy, weighted by initial energy -- 7
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerEvent_wInitE",
			ParticleNames[i] + " weighted initial energy",
			500, 0.0 * CLHEP::MeV, PerEventInitEBound[i] * CLHEP::MeV);
	// initial particle hits in SAC per layer -- 8
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_SAC_Mult",
			"number of " + ParticleNames[i] + " in SAC layers",
			4, 0, 4);
	// total energy deposition per event -- 99
	fAnalysisManager->CreateH1("hPerEvent_EDep",
		"total energy deposition per event",
		500, 0.0 * CLHEP::MeV, fMessenger->GetPerEventEDepBound() * CLHEP::MeV);
	// total untracked energy deposition in event -- 100
	fAnalysisManager->CreateH1("hPerEvent_UntrackedE",
		"untracked energy deposition from other particles per event",
		500, 0.0 * CLHEP::MeV, fMessenger->GetPerEventUntrackedEBound() * CLHEP::MeV);
	for(G4int i = 0; i < 4; i++) // 4 for number of layers in SAC -- start at 101
	{
		for(G4int j = 0; j < 100; j++) // 100 for 10 x 10 cells in each layer
		{
			G4String x = std::to_string(j / 10);
			G4String y = std::to_string(j % 10);
			G4String z = std::to_string(i);
			fAnalysisManager->CreateH1("h" + z + "_" + y + x + "_arrTime",
				"arrival time of Cerenkov photons",
				1000, 0.0 * CLHEP::ns, 10.0 * CLHEP::ns);
		}
	}

	// -------------------- CREATE 2D HISTOGRAMS --------------------
	// NOTE: number of cells in SAC are hard coded...

	// energy deposition vs. track length per hit -- 0
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH2("h" + ParticleNames[i] + "_PerHit_EDep_TrLen",
			ParticleNames[i] + " track length vs. energy deposition per hit",
			500, 0.0 * CLHEP::MeV, PerHitEDepBound[i] * CLHEP::MeV,
			500, 0.0 * CLHEP::cm, PerHitTrLenBound[i] * CLHEP::cm);
	// x-y plane of SAC for each particle, z = 0 (BACK) -- 1
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH2("hSACz0_" + ParticleNames[i],
			"x-y plane of " + ParticleNames[i] + " SAC hits, z = 0 (BACK)",
			10, 0, 10,
			10, 0, 10);
	// x-y plane of SAC for each particle, z = 1 -- 2
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH2("hSACz1_" + ParticleNames[i],
			"x-y plane of " + ParticleNames[i] + " SAC hits, z = 1",
			10, 0, 10,
			10, 0, 10);
	// x-y plane of SAC for each particle, z = 2 -- 3
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH2("hSACz2_" + ParticleNames[i],
			"x-y plane of " + ParticleNames[i] + " SAC hits, z = 2",
			10, 0, 10,
			10, 0, 10);
	// x-y plane of SAC for each particle, z = 3 (FRONT) -- 4
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH2("hSACz3_" + ParticleNames[i],
			"x-y plane of " + ParticleNames[i] + " SAC hits, z = 3 (FRONT)",
			10, 0, 10,
			10, 0, 10);
	// x-y plane of SAC, z = 0 (BACK) - 55
	fAnalysisManager->CreateH2("hSACz0", "x-y plane of SAC, z = 0 (BACK)",
		10, 0, 10,
		10, 0, 10);
	// x-y plane of SAC, z = 1 - 56
	fAnalysisManager->CreateH2("hSACz1", "x-y plane of SAC, z = 1",
		10, 0, 10,
		10, 0, 10);
	// x-y plane of SAC, z = 2 - 57
	fAnalysisManager->CreateH2("hSACz2", "x-y plane of SAC, z = 2",
		10, 0, 10,
		10, 0, 10);
	// x-y plane of SAC, z = 3 (FRONT) - 58
	fAnalysisManager->CreateH2("hSACz3", "x-y plane of SAC, z = 3 (FRONT)",
		10, 0, 10,
		10, 0, 10);

	// -------------------- CREATE 3D HISTOGRAMS --------------------
	// NOTE: number of cells in SAC are hard coded...

	// particle hits on the SAC -- 0
	for(G4int i = 0; i < nParticles; i++)
		fAnalysisManager->CreateH3("hSACHits_" + ParticleNames[i],
			ParticleNames[i] + " SAC hits",
			10, 0, 10,
			10, 0, 10,
			4, 0, 4);
	// hits on the SAC -- 11
	fAnalysisManager->CreateH3("hSACHits", "SAC hits",
		10, 0, 10,
		10, 0, 10,
		4, 0, 4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	fTimer->Stop();
	G4cout << "RunAction::EndOfRunAction(): Run " << aRun->GetRunID() << " completed!" << G4endl;

	// save histograms
	fAnalysisManager = G4AnalysisManager::Instance();
	fAnalysisManager->Write();
	fAnalysisManager->CloseFile();
}
